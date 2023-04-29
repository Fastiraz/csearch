#ifndef DDF_H
#define DDF_H

/*============================================================================*/
/* For Dir */
#include <curl/curl.h>
#include <time.h>

/* For DNS */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* For stuff */
#include "stuff.h"
/*============================================================================*/


/*============================================================================*/
int dir(char *url, char *word, bool verbose){
  CURL* curl;
  CURLcode res;

  // Construct URL
  char full_url[256];
  sprintf(full_url, "%s%s/", url, word);

  curl = curl_easy_init();
  if (curl) {
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, full_url);

    // Set request method to HEAD
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    // Perform request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK && verbose) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    } else {
      // Get status code
      long status_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
      if (status_code >= 200 && status_code < 300) {
        // Page exists
        printf("Page exists\t\t\t");
        printf(" | \033[0;32m%s\033[0m\n", full_url);
      } else {
        // Page does not exist
        if(verbose){
          printf("Page does not exist\t\t");
          printf(" | \033[0;31m%s\033[0m\n", full_url);
        }
      }
    }

    // Clean up
    curl_easy_cleanup(curl);
  }
  return 0;
}
/*============================================================================*/


/*============================================================================*/
int dns(char *domain, char *word, bool verbose){
    // Allocate memory for the DNS string
    size_t dns_len = strlen(word) + strlen(domain) + 2;
    char *dns = malloc(dns_len);
    if (dns == NULL) {
        return 1;
    }

    // Concatenate the word and domain with a "." separator
    snprintf(dns, dns_len, "%s.%s", word, domain);

    // Create a hints structure to specify the type of address we are interested in
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // Use a stream socket (TCP)
    hints.ai_flags = AI_CANONNAME; // Request the canonical name of the host

    // Call getaddrinfo to resolve the domain name
    struct addrinfo *result;
    int error = getaddrinfo(dns, NULL, &hints, &result);

    if (error != 0) {
        // getaddrinfo failed, subdomain does not exist
        if(verbose){
          printf("Subdomain does not exist\t");
          printf(" | \033[0;31m%s\033[0m\n", dns);
        }
        return 1;
    } else {
        // getaddrinfo succeeded, subdomain exists
        printf("Subdomain exists\t\t");
        printf(" | \033[0;32m%s\033[0m\n", dns);
        return 0;
    }

    return 0;
}
/*============================================================================*/


/*============================================================================*/
int fuzz(char *url, char *word, bool verbose){
  CURL* curl;
  CURLcode res;

  // Construct URL with replaced word
  char full_url[256];
  char *fuzz_ptr = strstr(url, "fuzz");
  if (fuzz_ptr != NULL) {
    strncpy(full_url, url, fuzz_ptr - url);
    full_url[fuzz_ptr - url] = '\0';
    strcat(full_url, word);
    strcat(full_url, fuzz_ptr + 4);
  } else {
    strcpy(full_url, url);
  }

  curl = curl_easy_init();
  if (curl) {
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, full_url);

    // Set request method to HEAD
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    // Perform request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK && verbose) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    } else {
      // Get status code
      long status_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
      if (status_code >= 200 && status_code < 300) {
        // Page exists
        printf("Page exists\t\t\t");
        printf(" | \033[0;32m%s\033[0m\n", full_url);
      } else {
        // Page does not exist
        if(verbose){
          printf("Page does not exist\t\t");
          printf(" | \033[0;31m%s\033[0m\n", full_url);
        }
      }
    }

    // Clean up
    curl_easy_cleanup(curl);
  }
  return 0;
}
/*============================================================================*/


#endif /* DDF_H */
