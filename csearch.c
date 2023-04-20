/*
 ===============================================================================
 Name        : csearch.c
 Author      : Fastiraz
 Version     : 1.2
 Copyright   : Your copyright notice
 Description : A simple tool for brute-forcing URLs in C.
 Compile     : gcc -o csearch csearch.c -lcurl
 Usage       : ./csearch http://example.com /usr/share/wordlists/dirb/common.txt
 ===============================================================================
 */

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
/*============================================================================*/

/*============================================================================*/
#define MAX_WORD_LEN 100
#define MAX_URL_LEN 200
/*============================================================================*/

/*============================================================================*/
int dir(char *url, char *word, bool);
void remove_n(char *str);
void banner();
void info(char*, char*, bool);
void start();
void end();
void help();
int dns(char *url, char *word, bool);
/*============================================================================*/

/*============================================================================*/
int main(int argc, char **argv)
{
  // Set default values for the base URL and wordlist file
  char *base_url = "http://example.com/";
  char *wordlist = "wordlist.txt";
  char *algo = "dir";
  bool verbose = false;

  // Parse the command-line arguments
  for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
          base_url = argv[++i];
      } else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
          wordlist = argv[++i];
      } else if (strcmp(argv[i], "dir") == 0 && i + 1 < argc) {
          algo = argv[i];
      } else if (strcmp(argv[i], "dns") == 0 && i + 1 < argc) {
          algo = argv[i];
      } else if (strcmp(argv[i], "-v") == 0) {
          verbose = true;
          //i++;
      } else {
          fprintf(stderr, "Usage: %s -u url\n", argv[0]);
          return 1;
      }
  }
  banner();
  info(base_url, wordlist, verbose);
  start();
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  FILE *fp = fopen(wordlist, "r");
  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    remove_n(line);
    if (strcmp(algo, "dir") == 0){
      dir(base_url, line, verbose);
    } else if (strcmp(algo, "dns") == 0){
      wordlist = "dns-wordlist.txt";
      dns(base_url, line, verbose);
    } else {
      printf("\nUnexpected error while starting the attack.\n");
      return 1;
    }

  }

  fclose(fp);
  if (line) {
    free(line);
  }
  end();

  return 0;
}
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
void remove_n(char *str){
    int len = strlen(str);
    // Loop through the string and replace \n with \0
    for (int i = 0; i < len; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
}
/*============================================================================*/

/*============================================================================*/
void banner(){
  printf("\n\n===============================================================\n");
  printf("CSearch v1.2\n");
  printf("by Fastiraz\n");
  printf("===============================================================\n");
}
/*============================================================================*/

/*============================================================================*/
void info(char *url, char *wdl, bool verbose){
  printf("[+] Url:\t\t\t%s\n", url);
  printf("[+] Threads:\t\t\t1\n");
  printf("[+] Wordlist:\t\t\t%s\n", wdl);
  printf("[+] Status codes:\t\t200,204,301,302,307,401,403\n");
  printf("[+] User Agent:\t\t\tCSearch/1.0.0\n");
  printf("[+] Timeout:\t\t\t10s\n");
  printf("[+] Verbose:\t\t\t%s\n", verbose ? "true" : "false");
}
/*============================================================================*/

/*============================================================================*/
void start(){
  // Get current calendar time
  time_t current_time = time(NULL);

  // Convert to local time
  struct tm* local_time = localtime(&current_time);

  // Create a buffer to hold the date and time string
  char date_time_str[32];

  // Use strftime to format the date and time
  strftime(date_time_str, 32, "%Y/%m/%d %H:%M:%S", local_time);

  // Print the date and time
  printf("===============================================================\n");
  printf("%s Starting CSearch...\n", date_time_str);
  printf("===============================================================\n");
}
/*============================================================================*/

/*============================================================================*/
void end(){
  // Get current calendar time
  time_t current_time = time(NULL);

  // Convert to local time
  struct tm* local_time = localtime(&current_time);

  // Create a buffer to hold the date and time string
  char date_time_str[32];

  // Use strftime to format the date and time
  strftime(date_time_str, 32, "%Y/%m/%d %H:%M:%S", local_time);

  // Print the date and time
  printf("===============================================================\n");
  printf("%s Finished\n", date_time_str);
  printf("===============================================================\n\n\n");
}
/*============================================================================*/

/*============================================================================*/
void help(){
  printf("\nFLAGS:\n");
  printf("\t-u : URL\n");
  printf("\t-w : Path to a custom wordlist\n");
  printf("\t-v : Verbose output (errors)\n");
  printf("\t-h : Display this content\n");
  printf("\nKEYWORDS:\n");
  printf("\tdir : Directory mode (default)");
  printf("\tdns : Subdomain mode");
  printf("\nEXAMPLES:\n");
  printf("\tUsage :\t./csearch -u http://example.com/\n");
  printf("\tUsage :\t./csearch dns -u http://example.com/ -w /usr/share/wordlist/dirb/big.txt -v\n");
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
