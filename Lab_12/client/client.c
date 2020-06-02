#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

void take_ip(char* args, char* ip, size_t pointer)
{
    memset(ip, 0, pointer + 1);
    for (size_t i = 0; i < pointer; ++i)
    {
        ip[i] = args[i];
    }
}

void print_help()
{
    printf("Команды для использования:\n");
    printf("/help - показывает существующие команды\n");
    printf("/exit - закрывает программу\n");
}

void recvFile(int sock, char* filename, long fileSize)
{
    char buffer[fileSize];
    memset(buffer, 0, fileSize);
    FILE* file = fopen(filename, "w");
    for(int i = 0; i < fileSize; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\n");
    if (file == NULL)
    {
        printf("Ошибка открытия файла\n");
        return;
    }
    int n = 0;
    while (n < fileSize)
    {
        n += read(sock, buffer, sizeof(buffer));
        fprintf(file, "%s", buffer);
    }
    for(int i = 0; i < fileSize; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\n");
    fclose(file);
    printf("Файл успешно получен как %s!\n", filename);
}

int main(int argc, char* argv[])
{
    char* args = argv[1];
    char* port = strchr(args, ':');
    size_t pointer = port - args;
    char ip[pointer + 1];
    take_ip(args, ip, pointer);
    printf("%s:%s\n", ip, port + 1);

    struct sockaddr_in server_addr;
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Ошибка сокета");
        return -1;
    }
    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port + 1));
    inet_aton(ip, &server_addr.sin_addr);

    int con = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (con < 0)
    {
        perror("Ошибка подключения");
        return -1;
    }

    printf("Вы подключились!\n");
    printf("Введите имя файла или команду, показанную ниже\n");
    print_help();

    char message[128];
    while (1)
    {
        scanf("%s", message);

        if (!strcmp(message, "/help"))
        {
            print_help();
        }
        else if (!strcmp(message, "/exit"))
        {
            close(sock);
            exit(0);
        }
        else
        {
            long fileSize;
            write(sock, message, strlen(message) + 1);
            int n = read(sock, &fileSize, sizeof(long));
            if (n == 0)
            {
                close(sock);
                perror("Соединение потерянно!");
                exit(0);
            }
            if (fileSize < 0)
            {
                printf("Файл занят другой программой или вообще не существует\n");
            }
            else
            {
                recvFile(sock, message, fileSize);
            }
        }
    }
}