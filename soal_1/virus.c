#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024

void replace_string(char *file_path) {
    char temp[] = "temp.txt";
    FILE *fp, *fp_temp, *fp_log;
    char buffer[MAX_BUFFER_SIZE];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fp = fopen(file_path, "r");
    fp_temp = fopen(temp, "w");
    fp_log = fopen("virus.log", "a");

    while ((fgets(buffer, MAX_BUFFER_SIZE, fp)) != NULL) {
        char *malware = strstr(buffer, "m4LwAr3");
        char *spyware = strstr(buffer, "5pYw4R3");
        char *ransomware = strstr(buffer, "R4nS0mWaR3");

        if (malware) {
            strncpy(malware, "[MALWARE]", 9);
            fprintf(fp_temp, "%s", buffer);
            fprintf(fp_log, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, file_path);
        } else if (spyware) {
            strncpy(spyware, "[SPYWARE]", 9);
            fprintf(fp_temp, "%s", buffer);
            fprintf(fp_log, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, file_path);
        } else if (ransomware) {
            strncpy(ransomware, "[RANSOMWARE]", 12);
            fprintf(fp_temp, "%s", buffer);
            fprintf(fp_log, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, file_path);
        } else {
            fprintf(fp_temp, "%s", buffer);
        }
    }

    fclose(fp);
    fclose(fp_temp);
    fclose(fp_log);

    remove(file_path);
    rename(temp, file_path);
}


void search_directory(char *dir_path) {
    DIR *d;
    struct dirent *dir;
    char path[1000];
    struct stat path_stat;

    d = opendir(dir_path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            snprintf(path, sizeof(path), "%s/%s", dir_path, dir->d_name);
            stat(path, &path_stat);
            if (S_ISREG(path_stat.st_mode)) {
                replace_string(path);
            }
        }
        closedir(d);
    }
}


int main(int argc, char *argv[]) {
    pid_t pid, sid;

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(1) {
        search_directory(argv[1]);
        sleep(15);
    }

    exit(EXIT_SUCCESS);
}
