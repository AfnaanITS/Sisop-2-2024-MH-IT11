#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 1024

char cwd[MAX_BUFFER_SIZE];
char dir_path[MAX_BUFFER_SIZE];

volatile sig_atomic_t mode = 0;

void handle_default(int sig) { mode = 0; }
void handle_backup(int sig) { mode = 1; }
void handle_restore(int sig) { mode = 2; }
void handle_exit(int sig) { exit(EXIT_SUCCESS); }

void default_mode();
void backup();
void restore();

void reset_workspace();
void download_file();
void unzip_file();
char rot19(char c);
void decrypt_filename();
void rename_file();
void delete_file();

void log_event(char *filename, char *action);

int main(int argc, char **argv) 
{
    getcwd(cwd, sizeof(cwd));
    strcpy(dir_path, cwd);

    signal(SIGRTMIN, handle_default);
    signal(SIGUSR1, handle_backup);
    signal(SIGUSR2, handle_restore);
    signal(SIGTERM, handle_exit);

    if (argc == 3 && strcmp(argv[1], "-m") == 0) 
    {
        if (strcmp(argv[2], "backup") == 0) mode = 1;
        else if (strcmp(argv[2], "restore") == 0) mode = 2;
    }

    pid_t pid, sid;

    pid = fork();

    if (pid < 0) 
    {
        printf("Fork Failed!\n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);

    if (setsid() < 0) perror("Error: setsid() failed");
    if ((chdir("/")) < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) 
    {
        switch(mode) 
        {
            case 0:
                default_mode();
                break;

            case 1:
                backup();
                break;

            case 2:
                restore();
                break;
        }
        sleep(100); 
    }
    exit(EXIT_SUCCESS);
}

void default_mode()
{
    reset_workspace();
    download_file();
    unzip_file();
    decrypt_filename();
    rename_file();
    delete_file();
}

void backup()
{
    char path[MAX_BUFFER_SIZE];
    strcpy(path, dir_path);
    strcat(path, "/library/");
    char backup_path[MAX_BUFFER_SIZE];
    strcpy(backup_path, path);
    strcat(backup_path, "backup/");

    DIR *dir = opendir(path);
    struct dirent *entry;

    pid_t pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);

    if (pid == 0)
    {
        struct stat st = {0};
        if (stat(backup_path, &st) == 0) exit(EXIT_SUCCESS);

        char *cmd = "/bin/mkdir";
        char *argv[3] = { "mkdir", backup_path, NULL };
        execv(cmd, argv);
        exit(EXIT_SUCCESS); 
    }

    else
    {
        int status;
        wait(&status);
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        char *filename = entry->d_name;

        if (strstr(filename, "m0V3") == NULL) continue;

        if (filename[0] == '.') continue;

        char file_path[MAX_BUFFER_SIZE];
        strcpy(file_path, path);
        strcat(file_path, filename);
        char file_backup_path[MAX_BUFFER_SIZE];
        strcpy(file_backup_path, backup_path);
        strcat(file_backup_path, filename);

        pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0)
        {
            char *cmd = "/bin/mv";
            char *argv[4] = { "mv", file_path, file_backup_path, NULL };
            execv(cmd, argv);
            log_event(filename, "moved to backup.");
            exit(EXIT_SUCCESS); 
        }

        else
        {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void restore()
{
    char path[MAX_BUFFER_SIZE];
    strcpy(path, dir_path);
    strcat(path, "/library/");
    char backup_path[MAX_BUFFER_SIZE];
    strcpy(backup_path, path);
    strcat(backup_path, "backup/");

    DIR *dir = opendir(backup_path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) 
    {
        char *filename = entry->d_name;

        if (strstr(filename, "m0V3") == NULL) continue;

        if (filename[0] == '.') continue;

        char file_path[MAX_BUFFER_SIZE];
        strcpy(file_path, path);
        strcat(file_path, filename);
        char file_backup_path[MAX_BUFFER_SIZE];
        strcpy(file_backup_path, backup_path);
        strcat(file_backup_path, filename);

        pid_t pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0)
        {
            char *cmd = "/bin/mv";
            char *argv[4] = { "mv", file_backup_path, file_path, NULL };
            execv(cmd, argv);
            log_event(filename, "restored from backup.");
            exit(EXIT_SUCCESS); 
        }

        else
        {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void reset_workspace()
{
    char folder_path[MAX_BUFFER_SIZE];
    strcpy(folder_path, dir_path);
    strcat(folder_path, "/library");
    char zip_path[MAX_BUFFER_SIZE];
    strcpy(zip_path, dir_path);
    strcat(zip_path, "/library.zip");

    pid_t pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);

    else if (pid == 0)
    {
        char *cmd = "/bin/rm";
        char *argv[4] = { "rm", "-rf", folder_path, NULL };
        execv(cmd, argv);
    }

    else
    {
        int status;
        wait(&status);

        pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0)
        {
            char *cmd = "/bin/rm";
            char *argv[3] = { "rm", zip_path, NULL };
            execv(cmd, argv);
        }

        else
        {
            int status;
            wait(&status);
        }
    }
}

void download_file()
{
    pid_t pid = fork();

    char download_path[MAX_BUFFER_SIZE];
    strcpy(download_path, dir_path);

    if (pid < 0) exit(EXIT_FAILURE);

    else if (pid == 0)
    {
        char *cmd = "/bin/wget";
        char *argv[7] = { "wget", "--content-disposition", "--no-check-certificate", "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup", "-P", download_path, NULL };
        execv(cmd, argv);
    }

    else
    {
        int status;
        wait(&status);
    }
}

void unzip_file()
{
    pid_t pid = fork();

    char unzip_path[MAX_BUFFER_SIZE];
    strcpy(unzip_path, dir_path);
    strcat(unzip_path, "/library.zip");

    if (pid < 0) exit(EXIT_FAILURE);

    else if (pid == 0)
    {
        char *cmd = "/bin/unzip";
        char *argv[5] = { "unzip", unzip_path, "-d", dir_path, NULL };
        execv(cmd, argv);
    }

    else
    {
        int status;
        wait(&status);
    }
}

char rot19(char c)
{
    if (isalpha(c))
    {
        char base = islower(c) ? 'a' : 'A';
        return (c - base + 7) % 26 + base;
    } return c;
}

void decrypt_filename()
{
    char path[MAX_BUFFER_SIZE];
    strcpy(path, dir_path);
    strcat(path, "/library/");

    DIR *dir = opendir(path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) 
    {
        char *filename = entry->d_name;
        char new_filename[MAX_BUFFER_SIZE];

        if (isdigit(filename[0])) continue;

        if (filename[0] == '.') continue;

        int nullterminator;
        for (int i = 0; i < strlen(filename); i++)
        {
            new_filename[i] = rot19(filename[i]);
            nullterminator = i + 1;
        } new_filename[nullterminator] = '\0';
        
        char old_path[MAX_BUFFER_SIZE];
        strcpy(old_path, path);
        strcat(old_path, filename);
        char new_path[MAX_BUFFER_SIZE];
        strcpy(new_path, path);
        strcat(new_path, new_filename);

        pid_t pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0)
        {
            char *cmd = "/bin/mv";
            char *argv[4] = { "mv", old_path, new_path, NULL };
            execv(cmd, argv);
            exit(EXIT_SUCCESS); 
        }

        else
        {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void rename_file()
{
    char path[MAX_BUFFER_SIZE];
    strcpy(path, dir_path);
    strcat(path, "/library/");

    DIR *dir = opendir(path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) 
    {
        char *filename = entry->d_name;

        if (strstr(filename, "r3N4mE") == NULL) continue;

        if (filename[0] == '.') continue;

        char old_path[MAX_BUFFER_SIZE];
        strcpy(old_path, path);
        strcat(old_path, filename);
        char new_path[MAX_BUFFER_SIZE];

        if (strstr(filename, ".ts") != NULL)
        {
            strcpy(new_path, path);
            strcat(new_path, "helper.ts");
        }

        else if (strstr(filename, ".py") != NULL)
        {
            strcpy(new_path, path);
            strcat(new_path, "calculator.py");
        }

        else if (strstr(filename, ".go") != NULL)
        {
            strcpy(new_path, path);
            strcat(new_path, "server.go");
        }

        else
        {
            strcpy(new_path, path);
            strcat(new_path, "renamed.file");
        }

        pid_t pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0)
        {
            char *cmd = "/bin/mv";
            char *argv[4] = { "mv", old_path, new_path, NULL };
            execv(cmd, argv);
            log_event(filename, "renamed.");
            exit(EXIT_SUCCESS); 
        }

        else
        {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void delete_file()
{
    char path[MAX_BUFFER_SIZE];
    strcpy(path, dir_path);
    strcat(path, "/library/");

    DIR *dir = opendir(path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) 
    {
        char *filename = entry->d_name;

        if (strstr(filename, "d3Let3") == NULL) continue;

        if (filename[0] == '.') continue;

        char file_path[MAX_BUFFER_SIZE];
        strcpy(file_path, path);
        strcat(file_path, filename);

        pid_t pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0)
        {
            char *cmd = "/bin/rm";
            char *argv[3] = { "rm", file_path, NULL };
            execv(cmd, argv);
            log_event(filename, "deleted.");
            exit(EXIT_SUCCESS); 
        }

        else
        {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void log_event(char *filename, char *action)
{
    // char *username;
    // time_t runtime;
    // struct tm *digitime;
    // username = (char*)malloc(MAX_BUFFER_SIZE);
    // username = getlogin();
    // digitime = localtime(&runtime);

    // char log_file[MAX_BUFFER_SIZE];
    // char log_file_path[MAX_BUFFER_SIZE];
    // strcpy(log_file_path, dir_path);
    // strcat(log_file_path, "/history.log");

    // sprintf(log_file, "[%s][%02d:%02d:%02d] - %s - Successfully %s\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename, action);

    // char *cmd = "/bin/touch";
    // char *argv[3] = { "touch", log_file_path, NULL };
    // execv(cmd, argv);

    // FILE *file = fopen(log_file_path, "a"); 
    // if (file != NULL) {  
    //     fprintf(file, "%s\n", log_file); 
    //     fclose(file); 
    // } else { 
    //     printf("Error opening the file\n"); 
    // } 
}
