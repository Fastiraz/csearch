#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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

int getch(void);
void display_menu(int, char **, int);

char* menu() {
  int choice = 0;
  char *options[] = {"Option 1", "Option 2", "Option 3", "Exit"};
	int options_size = sizeof(options) / sizeof(options[0]);

  char *mode[] = { "Directory", "Domain", "Fuzzing", "Security Header", "Quit" };
  int mode_size = sizeof(mode) / sizeof(mode[0]);

  while (1) {
    display_menu(choice, mode, mode_size);
    switch (getch()) {
      case UP_ARROW:
        if (choice > 0) {
          choice--;
        }
        break;
      case DOWN_ARROW:
        if (choice < 4) {
          choice++;
        }
        break;
      case ENTER:
        if (choice == 4) {
          printf("\nI will be waiting for you here -_-\n");
          exit(0);
        } else if (choice == 0) {
          return "dir";
        } else if (choice == 1) {
          return "dns";
        } else if (choice == 2) {
          return "fuzz";
        } else if (choice == 3) {
          return "sechead";
        } else {
          printf("Error while determine attack mode.\n");
          exit(1);
        }
        break;
    }
  }
}

int getch(void) {
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

void display_menu(int choice, char **options, int options_size) {
  system("clear");
  printf("%sMENU%s\n\n", KYEL, KNRM);
  for (int i = 0; i < options_size; i++) {
    if (i == choice) {
      printf("➜ %s%s%s\n", KMAG, options[i], KNRM);
    } else {
      printf("   %s\n", options[i]);
    }
  }
}

void restart(void) {
	printf("KNV");
}

#endif /* UI_H */
