#include <stdio.h>
#include <stdlib.h>

int TAM;

int main1();
int main2();

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Uso: %s <tam>\n",argv[0]);
        return 1;
    }

    TAM = atoi(argv[1]);

    printf("Tamaño arreglo: %d\n", TAM);

    printf("Para Hilos\n");
    main2();

    printf("Para Fork\n");
    main1();

    return 0;
}
