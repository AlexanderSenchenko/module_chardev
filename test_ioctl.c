#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_LEN 80

#define CHARDEV_READ  100
#define CHARDEV_WRITE 101

void ioct_get_msg(int fd)
{
  char msg[BUF_LEN];
  int ret = ioctl(fd, CHARDEV_READ, msg);
  if (ret < 0) {
    printf("Error: ioct_get_msg\n");
    exit(EXIT_FAILURE);
  }
  printf("msg: %s", msg);
}

void ioct_set_msg(int fd, char *msg)
{
  int ret = ioctl(fd, CHARDEV_WRITE, msg);
  if (ret < 0) {
    printf("Error: ioct_set_msg\n");
    exit(EXIT_FAILURE);
  }
}

int main()
{
  int fd = open("/dev/chardev", 0);
  if (fd < 0) {
    printf("Error: open dev file\n");
    exit(EXIT_FAILURE);
  }

  ioct_get_msg(fd);
  ioct_set_msg(fd, "Hi dev");

  close(fd);
  return 0;
}