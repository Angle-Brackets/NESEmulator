#ifndef NESEMULATOR_EMU_FILE_H
#define NESEMULATOR_EMU_FILE_H
#include <string.h>
#include "pixelinator.h"

#define MAX_FILE_LENGTH 256

/**
 * Cross-platform function to open a directory for the user to view.
 * @param path File path to directory
 * @return 0 if the function was successful, nonzero on any error.
 */
int32_t open_directory(const char* path);

#endif //NESEMULATOR_EMU_FILE_H
