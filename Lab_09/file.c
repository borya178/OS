#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    const char* name = argv[1];
    int bytesCount = atoi(argv[2]);

    FILE* file = fopen(name, "r");
    if (file == NULL)
    {
        perror("Не открыть файл");
        return -1;
    }

    int data[bytesCount];
    int res = fread(data, sizeof(int), bytesCount, file);

    printf("Результат: = %i\n", res);
    for (size_t i = 0; i < res; ++i)
    {
        printf("Случайное число = %i (0x%x)\n", data[i], data[i]);
    }
    fclose(file);
}