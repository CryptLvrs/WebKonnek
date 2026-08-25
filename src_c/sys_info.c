#include <stdio.h>
#include <stdlib.h>

int main() {

    // 'loadavg' part
    FILE *fptr = fopen("/proc/loadavg", "r");
    if (fptr == 0) {
        printf("Error : Unable to open file called 'loadavg'.\n");
        return 1;
    }

    char fline[128];
    if (fgets(fline, sizeof(fline), fptr) == NULL) {
        printf("Error : Failed to read from file.\n");
        fclose(fptr);
        return 1;
    }

    float load1min, load5min, load15min;
    if (sscanf(fline, "%f %f %f", &load1min, &load5min, &load15min) != 3) {
        printf("Error : Error: Failed to parse load averages from 'loadavg'.\n");
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

    char memline[256];
    long memory_total = -1;
    long memory_free = -1;
    int found_count = 0;

    while (fgets(memline, sizeof(memline), fptr2)) {
        if (sscanf(memline, "MemTotal: %ld kB", &memory_total) == 1) {
            found_count++;
        }
        if (sscanf(memline, "MemFree: %ld kB", &memory_free) == 1) {
            found_count++;
        }
        if (found_count == 2) {
            break;
        }
    }

    FILE *fptr3 = fopen("/proc/uptime", "r");
    if (fptr3 == NULL) {
        printf("Error : Unable to open file called 'uptime'.\n");
        fclose(fptr);
        fclose(fptr2);
        return 1;
    }

    char uptimeline[64];
    double uptime_seconds = -1;
    if (fgets(uptimeline, sizeof(uptimeline), fptr3) == NULL || sscanf(uptimeline, "%lf", &uptime_seconds) != 1) {
        printf("Error : Failed to parse uptime from 'uptime'.\n");
        fclose(fptr);
        fclose(fptr2);
        fclose(fptr3);
        return 1;
    }
    fclose(fptr3);

    // Temp output ('JSON' "format") 
    printf("{\"load1\": %.2f, \"load5\": %.2f, \"load15\": %.2f, \"memtotal\": %ld, \"memfree\": %ld, \"uptime\": %.0f}\n", load1min, load5min, load15min, memory_total, memory_free, uptime_seconds);
    fclose(fptr);
    fclose(fptr2);

    return 0;
}
