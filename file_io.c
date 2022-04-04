#include <stdio.h>
#include <unistd.h>

#include "file_io.h"

FILE *wrap_fopen(const char *filename, const char *modes) {
    FILE *file = fopen(filename, modes);
    if (!file) {
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) == -1) {
        fclose(file);
        return NULL;
    }

    long size = ftell(file);

    if (size == 1) {
        fclose(file);
        return NULL;
    }

    if (fseek(file, 0, SEEK_SET) == -1) {
        fclose(file);
        return NULL;
    }

    char contents[size];

    if (fread(contents, 1, size, file) != (size_t) size) {
        fclose(file);
        return NULL;
    }

    return file;
}
