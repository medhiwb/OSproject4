#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_file_info(const char* filename) {
    struct stat filestat;
    if (stat(filename, &filestat) == -1) {
        perror("stat");
        return;
    }

    printf("%-10s ", (S_ISDIR(filestat.st_mode)) ? "directory" : "file");
    printf("%8ld ", filestat.st_size);

    struct passwd *pw = getpwuid(filestat.st_uid);
    if (pw != NULL) {
        printf("%-10s ", pw->pw_name);
    } else {
        printf("%-10d ", filestat.st_uid);
    }

    struct group *gr = getgrgid(filestat.st_gid);
    if (gr != NULL) {
        printf("%-10s ", gr->gr_name);
    } else {
        printf("%-10d ", filestat.st_gid);
    }

    char time_str[80];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&filestat.st_mtime));
    printf("%s ", time_str);

    printf("%s\n", filename);
}

void print_directory(const char* dirname, int long_format) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        if (long_format) {
            print_file_info(path);
        } else {
            printf("%s\n", entry->d_name);
        }

        if (entry->d_type == DT_DIR) {
            print_directory(path, long_format);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    int long_format = 0;
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    int opt;
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
            case 'l':
                long_format = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    const char* dirname = (optind < argc) ? argv[optind] : ".";

    printf("Current directory: %s\n", cwd);
    print_directory(dirname, long_format);
    printf("Search has finished.");

    return 0;
}

