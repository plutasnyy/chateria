#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE 1024
#define NUM_THREADS 5

struct ThreadData {
    int connectstionSocketDescriptor;
};

void *ThreadBehavior(void *tData) {
    char buf[7];
    struct ThreadData *threadData = (struct ThreadData *) tData;
    while (1) {
        read(threadData->connectstionSocketDescriptor, buf, 7);
        printf("Server: %s\n", buf);
    }
    pthread_exit(NULL);
}


void handleConnection(int connectionSocketDescriptor) {
    int createResult = 0;
    pthread_t thread;
    char buf[7];
    struct ThreadData threadData;
    threadData.connectstionSocketDescriptor = connectionSocketDescriptor;

    createResult = pthread_create(&thread, NULL, ThreadBehavior, (void *) &threadData);
    if (createResult) {
        printf("1 Błąd przy próbie utworzenia wątku, kod błędu: %d\n", createResult);
        exit(-1);
    }
    while (1) {
        fgets(buf, 7, stdin);
        write(connectionSocketDescriptor, buf, 7);
    }
    pthread_detach(thread);
}


int main(int argc, char *argv[]) {
    int connectionSocketDescriptor;
    int connectResult;
    struct sockaddr_in serverAddress;
    struct hostent *serverHostEntity;

    if (argc != 3) {
        fprintf(stderr, "Sposób użycia: %s server_name port_number\n", argv[0]);
        exit(1);
    }

    serverHostEntity = gethostbyname(argv[1]);
    if (!serverHostEntity) {
        fprintf(stderr, "%s: Nie można uzyskać adresu IP serwera.\n", argv[0]);
        exit(1);
    }

    connectionSocketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if (connectionSocketDescriptor < 0) {
        fprintf(stderr, "%s: Błąd przy probie utworzenia gniazda.\n", argv[0]);
        exit(1);
    }

    memset(&serverAddress, 0, sizeof(struct sockaddr));
    serverAddress.sin_family = AF_INET;
    memcpy(&serverAddress.sin_addr.s_addr, serverHostEntity->h_addr, serverHostEntity->h_length);
    serverAddress.sin_port = htons(atoi(argv[2]));
    connectResult = connect(connectionSocketDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr));
    if (connectResult < 0) {
        fprintf(stderr, "%s: Błąd przy próbie połączenia z serwerem (%s:%i).\n", argv[0], argv[1], atoi(argv[2]));
        exit(1);
    }

    handleConnection(connectionSocketDescriptor);

    close(connectionSocketDescriptor);
    return 0;

}
