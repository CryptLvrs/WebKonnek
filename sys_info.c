#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fptr = fopen("/proc/loadavg", "r");
    if (fptr == 0) {
        printf("Error : Unable to open file called 'loadavg'.\n");
        return 1;
    }

    printf("Success : 'loadavg' file is open.\n");

    char fline[15];
    if (fgets(fline, sizeof(fline), fptr) == NULL) {
        printf("Error : Failed to read from file.\n");
        fclose(fptr);
        return 1;
    }

    float load1min, load5min, load15min;
    if (sscanf(fline, "%f %f %f", &load1min, &load5min, load15min) == -1) {
        printf("Error : There isn't any float number for one min load average in file 'loadavg'.\n");
        fclose(fptr);
        return 1;
    }

    printf("load1 : %.2f\n", load1min);
    printf("load5 : %.2f\n", load5min);
    printf("load 15 : %.2f\n", load15min);

    return 0;
}
