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



}

int main(int argc, char *argv[]) {
    char dirpath[PATH_MAX];
    char filepath[PATH_MAX];

    DIR *dir;
    struct dirent *dirent;
    struct stat filestat;
}