#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define RESET 'r'
#define NEXT 'n'
#define PREVIOUS 'p'
#define ART_SIZE 60

typedef enum { false, true } bool;

int getOptionFromMenu() {
  int option;
  printf("Asciiart device menu: \n");
  printf("0. Exit\n");
  printf("1. Current \n");
  printf("2. Reset \n");
  printf("3. Next \n");
  printf("4. Previous \n");
  printf("Type your option: ");
  scanf("%d", &option);
  return option;
}

void current(int fd) {
  char buffer[ART_SIZE];

  if (read (fd, buffer, ART_SIZE) < ART_SIZE) {
    printf("asciiart read");
    return;
  }

  printf("\n");
  printf("%s\n", buffer);
}

void requestNewFrame(int fd, char command) {
  if (write (fd, &command, 1) < 1) {
    printf("asciiart write");
    return;
  }

  current(fd);
  
}

void reset(int fd) {
  requestNewFrame(fd, RESET);
}

void next(int fd) {
  requestNewFrame(fd, NEXT);
}

void previous(int fd) {
  requestNewFrame(fd, PREVIOUS);
}

int main() {
  int fd;
  int option;

  fd = open("/dev/asciiart", O_RDWR);
  if (fd == -1) {
    printf("asciiart open");
    return 1;
  }

  do {
    option = getOptionFromMenu();

    switch(option) {
      case 0: break;
      case 1:
        current(fd);
        break;
      case 2:
        reset(fd);
        break;
      case 3:
        next(fd);
        break;
      case 4:
        previous(fd);
        break;
      default:
        printf("Invalid option!\n");
    }
    printf("\n\n");
  } while (option != 0);

  close (fd);
  return 0; 
}
