#ifndef OSINT_H
#define OSINT_H

/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>
#include <stdbool.h>

#include "stuff.h"
/*============================================================================*/

/*============================================================================*/
#define MAX_WORD_LEN 100
#define MAX_URL_LEN 200
/*============================================================================*/

/*============================================================================*/
void geturl(char *datalist, char ***list, int *list_size, char *name);
char *delchar(char *line);
int check(char *url, int *count, bool);
/*============================================================================*/

/*============================================================================*/
int find(char *name)
{
    // Set default values
    //char *name = "";
    char *datalist = "./lists/data.json";
    char **list = NULL;
    int list_size = 0;
    int count = 0;
    bool verbose = false;

    printf(
      "\n###################################\n"
      "For a better osint tool use GOSINT.\n"
      "https://github.com/Fastiraz/gosint\n"
      "###################################\n\n"
    );

    /* C0de */
    geturl(datalist, &list, &list_size, name);
    for(int i=0; i < list_size; i++){
        remove_n(list[i]);
        check(list[i], &count, verbose);
    }
    free(list);

    return 0;
}
/*============================================================================*/

/*============================================================================*/
void geturl(char *datalist, char ***list, int *list_size, char *name){
    char *start, *end;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    FILE *fp = fopen(datalist, "r");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        if(strstr(line, "\"url\":") != NULL){
            sprintf(strstr(line, "{}"), "%s\n", name); // Replace '{}' by the username
            for(int i=0; i<14; i++){ // For each characters before the url
                line[i] = ' '; // Replace char to get '              https://www.github.com/Fastiraz'
            }
            line = delchar(line); // Remove the space before url
            *list = (char**) realloc(*list, (*list_size + 1) * sizeof(char*)); // Allocate space for one more string
            (*list)[(*list_size)++] = strdup(line); // Add the new string to the array and increment size
        }
    }

    fclose(fp);
    if (line) {
        free(line);
    }
}
/*============================================================================*/

/*============================================================================*/
// Function to delete n characters
char *delchar(char *line){
    char ch = ' ';
    int len = strlen(line);
    for(int i=0; i<len; i++){
		if(line[i] == ch){
			for(int j=i; j<len; j++)
			{
				line[j] = line[j + 1];
			}
			len--;
			i--;	
		} 
	}
    return line;
}
/*============================================================================*/

/*============================================================================*/
int check(char *url, int *count, bool verbose){
  CURL* curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set request method to HEAD
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    // Perform request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK && verbose) {
      fprintf(stderr, "%s\t | \x1B[35m%s\x1B[0m\n", curl_easy_strerror(res), url);
    } else {
      // Get status code
      long status_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
      if (status_code >= 200 && status_code < 300) {
        // Page exists
        printf("Username exists\t\t\t");
        printf(" | \033[0;32m%s\033[0m\n", url);
        (*count)++;
      } else {
        // Page does not exist
        if (verbose) {
            printf("Username does not exist\t\t");
            printf(" | \033[0;31m%s\033[0m\n", url);
            (*count)++;
        }
      }
    }

    // Clean up
    curl_easy_cleanup(curl);
  }
  return 0;
}
/*============================================================================*/

#endif /* OSINT_H */