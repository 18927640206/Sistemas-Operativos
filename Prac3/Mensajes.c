/*
 Sistemas Operativos
 Practica de Servicio de Mensajes
 Nombre del alumno: Gonzalez Padron Joahan Andres
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define STORAGE_ID "/conversacion"
#define MAX_MSG 10
#define MSG_SIZE 128

typedef struct {
    int total;
    int inicio;
    int fin;
    char mensajes[MAX_MSG][MSG_SIZE];
} Conversacion;

int main(int argc, char *argv[])
{
    int fd;
    int res;
    void *addr;
    Conversacion *conv;

    if (argc < 2) {
        printf("Uso:\n");
        printf("./Mensajes crear\n");
        printf("./Mensajes borrar\n");
        printf("./Mensajes escribir \"mensaje\"\n");
        printf("./Mensajes leer\n");
        printf("./Mensajes estado\n");
        return 1;
    }

    /* ================= CREAR ================= */
    if (strcmp(argv[1], "crear") == 0) {

        fd = shm_open(STORAGE_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1) { perror("shm_open"); return 10; }

        res = ftruncate(fd, sizeof(Conversacion));
        if (res == -1) { perror("ftruncate"); return 20; }

        addr = mmap(NULL, sizeof(Conversacion),
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED) { perror("mmap"); return 30; }

        conv = (Conversacion *)addr;

        flock(fd, LOCK_EX);

        conv->total = 0;
        conv->inicio = 0;
        conv->fin = 0;

        flock(fd, LOCK_UN);

        printf("Conversacion creada.\n");

        return 0;
    }

    /* ================= BORRAR ================= */
    if (strcmp(argv[1], "borrar") == 0) {

        if (shm_unlink(STORAGE_ID) == -1) {
            perror("shm_unlink");
            return 40;
        }

        printf("Conversacion borrada.\n");
        return 0;
    }

    /* Para las demás operaciones se abre existente */
    fd = shm_open(STORAGE_ID, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) { perror("shm_open"); return 50; }

    addr = mmap(NULL, sizeof(Conversacion),
                PROT_READ | PROT_WRITE,
                MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) { perror("mmap"); return 60; }

    conv = (Conversacion *)addr;

    /* ================= ESCRIBIR ================= */
    if (strcmp(argv[1], "escribir") == 0) {

        if (argc < 3) {
            printf("Debe escribir un mensaje.\n");
            return 1;
        }

        flock(fd, LOCK_EX);

        if (conv->total == MAX_MSG) {
            printf("Memoria llena. No se puede escribir.\n");
            flock(fd, LOCK_UN);
            return 0;
        }

        strncpy(conv->mensajes[conv->fin], argv[2], MSG_SIZE);
        conv->fin = (conv->fin + 1) % MAX_MSG;
        conv->total++;

        printf("Mensaje escrito.\n");
        printf("Mensajes actuales: %d\n", conv->total);

        flock(fd, LOCK_UN);
        return 0;
    }

    /* ================= LEER ================= */
    if (strcmp(argv[1], "leer") == 0) {

        flock(fd, LOCK_EX);

        if (conv->total == 0) {
            printf("Memoria vacia. No hay mensajes.\n");
            flock(fd, LOCK_UN);
            return 0;
        }

        printf("Mensaje leido: %s\n", conv->mensajes[conv->inicio]);

        conv->inicio = (conv->inicio + 1) % MAX_MSG;
        conv->total--;

        printf("Mensajes restantes: %d\n", conv->total);

        flock(fd, LOCK_UN);
        return 0;
    }

    /* ================= ESTADO ================= */
    if (strcmp(argv[1], "estado") == 0) {

        flock(fd, LOCK_SH);
        printf("Mensajes actuales: %d\n", conv->total);
        flock(fd, LOCK_UN);

        return 0;
    }

    printf("Comando no reconocido.\n");
    return 0;
}