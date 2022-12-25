/*
 ===============================================================================
 Name        : csearch.c
 Author      : Fastiraz
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : A simple tool for brute-forcing URLs in C.
 Compile     : gcc -o csearch csearch.c -lcurl
 Usage       : ./csearch http://example.com /usr/share/wordlists/dirb/common.txt
 ===============================================================================
 */

/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>
/*============================================================================*/

/*============================================================================*/
int search(char *url, char *word);
void remove_n(char *str);
void banner();
void info();
void start();
void end();
/*============================================================================*/

/*============================================================================*/
int main(int argc, char **argv)
{
  banner();
  info(argv[1], argv[2]);
  start();
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  FILE *fp = fopen(argv[2], "r");
  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    remove_n(line);
    search(argv[1], line);
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
int search(char *url, char *word){
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
    if (res != CURLE_OK) {
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
        printf("Page does not exist\t\t");
        printf(" | \033[0;31m%s\033[0m\n", full_url);
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
  printf("CSearch v1.0.0\n");
  printf("by Fastiraz\n");
  printf("===============================================================\n");
}
/*============================================================================*/

/*============================================================================*/
void info(char *url, char *wdl){
  printf("[+] Url:\t\t\t%s\n", url);
  printf("[+] Threads:\t\t\t1\n");
  printf("[+] Wordlist:\t\t\t%s\n", wdl);
  printf("[+] Status codes:\t\t200,204,301,302,307,401,403\n");
  printf("[+] User Agent:\t\t\tCSearch/1.0.0\n");
  printf("[+] Timeout:\t\t\t10s\n");
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



// Yes, I copied Gobuster for the output :)

/*
$ gobuster dir -u http://example.com -w /usr/share/wordlists/dirb/common.txt 

===============================================================
Gobuster v3.0.1
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@_FireFart_)
===============================================================
[+] Url:            http://example.com
[+] Threads:        10
[+] Wordlist:       /usr/share/wordlists/dirb/common.txt
[+] Status codes:   200,204,301,302,307,401,403
[+] User Agent:     gobuster/3.0.1
[+] Timeout:        10s
===============================================================
2020/11/22 22:02:24 Starting gobuster
===============================================================
/css (Status: 200)
/js (Status: 200)
/admin (Status: 301)
/login (Status: 200)
/static (Status: 301)
/images (Status: 200)
/fonts (Status: 200)
/docs (Status: 200)
/downloads (Status: 200)
/blog (Status: 200)
/faq (Status: 200)
/contact (Status: 200)
/about (Status: 200)
/info (Status: 200)
===============================================================
2020/11/22 22:02:34 Finished
===============================================================
*/