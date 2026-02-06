#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>

extern int TAM;

void bubble_sort(int *array, int n);
void merge_arrays(int *A,int *L,int lc,int *R,int rc);

void *crea_memoria_compartida(size_t tam)
{
    int proteccion = PROT_READ | PROT_WRITE;
    int visibilidad = MAP_SHARED | MAP_ANONYMOUS;
    return mmap(NULL, tam, proteccion, visibilidad, -1, 0);
}

int main1()
{
    int *num = crea_memoria_compartida(TAM * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < TAM; i++)
        num[i] = rand() % 100000;

    clock_t ini, fin;

    /* SIN multiprogramación */
    ini = clock();
    bubble_sort(num, TAM);
    fin = clock();

    printf("Sin multiprogramación: %.4f s\n",
           (double)(fin - ini) / CLOCKS_PER_SEC);

    for (int i = 0; i < TAM; i++)
        num[i] = rand() % 100000;

    ini = clock();

    int pid = fork();

    if (pid > 0)
    {
        bubble_sort(num, TAM / 2);

        wait(NULL);

        merge_arrays(num, num, TAM / 2, num + TAM / 2, TAM - TAM / 2);
    }
    else
    {
        bubble_sort(num + TAM / 2, TAM - TAM / 2);
        exit(0);
    }

    fin = clock();

    printf("Con fork: %.4f s\n",
           (double)(fin - ini) / CLOCKS_PER_SEC);

    return 0;
}

/* ===== tomado de ordena.c ===== */

void bubble_sort(int *array, int n)
{
    int tmp, flag;
    do
    {
        flag = 1;
        for (int i = 1; i < n; i++)
        {
            if (array[i - 1] > array[i])
            {
                tmp = array[i];
                array[i] = array[i - 1];
                array[i - 1] = tmp;
                flag = 0;
            }
        }
    } while (!flag);
}

void merge_arrays(int *A,int *L,int lc,int *R,int rc)
{
    int i = 0, j = 0, k = 0;
    int *tmp = malloc((lc + rc) * sizeof(int));

    while (i < lc && j < rc)
        tmp[k++] = (L[i] < R[j]) ? L[i++] : R[j++];

    while (i < lc) tmp[k++] = L[i++];
    while (j < rc) tmp[k++] = R[j++];

    for (i = 0; i < lc + rc; i++)
        A[i] = tmp[i];

    free(tmp);
}
