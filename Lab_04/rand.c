#include <stdio.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int chisl = 20;

int func(const void* x, const void* y)
{
    return (*((int*)x) - *((int*)y));
}

void sort (int* arr)
{
    qsort(arr, chisl, sizeof(int), func);
    for (size_t i = 0; i < chisl; ++i)
    {
        printf("%d ", arr[i]);
    }
}

int main()
{
    srand(time(NULL));
    int memId = shmget(IPC_PRIVATE, sizeof(int) * chisl, 0600 | IPC_CREAT | IPC_EXCL);
    int *arr = (int*)shmat(memId, 0, 0);

    for (size_t i = 0; i < chisl; ++i)
    {
        arr[i] =  rand() % 51;
    }

    for (size_t i = 0; i < chisl; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int child_id = fork();
    if (child_id == 0)
    {
        sort(arr);
    }
    else
    {
        waitpid(child_id, NULL, 0);
    }
    shmdt(arr);
    return 0;
}
