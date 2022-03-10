#include "./logger.h"

void write_log(char *path, char* event) {
    time_t rawtime = time(NULL);
    struct tm *time_info;
    char time_string[25];
    time_info = localtime(&rawtime);
    strftime(time_string, 25, "%b/%d/%C -%l:%M:%S", time_info);
    FILE *fptr = fopen(path, "a+");
    fprintf(fptr, "[%s] %s\n", time_string, event);
    fclose(fptr);
}
