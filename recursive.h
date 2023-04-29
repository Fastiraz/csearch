#ifndef RECURSIVE_H
#define RECURSIVE_H


/*============================================================================*/
/* std */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
int recursive_dir(char* url, char* wordlist, bool verbose) {
	FILE *file = fopen(wordlist, "r"); // open file in read mode
  if (file == NULL) {
    perror("Error opening file"); // print error message if file cannot be opened
    return -1;
  }

  char line[256]; // buffer to store each line
  while (fgets(line, sizeof(line), file) != NULL) { // read one line at a time until EOF
		CURL* curl;
	  CURLcode res;

	  // Construct URL
	  char full_url[256];
		remove_n(line);
	  sprintf(full_url, "%s%s/", url, line);

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
					recursive_dir(full_url, wordlist, verbose);
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
  }

  fclose(file); // close the file
  return 0;
}
/*============================================================================*/


/*============================================================================*/
int recursive_dns(char *domain, char *wordlist, bool verbose){
	FILE *file = fopen(wordlist, "r"); // open file in read mode
  if (file == NULL) {
      perror("Error opening file"); // print error message if file cannot be opened
      return -1;
  }

  char line[256]; // buffer to store each line
  while (fgets(line, sizeof(line), file) != NULL) { // read one line at a time until EOF
		// Remove the f*cking \n
		remove_n(line);

		// Allocate memory for the DNS string
		size_t dns_len = strlen(line) + strlen(domain) + 2;
		char *dns = malloc(dns_len);
		if (dns == NULL) {
				return 1;
		}

		// Concatenate the word and domain with a "." separator
		snprintf(dns, dns_len, "%s.%s", line, domain);

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
		} else {
				// getaddrinfo succeeded, subdomain exists
				printf("Subdomain exists\t\t");
				printf(" | \033[0;32m%s\033[0m\n", dns);
				recursive_dns(dns, wordlist, verbose);
		}
  }

  fclose(file); // close the file
  return 0;
}
/*============================================================================*/


#endif /* RECURSIVE_H */
