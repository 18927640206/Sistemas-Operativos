#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int TAM;

int *num;

void bubble_sort(int *array,int n);
void merge_arrays(int *A,int *L,int lc,int *R,int rc);

void *ordenar1(void *ps)
{
    bubble_sort(num, TAM / 2);
    pthread_exit(0);
}

void *ordenar2(void *ps)
{
    bubble_sort(num + TAM / 2, TAM - TAM / 2);
    pthread_exit(0);
}

int main2()
{
    num = malloc(TAM * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < TAM; i++)
        num[i] = rand() % 100000;

    clock_t ini, fin;

    ini = clock();
    bubble_sort(num, TAM);
    fin = clock();

    printf("Sin multiprogramación: %.4f s\n",
           (double)(fin - ini) / CLOCKS_PER_SEC);

    for (int i = 0; i < TAM; i++)
        num[i] = rand() % 100000;

    pthread_t tid1, tid2;

    ini = clock();

    pthread_create(&tid1, NULL, ordenar1, NULL);
    pthread_create(&tid2, NULL, ordenar2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    merge_arrays(num, num, TAM / 2, num + TAM / 2, TAM - TAM / 2);

    fin = clock();

    printf("Con hilos: %.4f s\n",
           (double)(fin - ini) / CLOCKS_PER_SEC);

    free(num);
    return 0;
}


