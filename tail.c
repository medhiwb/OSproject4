#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BLOCK_SIZE 1024
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: mytail -n file\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1] + 1);
    if (n <= 0) {
        fprintf(stderr, "Invalid number of lines: %d\n", n);
        exit(EXIT_FAILURE);
    }
    char *filename = argv[2];
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        fprintf(stderr, "Error getting file size: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    off_t bytes_to_read = BLOCK_SIZE;
    if (bytes_to_read > file_size) {
        bytes_to_read = file_size;
    }
    off_t pos = file_size - bytes_to_read;
    char buf[bytes_to_read];
    ssize_t num_bytes = pread(fd, buf, bytes_to_read, pos);
    if (num_bytes == -1) {
        fprintf(stderr, "Error reading file %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    int num_lines = 0;
    for (off_t i = num_bytes - 1; i >= 0; i--) {
        if (buf[i] == '\n') {
            num_lines++;
            if (num_lines == n) {
                printf("These are the last %d lines of %s:\n", n, argv[2]);
                printf("%.*s", (int) (num_bytes - i - 1), buf + i + 1);
                break;
            }
        }
    }
    if (num_lines < n) {
        off_t bytes_remaining = pos;
        while (bytes_remaining > 0 && num_lines < n) {
            bytes_to_read = MIN(BLOCK_SIZE, bytes_remaining);
            bytes_remaining -= bytes_to_read;
            pos -= bytes_to_read;
            num_bytes = pread(fd, buf, bytes_to_read, pos);
            if (num_bytes == -1) {
                fprintf(stderr, "Error reading file %s: %s\n", filename, strerror(errno));
                exit(EXIT_FAILURE);
            }
            for (off_t i = num_bytes - 1; i >= 0; i--) {
                if (buf[i] == '\n') {
                    num_lines++;
                    if (num_lines == n) {
                        printf("%.*s", (int) (num_bytes - i - 1), buf + i + 1);
                        break;
                    }
                }
            }
        }
    }
    close(fd);
    return 0;
}

