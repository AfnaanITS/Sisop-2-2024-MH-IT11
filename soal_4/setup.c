#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_APPS 10


typedef struct {
    char app_name[100];
    int num_windows;
    pid_t process_ids[10]; 
    int num_processes;
} Application;

Application applications[MAX_APPS];
int num_applications = 0;

void parse_arguments(int argc, char *argv[]) {
    int i;
    for (i = 2; i < argc; i += 2) {
        strcpy(applications[num_applications].app_name, argv[i]);
        applications[num_applications].num_windows = atoi(argv[i + 1]);
        applications[num_applications].num_processes = 0;
        num_applications++;
    }
}

void read_configuration_file(char *filename) {
    FILE *file_pointer = fopen(filename, "r");
    if (file_pointer == NULL) {
        printf("Gagal membuka file konfigurasi\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file_pointer)) {
        char *token = strtok(line, " ");
        if (token != NULL) {
            strcpy(applications[num_applications].app_name, token);
            token = strtok(NULL, " ");
            if (token != NULL) {
                applications[num_applications].num_windows = atoi(token);
                applications[num_applications].num_processes = 0;
                num_applications++;
            }
        }
    }

    fclose(file_pointer);
}

void launch_applications() {
    int i, j;
    for (i = 0; i < num_applications; i++) {
        for (j = 0; j < applications[i].num_windows; j++) {
            pid_t pid = fork();
            if (pid == 0) {
                char *args[] = {applications[i].app_name, NULL};
                execvp(applications[i].app_name, args);
                printf("Gagal membuka %s\n", applications[i].app_name);
                exit(1);
            } else {
                applications[i].process_ids[applications[i].num_processes++] = pid;
            }
        }
    }
}

void terminate_applications(char *filename) {
    int i, j;
    if (filename == NULL) {
        for (i = 0; i < num_applications; i++) {
            for (j = 0; j < applications[i].num_processes; j++) {
                kill(applications[i].process_ids[j], SIGTERM);
            }
        }
    } else {
        for (i = 0; i < num_applications; i++) {
            if (strcmp(applications[i].app_name, filename) == 0) {
                for (j = 0; j < applications[i].num_processes; j++) {
                    kill(applications[i].process_ids[j], SIGTERM);
                }
                break;
            }
        }
    }

    for (i = 0; i < num_applications; i++) {
        for (j = 0; j < applications[i].num_processes; j++) {
            waitpid(applications[i].process_ids[j], NULL, 0);
        }
        applications[i].num_processes = 0;
    }

    num_applications = 0;
}

void signal_handler(int sig) {
    terminate_applications(NULL);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Penggunaan: %s -o <app1> <num1> <app2> <num2>... atau %s -f <file.conf> atau %s -k [<file.conf>]\n", argv[0], argv[0], argv[0]);
        return 1;
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (strcmp(argv[1], "-o") == 0) {
        parse_arguments(argc, argv);
        launch_applications();
    } else if (strcmp(argv[1], "-f") == 0) {
        if (argc < 3) {
            printf("Penggunaan: %s -f <file.conf>\n", argv[0]);
            return 1;
        }
        read_configuration_file(argv[2]);
        launch_applications();
    } else if (strcmp(argv[1], "-k") == 0) {
        if (argc == 2) {
            terminate_applications(NULL);
        } else {
            terminate_applications(argv[2]);
        }
    } else {
        printf("Argumen tidak valid\n");
        return 1;
    }

    return 0;
}
