#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat filestat;
    if (stat(argv[1], &filestat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("Size: %ld bytes\n", filestat.st_size); // file size
    printf("Blocks: %ld\n", filestat.st_blocks); // blocks allocated
    printf("Links: %ld\n", filestat.st_nlink); // link count

    // file permissions
    printf("Permissions: ");
    printf((S_ISDIR(filestat.st_mode)) ? "d" : "-");
    printf((filestat.st_mode & S_IRUSR) ? "r" : "-");
    printf((filestat.st_mode & S_IWUSR) ? "w" : "-");
    printf((filestat.st_mode & S_IXUSR) ? "x" : "-");
    printf((filestat.st_mode & S_IRGRP) ? "r" : "-");
    printf((filestat.st_mode & S_IWGRP) ? "w" : "-");
    printf((filestat.st_mode & S_IXGRP) ? "x" : "-");
    printf((filestat.st_mode & S_IROTH) ? "r" : "-");
    printf((filestat.st_mode & S_IWOTH) ? "w" : "-");
    printf((filestat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    // file inode
    printf("Inode: %ld\n", filestat.st_ino);

    return 0;
}
