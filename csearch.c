/*
 ===============================================================================
 Name        : csearch.c
 Author      : Fastiraz
 Version     : 1.5
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

/* For Dir, DNS & Fuzz */
#include "ddf.h"

/* For Recursive */
#include "recursive.h"

/* For stuff */
#include "stuff.h"

/* For user interface */
#include "ui.h"
#include "check.h"

/* OSINT on username */
#include "osint.h"

/* security headers */
#include "sechead.h"
/*============================================================================*/

/*============================================================================*/
#define MAX_WORD_LEN 100
#define MAX_URL_LEN 200
#define MAX_PATH_LENGTH 256
/*============================================================================*/

/*============================================================================*/
// int dir(char *url, char *word, bool);
void remove_n(char *str);
void banner();
void info(char*, char*, bool, char*, bool);
void start();
void end();
void help();
// int dns(char *url, char *word, bool);
// int fuzz(char *url, char *word, bool);
/*============================================================================*/

/*============================================================================*/
int main(int argc, char **argv){
  // Set default values for the base URL and wordlist file
  char *base_url = "http://example.com/";
  char *wordlist = "lists/wordlist.txt";
  char *algo = "dir";
  bool verbose = false;
  bool recursive = false;

  // Parse the command-line arguments
  for (int i = 1; i < argc; i++) {
      // if (argc < 2)
      //     help();
      if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
          base_url = argv[++i];
      } else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
          wordlist = argv[++i];
      } else if (strcmp(argv[i], "dir") == 0 && i + 1 < argc) {
          algo = argv[i];
      } else if (strcmp(argv[i], "dns") == 0 && i + 1 < argc) {
          algo = argv[i];
      } else if (strcmp(argv[i], "fuzz") == 0 && i + 1 < argc) {
          algo = argv[i];
      } else if (strcmp(argv[i], "-v") == 0) {
          verbose = true;
          //i++;
      } else if (strcmp(argv[i], "-h") == 0) {
          banner();
          help();
          exit(0);
      } else if (strcmp(argv[i], "-r") == 0) {
          recursive = true;
          // printf("Recursive mode: %s", recursive ? "true" : "false");
      } else if (strcmp(argv[i], "-ui") == 0) {
          algo = menu();
          checked_menu(&recursive, &verbose);
      } else if (strcmp(argv[i], "-n") == 0) {
          find(argv[++i]);
      } else if (strcmp(argv[i], "-sh") == 0) {
          sechead(argv[++i]);
          return 0;
      } else {
          help();
          return 1;
      }
  }

  banner();
  info(base_url, wordlist, verbose, algo, recursive);
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
      if(!recursive){
        dir(base_url, line, verbose);
      } else {
        recursive_dir(base_url, wordlist, verbose);
      }
    } else if (strcmp(algo, "dns") == 0){
      if(!recursive){
        dns(base_url, line, verbose);
      } else {
        recursive_dns(base_url, wordlist, verbose);
      }
    } else if (strcmp(algo, "fuzz") == 0){
      fuzz(base_url, line, verbose);
    } else if (strcmp(algo, "sechead") == 0) {
      char target[256];
      printf("Enter the target URL: ");
        if (fgets(target, sizeof(target), stdin) == NULL) {
            // Handle fgets error (e.g., input too long)
            printf("Error reading target URL\n");
            return 1;
        }
      target[strcspn(target, "\n")] = '\0';
      sechead(target);
      return 0;
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
