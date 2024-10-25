#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

// Describing the type of the file
const char* type(mode_t mode) {
    switch (mode & S_IFMT) {
        case S_IFREG: return "REG";
        case S_IFDIR: return "DIR";
        case S_IFSOCK: return "SOCK";
        case S_IFLNK: return "LNK";
        case S_IFBLK: return "BLK";
        case S_IFCHR: return "CHR";
        case S_IFIFO: return "FIFO";
        default: return "UNKNOWN";
    }
}
// filtering out non printable or non ascii characters
void get_file_preview(const char* filepath, char* preview) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("open");
        strcpy(preview, "");
        return;
    }
    char buffer[17]; // storing in buffer

    ssize_t bytes_read = read(fd, buffer, 16);
    if (bytes_read == -1) {
        perror("read");
        strcpy(preview, "");
    } 
    else {
        for (ssize_t i = 0; i < bytes_read; ++i) {
            if (isprint(buffer[i]) && (buffer[i] < 128)) {
                preview[i] = buffer[i];
            } 
            else {
                preview[i] = ' ';
            }
        }
        preview[bytes_read] = '\0';
    }
    close(fd);

}

int main(int argc, char *argv[]) {
    char dirpath[PATH_MAX];
    char filepath[PATH_MAX];

    DIR *dir;
    struct dirent *dirent;
    struct stat filestat;

    if (argc == 1) {
        strcpy(dirpath, ".");
    } 
    else if (argc == 2) {
        strcpy(dirpath, argv[1]);
    } 
    else {
        fprintf(stderr, "too many args\n");
        return EXIT_FAILURE;
    }
    dir = opendir(dirpath);
    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }
 
    errno = 0;
    while ((dirent = readdir(dir)) != NULL) {
        snprintf(filepath, PATH_MAX, "%s/%s", dirpath, dirent->d_name);

        // Get file stats
        if (stat(filepath, &filestat) == -1) {
            perror("stat");
            continue;
        }
        printf("%16.16s\t", dirent->d_name);
        printf("%" PRIdMAX "\t", (intmax_t)filestat.st_nlink);
        printf("%s\t", type(filestat.st_mode));
        if (S_ISDIR(filestat.st_mode)) {
            DIR *subdir = opendir(filepath);
            if (subdir == NULL) {
                printf("unknown\t");
            } else {
                int count = 0;
                while (readdir(subdir)) {
                    count++;
                }
                printf("%d\t", count);
                closedir(subdir);
            }
        } else {
            printf("%" PRIdMAX "\t", (intmax_t)filestat.st_size);
        }
        if (S_ISREG(filestat.st_mode)) {
            char preview[17];
            get_file_preview(filepath, preview);
            printf("%s", preview);
        }

        printf("\n");
    }
    if (errno != 0) {
        perror("readdir");
        closedir(dir);
        return EXIT_FAILURE;
    }

    if (closedir(dir) == -1) {
        perror("closedir");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}