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



// int main(int argc, char* argv[]) {
//     if (argc != 3) {
//         fprintf(stderr, "Usage: %s -n <num_lines> <filename>\n", argv[0]);
//         exit(EXIT_FAILURE);
//     }

//     int n = atoi(argv[1] + 1); // Skip the '-' character
//     if (n <= 0) {
//         fprintf(stderr, "Invalid number of lines: %d\n", n);
//         exit(EXIT_FAILURE);
//     }

//     const char* filename = argv[2];

//     int fd = open(filename, O_RDONLY);
//     if (fd == -1) {
//         perror("open");
//         exit(EXIT_FAILURE);
//     }

//     off_t file_size = lseek(fd, 0, SEEK_END);
//     off_t block_start = BLOCK_SIZE;

//     if (block_start > file_size) {
//         block_start = file_size;
//     }

//     char block[BLOCK_SIZE + 1];
//     off_t block_end = file_size;

//     while (block_end > 0) {
//         off_t block_read_size = BLOCK_SIZE;
//         if (block_end < block_start) {
//             block_read_size = block_end;
//         }

//         block_end = block_start;
//         block_start -= block_read_size;

//         if (lseek(fd, block_start, SEEK_SET) == -1) {
//             perror("lseek");
//             exit(EXIT_FAILURE);
//         }

//         ssize_t num_read = read(fd, block, block_read_size);
//         if (num_read == -1) {
//             perror("read");
//             exit(EXIT_FAILURE);
//         }

//         block[num_read] = '\0';

//         // Count number of lines in block
//         int line_count = 0;
//         for (int i = num_read - 1; i >= 0; i--) {
//             if (block[i] == '\n') {
//                 line_count++;
//                 if (line_count >= n) {
//                     // Print remaining lines in block
//                     printf("%s", block + i + 1);
//                     break;
//                 }
//             }
//         }

//         if (line_count < n && block_start == 0) {
//             // Print the remaining lines in the file
//             printf("%s", block);
//         }
//         printf("\n");
//     }

//     close(fd);

//     return 0;
// }
