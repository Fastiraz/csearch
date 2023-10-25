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
  printf("\tCSearch v1.1.6\n");
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
  printf("[+] User Agent:\t\t\tCSearch/1.1.6\n");
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
void help() {
  const char *items[] = {
    "\nFLAGS:\n",
    "\t-u  : URL\n",
    "\t-w  : Path to a custom wordlist\n",
    "\t-v  : Verbose output (errors)\n",
    "\t-r  : Enable recursive mode\n",
    "\t-ui : User Interface\n",
    "\t-sh : Check Security Headers\n",
    "\t-n  : Search for a username on social media (osint)\n",
    "\t-h  : Display this content\n",
    "\t-ge : Find email address from Github username\n",
    "\nKEYWORDS:\n",
    "\tdir  : Directory mode (default)\n",
    "\tdns  : Subdomain mode\n",
    "\tfuzz : Uses fuzzing mode. Replaces the keyword FUZZ in the URL, Headers and the request body\n",
    "\nEXAMPLES:\n",
    "\tUsage :\t./csearch dir -u http://example.com/ -r\n",
    "\tUsage :\t./csearch dns -u example.com -w lists/dns-wordlist.txt -v\n",
    "\tUsage :\t./csearch fuzz -u http://fuzz.example.com/ -w lists/dns-wordlist.txt\n",
    "\tUsage :\t./csearch -ui -u http://example.com/\n",
    "\tUsage :\t./csearch -sh http://example.com/\n",
    "\tUsage :\t./csearch -ge username\n",
    "\tUsage :\t./csearch -n username\n"
  };

  int item = sizeof(items) / sizeof(items[0]);

  for (int i = 0; i < item; i++) {
    printf("%s", items[i]);
  }
}
/*============================================================================*/


#endif /* STUFF_H */
