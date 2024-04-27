# Sisop-2-2024-MH-IT11
# Langkah-langkah penyelesaian setiap soal
# SOAL 1
Dikerjakan oleh Rafi' Afnaan Fathurrahman (5027231040)
Pengerjaan soal ini menggunakan 1 file .c
## virus.c
## Subsoal a
Program harus dapat menerima input path berupa `argv[]` untuk mengatur folder dimana file akan dieksekusi
```c
int main(int argc, char *argv[]) 
// ...
search_directory(argv[1]);
```

## Subsoal b
Program tersebut berfungsi untuk mereplace string dengan ketentuan sebagai berikut:
- String `m4LwAr3` direplace dengan string `[MALWARE]`
- String `5pYw4R3` direplace dengan string `[SPYWARE]`
- String `R4nS0mWaR3` direplace dengan string `[RANSOMWARE]`
```c
void replace_string(char *file_path) 
{
    char temp[] = "temp.txt";
    FILE *fp, *fp_temp, *fp_log;
    char buffer[MAX_BUFFER_SIZE];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fp = fopen(file_path, "r");
    fp_temp = fopen(temp, "w");
    fp_log = fopen("virus.log", "a");

    while ((fgets(buffer, MAX_BUFFER_SIZE, fp)) != NULL) 
    {
        char *malware = strstr(buffer, "m4LwAr3");
        char *spyware = strstr(buffer, "5pYw4R3");
        char *ransomware = strstr(buffer, "R4nS0mWaR3");

        if (malware) 
        {
            strncpy(malware, "[MALWARE]", 9);
            fprintf(fp_temp, "%s", buffer);
            fprintf(fp_log, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, file_path);
            sleep(1);
        } 
        
        else if (spyware) 
        {
            strncpy(spyware, "[SPYWARE]", 9);
            fprintf(fp_temp, "%s", buffer);
            fprintf(fp_log, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, file_path);
            sleep(1);
        } 
        
        else if (ransomware) 
        {
            strncpy(ransomware, "[RANSOMWARE]", 12);
            fprintf(fp_temp, "%s", buffer);
            fprintf(fp_log, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, file_path);
            sleep(1);
        } 
        
        else 
        {
            fprintf(fp_temp, "%s", buffer);
        }
    }

    fclose(fp);
    fclose(fp_temp);
    fclose(fp_log);

    remove(file_path);
    rename(temp, file_path);
}
```
## Subsoal c
Program harus berjalan secara daemon, dan tidak diperbolehkan menggunakan command `system()` dalam pembuatan program
```c
// ...
pid_t pid, sid;

pid = fork();

if (pid < 0) exit(EXIT_FAILURE);

if (pid > 0) exit(EXIT_SUCCESS);

umask(0);

sid = setsid();
if (sid < 0) exit(EXIT_FAILURE);

close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
```

## Subsoal d
Program akan secara terus menerus berjalan di background dengan jeda 15 detik
```c
sleep(15);
```

## Subsoal e
Catat setiap penghapusan string yang dilakukan oleh program pada sebuah file bernama virus.log dengan format: [dd-mm-YYYY][HH:MM:SS] Suspicious string at <file_name> successfully replaced!
```c
fprintf(fp_log, "[%02d-%02d-%04d][%02d:%02d:%02d] Suspicious string at %s successfully replaced!\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, file_path);
```

## dokumentasi
![Screenshot_2024-04-27_22_05_54](https://github.com/AfnaanITS/Sisop-2-2024-MH-IT11/assets/143690594/948b2294-f174-4740-88a9-1a4f7e3e5c81)
![Screenshot_2024-04-27_22_07_37](https://github.com/AfnaanITS/Sisop-2-2024-MH-IT11/assets/143690594/984ead27-f4fd-4885-b853-b1e197100ccb)


# SOAL 2
Dikerjakan oleh Rafi' Afnaan Fathurrahman (5027231040)
Pengerjaan soal ini menggunakan 1 file .c
MENGGUNAKAN KODE YANG TELAH DI REVISI:
- Memperbaiki pembuatan file `history.log`
## management.c
## Subsoal a
Atasannya meminta agar program tersebut dapat berjalan secara `daemon` dan dapat mengunduh serta melakukan unzip terhadap file berikut. Atasannya juga meminta program ini dibuat tanpa menggunakan command `system()`
```c
pid_t pid, sid;
pid = fork();
if (pid < 0) exit(EXIT_FAILURE);
if (pid > 0) exit(EXIT_SUCCESS);
umask(0);
if (setsid() < 0) perror("Error: setsid() failed");
if ((chdir("/")) < 0) exit(EXIT_FAILURE);
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
// ...
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
```
## Subsoal b
Setelah ditelusuri, ternyata hanya 6 file teratas yang nama filenya tidak dienkripsi. Oleh karena itu, bantulah Paul untuk melakukan dekripsi terhadap nama file ke-7 hingga terakhir menggunakan algoritma ROT19
```c
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
```

## Subsoal c
Setelah dekripsi selesai, akan terlihat bahwa setiap file memuat salah satu dari kode berikut: `r3N4mE`, `d3Let3`, dan `m0V3`. Untuk setiap file dengan nama yang memuat kode `d3Let3`, hapus file tersebut. Sementara itu, untuk setiap file dengan nama yang memuat kode `r3N4mE`, lakukan hal berikut:
- Jika ekstensi file adalah “`.ts`”, rename filenya menjadi “`helper.ts`”
- Jika ekstensi file adalah “`.py`”, rename filenya menjadi “`calculator.py`”
- Jika ekstensi file adalah “`.go`”, rename filenya menjadi “`server.go`”
- Jika file tidak memuat salah satu dari ekstensi diatas, rename filenya menjadi “`renamed.file`”
```c
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
        FILE *file = fopen(log_file_path, "a"); 
        if (file != NULL) 
        {  
            fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully renamed.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
            fclose(file); 
        }
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
        FILE *file = fopen(log_file_path, "a"); 
        if (file != NULL) 
        {  
            fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully deleted.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
            fclose(file); 
        }
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
            exit(EXIT_SUCCESS); 
        }
        else
        {
            int status;
            wait(&status);
        }
    } closedir(dir);
}
```

## Subsoal d
Atasan Paul juga meminta agar program ini dapat membackup dan merestore file. Oleh karena itu, bantulah Paul untuk membuat program ini menjadi 3 mode, yaitu:
- default: program berjalan seperti biasa untuk me-rename dan menghapus file. Mode ini dieksekusi ketika program dijalankan tanpa argumen tambahan, yaitu dengan command `./management` saja
- backup: program memindahkan file dengan kode `m0V3` ke sebuah folder bernama “backup”
- restore: program mengembalikan file dengan kode `m0V3` ke folder sebelum file tersebut dipindahkan
- Contoh penggunaan: `./management -m backup`
```c
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
        FILE *file = fopen(log_file_path, "a"); 
        if (file != NULL) 
        {  
            fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully moved to backup.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
            fclose(file); 
        }
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
        FILE *file = fopen(log_file_path, "a"); 
        if (file != NULL) 
        {  
            fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully restored from backup.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
            fclose(file); 
        }
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
            exit(EXIT_SUCCESS); 
        }
        else
        {
            int status;
            wait(&status);
        }
    } closedir(dir);
}
```

## Subsoal e
Terkadang, Paul perlu mengganti mode dari program ini tanpa menghentikannya terlebih dahulu. Oleh karena itu, bantulan Paul untuk mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan:
- `SIGRTMIN` untuk mode default
- `SIGUSR1` untuk mode backup
- `SIGUSR2` untuk mode restore
- Contoh penggunaan: `kill -SIGUSR2 <pid_program>`
```c
volatile sig_atomic_t mode = 0;
void handle_default(int sig) { mode = 0; }
void handle_backup(int sig) { mode = 1; }
void handle_restore(int sig) { mode = 2; }
// ...
signal(SIGRTMIN, handle_default);
signal(SIGUSR1, handle_backup);
signal(SIGUSR2, handle_restore);
```

## Subsoal f
Program yang telah dibuat ini tidak mungkin akan dijalankan secara terus-menerus karena akan membebani sistem. Maka dari itu, bantulah Paul untuk membuat program ini dapat dimatikan dengan aman dan efisien
```c
void handle_exit(int sig) { exit(EXIT_SUCCESS); }
// ...
signal(SIGTERM, handle_exit);
```

## Subsoal g
Terakhir, program ini harus berjalan setiap detik dan mampu mencatat setiap peristiwa yang terjadi ke dalam file .log yang bernama “history.log” dengan ketentuan:
- Format: `[nama_user][HH:MM:SS] - <nama_file> - <action>`
- `nama_user` adalah username yang melakukan action terhadap file
- Format `action` untuk setiap kode:
- kode `r3N4mE`: `Successfully renamed.`
- kode `d3Let3`: `Successfully deleted.`
- mode backup: `Successfully moved to backup.`
- mode restore: `Successfully restored from backup.`
- Contoh pesan log:
    1. `[paul][00:00:00] - r3N4mE.ts - Successfully renamed.`
    2. `[paul][00:00:00] - m0V3.xk1 - Successfully restored from backup.`
```c
FILE *file = fopen(log_file_path, "a"); 
if (file != NULL) 
{  
    fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully moved to backup.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
    fclose(file); 
} // contoh untuk backup
```

## Subsoal h
Berikut adalah struktur folder untuk pengerjaan nomor 2:
    soal_2/
    ├── history.log
    ├── management.c
    └── library/
        └── backup/

## dokumentasi
![Screenshot_2024-04-27_22_14_02](https://github.com/AfnaanITS/Sisop-2-2024-MH-IT11/assets/143690594/a06f313f-9c7d-47d7-bc34-837939ce3d46)


# Soal 3
Dikerjakan oleh Ricko Mianto Jaya Saputra (5027231031)

Kode yang disajikan adalah implementasi dari sebuah sistem untuk memonitor aktivitas pengguna dalam bahasa C. Skrip utama yang disertakan adalah:

- `admin.c` : sebuah file dalam bahasa C yang berguna untuk memonitor aktivitas pengguna
- `rickomianto.log` : sebuah file user.log yang berguna sebagai tempat mencatat aktivitas pengguna yang di monitor

Dalam soal ini, kita diminta untuk membuat sebuah program yang berfungsi untuk memantau aktivitas pengguna, menggagalkan proses pengguna, dan mengembalikan akses dari proses yang digagalkan.

## admin.c

Berikut adalah isi dari skrip saya:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#define LOG_DIR "./logs/"

pid_t monitor_pid = -1;

void log_activity(char *user, char *process_name, int pid, int status) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char log_file[100];
    sprintf(log_file, "%s%s.log", LOG_DIR, user);

    char timestamp[100];
    sprintf(timestamp, "[%02d:%02d:%02d]-[%02d:%02d:%02d]", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    char status_str[6];
    if (status == 0) {
        strcpy(status_str, "JALAN");
    } else {
        strcpy(status_str, "GAGAL");
    }

    FILE *file = fopen(log_file, "a");
    if (file != NULL) {
        fprintf(file, "%s_%d-%s_%s\n", timestamp, pid, process_name, status_str);
        fclose(file);
    }
}

void monitor_user(char *user) {
    monitor_pid = getpid();
    while (1) {
        char command[50];
        sprintf(command, "ps -u %s -o pid,cmd", user);
        FILE *process = popen(command, "r");
        if (process == NULL) {
            perror("popen failed");
            exit(EXIT_FAILURE);
        }

        char buffer[256];
        char process_name[256];
        int pid;
        while (fgets(buffer, sizeof(buffer), process) != NULL) {
            if (sscanf(buffer, "%d %[^\n]", &pid, process_name) == 2) {
                log_activity(user, process_name, pid, 0);
            }
        }

        int status = pclose(process);
        if (status == -1) {
            perror("pclose failed");
            exit(EXIT_FAILURE);
        } else if (WEXITSTATUS(status) != 0) {
            printf("`ps` command exited with status %d\n", WEXITSTATUS(status));
        }

        sleep(10);
    }
}

void control_processes(char *user, int action) {
    FILE *process;
    char command[100];

    if (action == 1) {
        if (monitor_pid != -1) {
            kill(monitor_pid, SIGKILL);
            monitor_pid = -1;
        }
    } else if (action == 0) {
        sprintf(command, "pkill -u %s", user);
        log_activity(user, "control_processes", getpid(), 1);
    } else if (action == 2) {
        sprintf(command, "pkill -u %s -CONT", user);
        log_activity(user, "control_processes", getpid(), 0);
    }

    process = popen(command, "r");
    if (process == NULL) {
        perror("Error executing command");
        exit(EXIT_FAILURE);
    }
    pclose(process);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <option> <user>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-m") == 0) {
        monitor_user(argv[2]);
    } else if (strcmp(argv[1], "-s") == 0) {
        control_processes(argv[2], 1);
    } else if (strcmp(argv[1], "-c") == 0) {
        control_processes(argv[2], 1);
    } else if (strcmp(argv[1], "-a") == 0) {
        control_processes(argv[2], 0);
    } else {
        printf("Invalid option\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

## Penjelasan Struktur Kode

1. Include Dependencies: Program ini menggunakan beberapa library standar C seperti stdio.h, stdlib.h, string.h, time.h, unistd.h, fcntl.h, sys/types.h, sys/stat.h, sys/wait.h, dan signal.h.
2. Define Constants: Program mendefinisikan konstanta LOG_DIR yang menyimpan lokasi direktori untuk file log.
3. Global Variables: Program memiliki variabel global monitor_pid yang menyimpan PID dari proses monitor pengguna.
4. Function Definitions: log_activity(char *user, char *process_name, int pid, int status): Fungsi ini digunakan untuk mencatat aktivitas proses ke dalam file log dengan format tertentu.
6. monitor_user(char *user): Fungsi utama untuk memantau proses pengguna. Ini membuat proses baru yang secara terus-menerus memeriksa proses yang berjalan untuk pengguna yang ditentukan dan mencatat aktivitasnya ke dalam file log.
7. control_processes(char *user, int action): Fungsi ini digunakan untuk mengendalikan proses pengguna dengan tindakan tertentu (menghentikan, melanjutkan, atau membunuh).
8. Main Function: Fungsi main adalah titik masuk program. Ini mengambil argumen baris perintah dan memanggil fungsi yang sesuai berdasarkan opsi yang diberikan (-m, -s, -c, atau -a).

Secara keseluruhan, program ini menggunakan beberapa konsep penting dalam pemrograman C, seperti:
Penggunaan file dan direktori
Manipulasi string
Fungsi waktu
Pemrosesan baris perintah
Manajemen proses (fork, exec, kill, waitpid)
Pembuatan log
Penanganan sinyal
Program ini dirancang untuk dijalankan dari baris perintah, dan outputnya akan berupa entri log dalam file log yang sesuai. Fungsi utamanya adalah memantau dan mengendalikan proses untuk pengguna tertentu, serta mencatat aktivitas proses tersebut ke dalam file log.

### Hasil Run

![WhatsApp Image 2024-04-27 at 18 21 06](https://github.com/AfnaanITS/Sisop-2-2024-MH-IT11/assets/143690594/cb1e99e7-153f-4cc0-a454-0266ee1cb5d0)
![WhatsApp Image 2024-04-27 at 18 21 07](https://github.com/AfnaanITS/Sisop-2-2024-MH-IT11/assets/143690594/412ed9d9-2a23-46fd-b68d-4863b13cbbda)
![WhatsApp Image 2024-04-27 at 18 21 07(1)](https://github.com/AfnaanITS/Sisop-2-2024-MH-IT11/assets/143690594/d0b5e030-f817-4975-ab7a-ca1a4e88794e)


# Kendala yang dialami
pada fitur ./admin -c user malah membuat vm saya tershutdown, yang mana seharusnya program hanya mengkill prosesnya saja bukan mengkill user. Lalu pada fitur ./admin -s user tidak dapat menghentikan monitor user yang sedang berjalan
# SOAL 4
Dikerjakan oleh Raditya Hardian Santoso (5027231033)


Kode yang disajikan adalah implementasi dari sebuah sistem manajemen aplikasi sederhana dalam bahasa C. Skrip utama yang disertakan adalah:


- `setup.c`: Sebuah file dalam bahasa pemrograman C yang digunakan untuk menginisialisasi dan menyiapkan lingkungan atau konfigurasi awal sebelum program utama dijalankan
- `file.conf`: Sebuah file config yang berisi config yang nantinya bisa di-run oleh hasil compile setup.c


Dalam soal ini, kita diminta untuk membuat program yang memungkinkan pengguna untuk meluncurkan aplikasi dengan jumlah jendela yang ditentukan dan memberikan kemampuan untuk menghentikan aplikasi yang sedang berjalan.


## setup.c


Isi skrip saya :


```bash
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

```
## Penjelasan Struktur Kode 

1. Header File dan Deklarasi Awal
  Kode dimulai dengan inklusi beberapa header file standar C yang diperlukan untuk operasi sistem (stdio, stdlib, string, unistd, sys/wait, signal).
  Konstanta MAX_APPS didefinisikan untuk menetapkan jumlah maksimum aplikasi yang dapat dikelola.
2. Definisi Struktur Data
  Application: Struktur data yang merepresentasikan sebuah aplikasi.
    app_name: Nama aplikasi (string).
    num_windows: Jumlah jendela aplikasi yang akan diluncurkan.
    process_ids: Array pid_t yang menyimpan ID proses dari aplikasi yang diluncurkan.
    num_processes: Jumlah proses yang sedang berjalan untuk aplikasi tersebut.
3. Fungsi parse_arguments
  Fungsi ini digunakan untuk membaca dan menguraikan argumen baris perintah yang diberikan kepada program.
  Argumen baris perintah dipecah berdasarkan pola tertentu untuk menentukan aplikasi mana yang harus       diluncurkan dan dengan berapa banyak jendela.
  Informasi ini disimpan dalam array applications.
4. Fungsi read_configuration_file
  Fungsi ini digunakan untuk membaca konfigurasi aplikasi dari sebuah file.
  Setiap baris dalam file konfigurasi berisi nama aplikasi dan jumlah jendela yang harus diluncurkan.
  Informasi ini juga disimpan dalam array applications.
5. Fungsi launch_applications
  Fungsi ini mengelola peluncuran aplikasi berdasarkan informasi yang telah diperoleh dari argumen baris   perintah atau file konfigurasi.
  Setiap aplikasi diluncurkan dalam loop menggunakan fork().
  Proses anak (child process) akan mengeksekusi aplikasi dengan bantuan execvp.
  PID dari setiap proses aplikasi disimpan untuk pemantauan dan penghentian nantinya.
6. Fungsi terminate_applications
  Fungsi ini digunakan untuk menghentikan aplikasi-aplikasi yang sedang berjalan.
  Dapat dijalankan untuk menghentikan semua aplikasi atau hanya aplikasi tertentu berdasarkan nama.
7. Fungsi signal_handler dan main
   signal_handler: Fungsi penangan sinyal yang akan memastikan bahwa aplikasi dapat dihentikan dengan aman saat menerima sinyal seperti SIGINT (CTRL+C) atau SIGTERM.
   main: Fungsi utama yang menangani logika dasar program.
      Memeriksa argumen baris perintah untuk menentukan tindakan yang diperlukan (peluncuran aplikasi, baca dari file konfigurasi, atau penghentian aplikasi).
      Mengatur penangan sinyal dan memanggil fungsi-fungsi yang sesuai berdasarkan argumen yang diberikan.

## file.conf

Isi skrip saya :


```bash
firefox 2
wireshark 3
```


Kegunaan kedua kode tadi : 


Kode ini berguna untuk mengelola peluncuran dan penghentian aplikasi secara otomatis berdasarkan konfigurasi yang diberikan. Pengguna dapat menjalankan aplikasi dengan jumlah jendela yang diinginkan atau membaca konfigurasi dari file eksternal untuk mengotomatiskan proses peluncuran aplikasi.


### hasil run:

Test Case 1 (BERHASIL) :

ketika saya input ini
![sisop1](https://github.com/rdthrdn/Sisop-Modul-2/assets/137570361/d35dcc5f-7fff-42cf-9d2c-0b55ba2389ee)
hasilnya seperti ini
![sisop2](https://github.com/rdthrdn/Sisop-Modul-2/assets/137570361/6a8a2991-5bb4-4e40-90d4-8fa473f78f2c)

Test Case 2 (BERHASIL) :

ketika saya input ini
![sisop3](https://github.com/rdthrdn/Sisop-Modul-2/assets/137570361/17225118-42b1-46b0-a9a1-3c022c64e6a0)
hasilnya seperti ini
![sisop4](https://github.com/rdthrdn/Sisop-Modul-2/assets/137570361/61baa3f3-8297-4659-8b68-3c3b05ecff00)

Test Case 3 (MASIH GAGAL) :

ketika saya input ini
![sisop5](https://github.com/rdthrdn/Sisop-Modul-2/assets/137570361/4e4ebe73-8043-470c-a10b-4852302f88e8)
hasilnya seperti ini
![sisop6](https://github.com/rdthrdn/Sisop-Modul-2/assets/137570361/5a1d526d-f3f6-4dba-a8df-b7526138c44e)

# Kendala yang dialami 
Kendala yang saya alami selama pengerjaan soal nomor 4 ini adalah hasil dari eksekusi '-k [<file.conf>]' untuk menghentikan aplikasi berdasarkan file konfigurasi belum berhasil. Yang saya tangkap adalah karena aplikasi yang misal saya taruh di config atau yang di run menggunakan perintah -o tidak menerima sinyal SIGTERM dan SIGKILL yang telah saya buat fungsi di setup.c
