#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void print_directory(char *dirname);

int main(int argc, char **argv) {
    char *dirname;

    if (argc == 1) {
        dirname = ".";
    } else if (argc == 2) {
        dirname = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        return 1;
    }

    print_directory(dirname);

    return 0;
}

void print_directory(char *dirname) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirname);
    if (!dir) {
        fprintf(stderr, "Cannot open directory %s\n", dirname);
        return;
    }

    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[1024];
        sprintf(path, "%s/%s", dirname, entry->d_name);

        if (entry->d_type == DT_DIR) {
            printf("Directory: %s\n", entry->d_name);
            print_directory(path);
        } else {
            printf("Filename: %s\n", entry->d_name);
        }
    }

    closedir(dir);
}
