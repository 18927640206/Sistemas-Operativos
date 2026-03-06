#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int fd;

char *mapFile(char *filePath) {

    fd = open(filePath,O_RDONLY);

    if(fd==-1){
        perror("Error abriendo archivo");
        return NULL;
    }

    struct stat st;
    fstat(fd,&st);

    int fs = st.st_size;

    char *map = mmap(0,fs,PROT_READ,MAP_SHARED,fd,0);

    if(map==MAP_FAILED){
        perror("Error mmap");
        close(fd);
        return NULL;
    }

    return map;
}

int hazChecksum(char *filename){

    char *map = mapFile(filename);

    struct stat st;
    fstat(fd,&st);

    long fs = st.st_size;

    long sum = 0;

    for(long i=0;i<fs;i++)
        sum += map[i];

    printf("Checksum: %lx\n",sum);

    munmap(map,fs);
    close(fd);

    return 0;
}

int main(int argc,char *argv[]){

    if(argc!=2){
        printf("Uso: %s archivo\n",argv[0]);
        return -1;
    }

    hazChecksum(argv[1]);

    return 0;
}