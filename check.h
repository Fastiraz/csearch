#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define UP_ARROW    65
#define DOWN_ARROW  66
#define ENTER       10
#define SPACE       32

int getch_check(void);
void display_check_menu(int, char **, int, bool *);
void mark_checkbox(int, bool *);

/*void checked_menu(bool *recurs, bool *verb) {
  int choice = 0;
  bool checkboxes[] = {false, false, false, false};
  char *options[] = { "Recursive", "Verbose", "Help", "Set wordlist" };
  int options_size = sizeof(options) / sizeof(options[0]);

  while (1) {
    display_check_menu(choice, options, options_size, checkboxes);
    switch (getch_check()) {
      case UP_ARROW:
        if (choice > 0) {
          choice--;
        }
        break;
      case DOWN_ARROW:
        if (choice < 3) {
          choice++;
        }
        break;
      case SPACE:
        mark_checkbox(choice, checkboxes);
        break;
      case ENTER:
        if (choice == 4) {
          printf("\nI will be waiting for you here -_-\n");
          exit(0);
        } else {
          for (int i = 0; i < options_size; i++) {
            if (checkboxes[i]) {
              if (strcmp(options[i], "Recursive") == 0) {
                recurs = true;
              } else if (strcmp(options[i], "Recursive") == 0) {
                verb = true;
              }
            }
          }
          sleep(1);
        }
        break;
    }
  }
}*/

void checked_menu(bool *recurs, bool *verb) {
  int choice = 0;
  bool checkboxes[] = {false, false};
  //char *options[] = { "Recursive", "Verbose", "Help", "Set wordlist" };
  char *options[] = { "Recursive", "Verbose" };
  int options_size = sizeof(options) / sizeof(options[0]);

  while (1) {
    display_check_menu(choice, options, options_size, checkboxes);
    switch (getch_check()) {
      case UP_ARROW:
        if (choice > 0) {
          choice--;
        }
        break;
      case DOWN_ARROW:
        if (choice < 3) {
          choice++;
        }
        break;
      case SPACE:
        mark_checkbox(choice, checkboxes);
        break;
      case ENTER:
        if (choice == 4) {
          printf("\nI will be waiting for you here -_-\n");
          exit(0);
        } else {
          for (int i = 0; i < options_size; i++) {
            if (checkboxes[i]) {
              if (strcmp(options[i], "Recursive") == 0) {
                *recurs = true;
              } else if (strcmp(options[i], "Verbose") == 0) {
                *verb = true;
              }
            }
          }
          return;
        }
        break;
    }
  }
}


int getch_check(void) {
  struct termios oldattr, newattr;
  int ch;
  tcgetattr(STDIN_FILENO, &oldattr);
  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
  return ch;
}

void display_check_menu(int choice, char **options, int options_size, bool *checkboxes) {
  system("clear");
  printf("%sMENU%s [use the space key to select items]\n\n", KYEL, KNRM);
  for (int i = 0; i < options_size; i++) {
    printf("   %s ", checkboxes[i] ? "[✓]" : "[ ]");
    if (i == choice) {
      printf("%s%s%s\n", KMAG, options[i], KNRM);
    } else {
      printf("%s\n", options[i]);
    }
  }
}

void mark_checkbox(int choice, bool *checkboxes) {
  checkboxes[choice] = !checkboxes[choice];
}

/*int main(void) {
  checked_menu();
  return 0;
}*/

#endif /* CHECK_H */
