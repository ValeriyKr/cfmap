#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int c, char **v) {
  if (c != 2) {
    fprintf(stderr, "USAGE: %s <func_path>\n", v[0]);
    return EXIT_SUCCESS;
  }

  int func_fd;
  if (-1 == (func_fd = open(v[1], O_RDONLY))) {
    perror("open binary file");
    return EXIT_FAILURE;
  }

  struct stat st_buf;
  if (-1 == fstat(func_fd, &st_buf)) {
    perror("fstat");
    return EXIT_FAILURE;
  }
  size_t size = (size_t)st_buf.st_size;
  if (0 == size) {
    fprintf(stderr, "zero size executable bytecode");
    return EXIT_FAILURE;
  }

  void *func = mmap(NULL, size, PROT_READ|PROT_EXEC, MAP_PRIVATE, func_fd, 0);
  if (MAP_FAILED == func) {
    perror("mmap");
    return EXIT_FAILURE;
  }
  void (*fmap)(char*, char*) = (void (*)(char*, char*))func;

  char *line;
  size_t n;
  do {
    line = NULL;
    n = 0;
    ssize_t read_chars = getline(&line, &n, stdin);
    if (-1 == read_chars) {
      if (feof(stdin)) {
        break;
      }
      perror("getline");
      return EXIT_FAILURE;
    }
    if (0 == --read_chars) {
      goto print;
    }
    fmap(line, line + read_chars - 1);
print:
    printf("%s", line);
    free(line);
  } while (1);

  return EXIT_SUCCESS;
}
