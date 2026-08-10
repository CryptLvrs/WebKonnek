#include <stdio.h>
#include <stdlib.h>

int main() {

    // 'loadavg' part
    FILE *fptr = fopen("/proc/loadavg", "r");
    if (fptr == 0) {
        printf("Error : Unable to open file called 'loadavg'.\n");
        return 1;
    }
    printf("Success : 'loadavg' file is open.\n");

    char fline[128];
    if (fgets(fline, sizeof(fline), fptr) == NULL) {
        printf("Error : Failed to read from file.\n");
        fclose(fptr);
        return 1;
    }

    float load1min, load5min, load15min;
    if (sscanf(fline, "%f %f %f", &load1min, &load5min, &load15min) != 3) {
        printf("Error : There isn't any float number for one min load average in file 'loadavg'.\n");
        fclose(fptr);
        return 1;
    }

    // 'meminfo' part
    FILE *fptr2 = fopen("/proc/meminfo", "r");
    if (fptr2 == NULL) {
        printf("Error : Unable to open file called 'meminfo'.\n");
        fclose(fptr);
        return 1;
    }
    printf("Success : 'meminfo' file is open.\n");

    char memline[256];
    long memory_total;
    long memory_free;

    if (fgets(memline, sizeof(memline), fptr2) == NULL) {
        printf("Error : Failed to read from memory information file.\n");
        fclose(fptr2);
        fclose(fptr);
        return 1;
    }

    if (sscanf(memline, "MemTotal: %ld", &memory_total) != 1) {
        printf("Error : There isn't any integer for 'MemTotal' in file 'meminfo'.\n");
        fclose(fptr2);
        fclose(fptr);
        return 1;
    }

    if (fgets(memline, sizeof(memline), fptr2) == NULL) {
        printf("Error : Failed to read from memory information file.(l2)\n");
        fclose(fptr2);
        fclose(fptr);
        return 1;
    }

    if (sscanf(memline, "MemFree: %ld", &memory_free) != 1) {
        printf("Error : There isn't any integer for 'MemFree' in file 'meminfo'.\n");
        fclose(fptr2);
        fclose(fptr);
        return 1;
    }

    // Temp output ('JSON' "format") 
    printf("{\"load1\": %.2f, \"load5\": %.2f, \"load15\": %.2f, \"memtotal\": %ld, \"memfree\": %ld}\n", load1min, load5min, load15min, memory_total, memory_free);
    fclose(fptr);
    fclose(fptr2);

    return 0;
}
