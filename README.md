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
