#include "emu_file.h"

int32_t open_directory(const char* path){
    static char buffer[MAX_FILE_LENGTH];
    int32_t result = 0;

    #ifdef _WIN32
        //I have no idea.
        return 1;
    #elif __APPLE__
        snprintf(buffer, MAX_FILE_LENGTH, "open -R \"%s\"", path);
        result = system(buffer); //Opens current directory
    #elif __linux__
        snprintf(buffer, MAX_FILE_LENGTH, "xdg-open \"%s\"", path);
        result = system(buffer);
    #else
        fprintf(stderr, "Unknown Operating System.\n");
        return 1;
    #endif

    return result;
}