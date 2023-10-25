 #ifndef GITHUB_H
 #define GITHUB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        fprintf(stderr, "Out of memory!\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char *grab_email(const char *body) {
    const char *email_start = strstr(body, "\"email\":\"");
    
    if (email_start == NULL) {
        return NULL; // Email field not found
    }

    email_start += strlen("\"email\":\"");
    
    const char *email_end = strchr(email_start, '"');
    
    if (email_end == NULL) {
        return NULL; // Malformed email field
    }

    int email_length = email_end - email_start;

    char *email = (char *)malloc(email_length + 1);
    if (email == NULL) {
        return NULL; // Memory allocation error
    }

    strncpy(email, email_start, email_length);
    email[email_length] = '\0'; // Null-terminate the string

    return email;
}

char* github_email(char* username) {
    char url[1024];
    snprintf(url, sizeof(url), "https://api.github.com/users/%s/events/public", username);

    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = NULL;
    chunk.size = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set user-agent header (GitHub API requires it).
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: MyCurlClient/1.0");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the callback function to handle received data.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

        // Perform the request.
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // The response body is in chunk.memory.
            char *email = grab_email(chunk.memory);
            printf("Email:\t%s\n", email);
        }

        curl_slist_free_all(headers); // Clean up the headers list
        curl_easy_cleanup(curl);
        
        if (chunk.memory) {
            free(chunk.memory); // Free the dynamically allocated memory.
        }
    }

    return 0;
}

 #endif /* GITHUB_H */
