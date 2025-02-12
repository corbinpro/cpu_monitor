/*
 ============================================================================
 Name        : CPU_monitor.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROC_STAT "/proc/stat"

void clear_screen(){
	usleep(16667);
	system("clear");
}


void get_cpu_times(unsigned long long *idle, unsigned long long *total) {
    FILE *fp = fopen(PROC_STAT, "r");
    if (!fp) {
        perror("Error opening /proc/stat");
        exit(1);
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), fp); // Read first line
    fclose(fp);

    // CPU time variables
    unsigned long long user, nice, system, idle_time, iowait, irq, softirq, steal;

    sscanf(buffer, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle_time, &iowait, &irq, &softirq, &steal);

    *idle = idle_time + iowait;  // Total idle time
    *total = user + nice + system + idle_time + iowait + irq + softirq + steal; // Total time
}

int main(void) {
    unsigned long long idle1, total1, idle2, total2;
    setenv("TERM", "xterm", 1);

    while (1){

        get_cpu_times(&idle1, &total1);
        clear_screen();
        get_cpu_times(&idle2, &total2);

        // Calculate CPU usage percentage
        double cpu_usage = (1.0 - (double)(idle2 - idle1) / (total2 - total1)) * 100.0;

        int number_bars = (int)cpu_usage / 5;
        if (number_bars < 1){
        	number_bars = 1;
        }
        int number_spaces = 20 - number_bars;

        printf("CPU: ");
        printf("0%%- ");

        for (int i = 0; i < number_bars;i++){
        	printf("# ");
        }
        for (int i = 0; i < number_spaces;i++){
        	printf(". ");
        }

        printf(" -100%%");
        printf("\n");

    }

    return 0;
}
