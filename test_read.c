#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  int fd = open("/dev/chardev", O_RDONLY);

  char buf[80];

  int size = read(fd, buf, 80);

  printf("size = %d\n", size);
  write(1, buf, size);

  close(fd);
  return 0;
}