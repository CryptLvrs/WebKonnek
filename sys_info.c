#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fptr;
    fptr = fopen("/proc/loadavg", "r");
    if (fptr == 0) {
        printf("Error : Unable to open file called 'loadavg'.\n");
        return 1;
    }

    printf("Success : 'loadavg' file is open.\n");

    char fline[5];

    if (fgets(fline, sizeof(fline), fptr) == NULL) {
        printf("Error : Failed to read from file.\n");
        fclose(fptr);
        return 1;
    }

    char *endptr;
    float loadst = strtof(fline, &endptr);

    if (endptr == NULL) {
        printf("Error : There isn't any float number for one min load average in file 'loadavg'.\n");
        fclose(fptr);
        return 1;
    }

    printf("%.2f\n", loadst);

    return 0;
}
