#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <time.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/wait.h>

#define LOG_DIR "./logs"
#define LOG_FILE_FORMAT "%s/%s.log"
#define SLEEP_INTERVAL 1

typedef enum {
    MODE_LISTPROSES,
    MODE_MONITOR,
    MODE_KILLPROSES,
    MODE_STOP,
    MODE_INVALID
} Mode;

void list_proses(const char *username);
void write_log(const char *program_name, const char *format, const char *status, ...);
void monitor_daemon(const char *username);
void kill_proses(const char *username);
void handle_signal(int signal);

Mode current_mode = MODE_STOP;

void list_processes(const char *username) {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Error in fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    char *argv[] = {"ps", "-u", (char *)username, NULL};
    execv("/bin/ps", argv);  
    perror("Error in execv");
    exit(EXIT_FAILURE);
  } else { 
    int status;
    waitpid(pid, &status, 0); 
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
    }
  }
}

void write_log(const char *program_name, pid_t pid, const char *format, const char *status, ...) {
    va_list args;
    va_start(args, status);

    time_t current_time;
    time(&current_time);
    struct tm *time_info = localtime(&current_time);

    char log_file[1024];
    char log_entry[1024];

    snprintf(log_file, sizeof(log_file), LOG_FILE_FORMAT, LOG_DIR, getenv("USER"));
    snprintf(log_entry, sizeof(log_entry), "[%02d:%02d:%02d]-[%04d-%02d-%02d]-[PID: %d]-[%s]",
             time_info->tm_hour, time_info->tm_min, time_info->tm_sec,
             time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, pid, program_name);

    vsnprintf(log_entry + strlen(log_entry), sizeof(log_entry) - strlen(log_entry), format, args);

    snprintf(log_entry + strlen(log_entry), sizeof(log_entry) - strlen(log_entry), "-%s", status);

    pid_t fork_pid = fork();

    if (fork_pid == -1) {
        perror("fork");
        return;
    }

    if (fork_pid == 0) {
        struct stat st;
        if (stat(LOG_DIR, &st) == -1) {
            if (mkdir(LOG_DIR, 0755) == -1) {
                perror("mkdir");
                exit(EXIT_FAILURE);
            }
        } else if (!S_ISDIR(st.st_mode)) {
            fprintf(stderr, "%s bukan direktori.\n", LOG_DIR);
            exit(EXIT_FAILURE);
        }

        FILE *fp = fopen(log_file, "a");
        if (fp) {
            fprintf(fp, "%s\n", log_entry);
            fclose(fp);
            exit(EXIT_SUCCESS);
        } else {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(fork_pid, &status, 0);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS) {
            printf("Gagal menulis log.\n");
        }
    }

    va_end(args);
}

void monitor_daemon(const char *username) {
    signal(SIGALRM, handle_signal);
    signal(SIGINT, handle_signal);

    while (current_mode == MODE_MONITOR) {
        list_proses(username);
        write_log(getpid(), "Memantau proses pengguna %s", "JALAN", username);

        sleep(SLEEP_INTERVAL);
        alarm(SLEEP_INTERVAL);
    }
}

void kill_proses(const char *username) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) { // Child process
        execlp("pkill", "pkill", "-KILL", "-u", username, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 0) {
                write_log(getpid(), "Proses pengguna %s digagalkan", "JALAN", username);
            } else {
                write_log(getpid(), "Gagal menggagalkan proses pengguna %s", "GAGAL", username);
            }
        }
    }
}

void handle_signal(int signal) {
    if (signal == SIGALRM && current_mode == MODE_KILLPROSES) {
        kill_proses(getenv("USER"));
    } else if (signal == SIGINT) {
        if (current_mode == MODE_MONITOR) {
            current_mode = MODE_STOP;
            write_log(getpid(), "Memantau proses pengguna dihentikan", "GAGAL");
        } else {
            exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Penggunaan: admin [opsi] <username>\n");
        printf("Opsi:\n");
        printf("-l: Menampilkan daftar proses user\n");
        printf("-m: Memantau proses user\n");
        printf("-s: Menghentikan pemantauan user\n");
        printf("-c: Menggagalkan proses user\n");
        printf("-a: Mengembalikan akses user\n");
        return 1;
    }

    const char *username = argv[argc - 1];

    Mode mode = MODE_INVALID;
    if (strcmp(argv[1], "-l") == 0) {
        mode = MODE_LISTPROSES;
    } else if (strcmp(argv[1], "-m") == 0) {
        mode = MODE_MONITOR;
    } else if (strcmp(argv[1], "-c") == 0) {
        mode = MODE_KILLPROSES;
    } else if (strcmp(argv[1], "-s") == 0) {
        mode = MODE_STOP;
    } else {
        printf("Opsi tidak valid.\n");
        return 1;
    }

    switch (mode) {
        case MODE_LISTPROSES:
            list_proses(username);
            break;
        case MODE_MONITOR:
            current_mode = MODE_MONITOR;
            monitor_daemon(username);
            break;
        case MODE_KILLPROSES:
            current_mode = MODE_KILLPROSES;
            kill_proses(username);
            break;
        case MODE_STOP:
            if (strcmp(argv[1], "-s") == 0) {
                current_mode = MODE_STOP;
                write_log(getpid(), "Memantau proses pengguna dihentikan", "GAGAL");
            } else if (strcmp(argv[1], "-a") == 0) {
                current_mode = MODE_MONITOR;
                write_log(getpid(), "Memantau proses pengguna kembali dijalankan", "JALAN");
            }
            break;
        default:
            printf("Mode tidak valid.\n");
            return 1;
    }

    return 0;
}
