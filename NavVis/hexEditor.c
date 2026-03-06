#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <curses.h>
#include <sys/stat.h>
#include <sys/mman.h>

/* Variables globales */
int fd;
int offset = 0;
long fileSize;

char *hazLinea(char *base, int dir) {

    char linea[100];
    int o = 0;

    o += sprintf(linea,"%08x ",dir);

    for(int i=0;i<4;i++) {

        unsigned char a,b,c,d;

        a = base[dir+4*i+0];
        b = base[dir+4*i+1];
        c = base[dir+4*i+2];
        d = base[dir+4*i+3];

        o += sprintf(&linea[o],"%02x %02x %02x %02x ",a,b,c,d);
    }

    for(int i=0;i<16;i++) {

        if(isprint(base[dir+i]))
            o += sprintf(&linea[o],"%c",base[dir+i]);
        else
            o += sprintf(&linea[o],".");

    }

    sprintf(&linea[o],"\n");

    return strdup(linea);
}

char *mapFile(char *filePath) {

    fd = open(filePath,O_RDONLY);

    if(fd==-1) {
        perror("Error abriendo archivo");
        return NULL;
    }

    struct stat st;
    fstat(fd,&st);

    fileSize = st.st_size;

    char *map = mmap(0,fileSize,PROT_READ,MAP_SHARED,fd,0);

    if(map==MAP_FAILED) {

        close(fd);
        perror("Error mmap");
        return NULL;
    }

    return map;
}

void dibujar(char *map) {

    clear();

    for(int i=0;i<25;i++) {

        int dir = offset + i*16;

        if(dir >= fileSize)
            break;

        char *l = hazLinea(map,dir);

        move(i,0);
        addstr(l);

        free(l);
    }

    move(26,0);
    printw("Offset: %d | Flechas mover | PgUp/PgDn | g ir a offset | i inicio | f final | Ctrl+X salir", offset);
    refresh();
}

int visor(char *filename) {

    char *map = mapFile(filename);

    if(map==NULL)
        exit(EXIT_FAILURE);

    int c;

    int x=0,y=0;

    dibujar(map);

    c = getch();

    while(c != 24) {

        switch(c) {

            case KEY_DOWN:
                if(offset + 16 < fileSize)
                    offset += 16;
                break;

            case KEY_UP:
                if(offset - 16 >= 0)
                    offset -= 16;
                break;

            case KEY_NPAGE:
                offset += 25*16;
                if(offset > fileSize)
                    offset = fileSize - 16;
                break;

            case KEY_PPAGE:
                offset -= 25*16;
                if(offset < 0)
                    offset = 0;
                break;

            case 'i':
                offset = 0;
                break;

            case 'f':
                offset = fileSize - (25*16);
                if(offset < 0)
                    offset = 0;
                break;

            case 'g': {

                char lin[20];

                echo();

                move(27,0);
                clrtoeol();
                addstr("Ir a offset: ");
                getstr(lin);

                offset = atoi(lin);

                if(offset < 0)
                    offset = 0;

                if(offset > fileSize)
                    offset = fileSize-16;

                noecho();
                break;
            }
        }

        dibujar(map);

        c = getch();
    }

    munmap(map,fileSize);
    close(fd);

    return 0;
}

int main(int argc,char *argv[]) {

    initscr();
    raw();
    keypad(stdscr,TRUE);
    noecho();

    if(argc!=2) {

        endwin();
        printf("Uso: %s archivo\n",argv[0]);
        return -1;
    }

    visor(argv[1]);

    endwin();

    return 0;
}