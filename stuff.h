#ifndef STUFF_H
#define STUFF_H


/*============================================================================*/
/* std */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
  printf("\tCSearch v1.5\n");
  printf("\tby Fastiraz\n");
  printf("===============================================================\n");
}
/*============================================================================*/


/*============================================================================*/
void info(char *url, char *wdl, bool verbose, char *algo, bool recursive){
  printf("[+] Url:\t\t\t%s\n", url);
  printf("[+] Mode:\t\t\t%s\n", algo);
  printf("[+] Threads:\t\t\t1\n");
  printf("[+] Wordlist:\t\t\t%s\n", wdl);
  printf("[+] Status codes:\t\t200,204,301,302,307,401,403\n");
  printf("[+] User Agent:\t\t\tCSearch/1.0.0\n");
  printf("[+] Timeout:\t\t\t10s\n");
  printf("[+] Verbose:\t\t\t%s\n", verbose ? "true" : "false");
  printf("[+] Recursive:\t\t\t%s\n", recursive ? "true" : "false");
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
  printf("\t-u  : URL\n");
  printf("\t-w  : Path to a custom wordlist\n");
  printf("\t-v  : Verbose output (errors)\n");
  printf("\t-r  : Enable recurcive mode\n");
  printf("\t-ui : User Interface\n");
  printf("\t-sh : Chech Security Headers\n");
  printf("\t-n  : Search for a username on social media (osint)\n");
  printf("\t-h  : Display this content\n");
  printf("\t-gu : Find email address from Github username\n");
  printf("\nKEYWORDS:\n");
  printf("\tdir  : Directory mode (default)\n");
  printf("\tdns  : Subdomain mode\n");
  printf("\tfuzz : Uses fuzzing mode. Replaces the keyword FUZZ in the URL, Headers and the request body\n");
  printf("\nEXAMPLES:\n");
  printf("\tUsage :\t./csearch -u http://example.com/\n");
  printf("\tUsage :\t./csearch dns -u http://example.com/ -w /usr/share/wordlist/dirb/big.txt -v\n");
}
/*============================================================================*/


#endif /* STUFF_H */
