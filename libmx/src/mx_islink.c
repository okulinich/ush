#include "libmx.h"

bool mx_islink(char *file) {
    struct stat st;

    if (lstat(file, &st) == -1)
        return false;
    if ((st.st_mode & S_IFLNK) == S_IFLNK)
        return true;
    return false;
}
