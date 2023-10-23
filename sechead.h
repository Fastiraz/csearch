#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <time.h> // DEBUG

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// A structure to hold the captured header data
struct HeaderData {
    char *data;
    size_t size;
};

size_t write_header_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    struct HeaderData *header_data = (struct HeaderData *)userdata;
    size_t header_size = size * nmemb;

    // Append the header data to the existing buffer
    size_t newSize = header_data->size + header_size;
    header_data->data = realloc(header_data->data, newSize + 1);
    if (header_data->data) {
        memcpy(header_data->data + header_data->size, ptr, header_size);
        header_data->size = newSize;
        header_data->data[newSize] = '\0'; // Null-terminate the buffer
    }

    return header_size;
}

void check_headers(const char *data, int *found) {
    /*char *sec_headers[] = {
        "X-XSS-Protection",
        "X-Frame-Options",
        "X-Content-Type-Options",
        "Strict-Transport-Security",
        "Content-Security-Policy",
        "X-Permitted-Cross-Domain-Policies",
        "Referrer-Policy",
        "Expect-CT",
        "Permissions-Policy",
        "Cross-Origin-Embedder-Policy",
        "Cross-Origin-Resource-Policy",
        "Cross-Origin-Opener-Policy"
    };*/

    char *sec_headers[] = {
        "x-xss-protection",
        "x-frame-options",
        "x-content-type-options",
        "strict-transport-security",
        "content-security-policy",
        "x-permitted-cross-domain-policies",
        "referrer-policy",
        "expect-ct",
        "permissions-policy",
        "cross-origin-embedder-policy",
        "cross-origin-resource-policy",
        "cross-origin-opener-policy"
    };

    printf("Checking headers:\n");

    for (int i = 0; i < sizeof(sec_headers) / sizeof(sec_headers[0]); i++) {
        if (strstr(data, sec_headers[i])) {
            //*found++;
            *found += 1;
            printf("[+] Found:\t\x1b[32m%s\x1b[0m\n", sec_headers[i]);
        } else {
            printf("[!] Not Found:\t\x1b[31m%s\x1b[0m\n", sec_headers[i]);
        }
    }
}

bool check_url_format(char *url) {
    if (strncmp(url, "http://", 7) == 0 || strncmp(url, "https://", 8) == 0) {
        return true;
    } else {
        return false;
    }
}

int sechead(char *url) {
    // Add these two lines for execution time measurement
    clock_t start_time = clock();
    double execution_time;

    if (check_url_format(url)) {
        printf("[*] Analyzing headers of \x1b[35m%s\x1b[0m\n", url);
    } else {
        printf("[*] Invalid URL format \x1b[33m%s\x1b[0m", url);
    }

    int found = 0;


    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl = curl_easy_init();
    if (!curl) {
        printf("Failed to initialize libcurl\n");
        return 1;
    }

    // Set the URL to analyze
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Enable header output
    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    // Create a structure to hold header data
    struct HeaderData header_data = {NULL, 0};
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &header_data);

    // Set the callback function to handle header data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_header_callback);

    // Disable SSL verification (for testing purposes only, not recommended for production use)
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Perform the HTTP request
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("Failed to perform the HTTP request: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(header_data.data); // Clean up allocated memory
        return 1;
    }

    // Clean up libcurl resources
    curl_easy_cleanup(curl);

    // Now header_data.data contains the captured header output
    printf(
        "\nCaptured Header:\n"
        "%s\n",
        header_data.data
    );

    // Compare headers
    check_headers(header_data.data, &found);

    // Scan result
    //int unfound = 12 - found;
    printf(
        "-------------------------------------------------------\n"
        "[!] Headers analyzed for %s\n"
        "[+] There are \x1b[32m%d\x1b[0m security headers\n"
        "[-] There are not \x1b[31m%d\x1b[0m security headers\n",
        url, found, 12-found
    );

    // Clean up allocated memory
    free(header_data.data);

    // Calculate the execution time
    clock_t end_time = clock();
    execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("[*]Execution Time: \x1b[33m%.6f\x1b[0m seconds\n", execution_time);

    return 0;
}