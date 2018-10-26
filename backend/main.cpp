#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define SERVER_PORT 8000
#define QUEUE_SIZE 5

struct ThreadData {
    int connectionSocketDescriptor;
};

void sendMessage(int connectionSocketDescriptor);

void *ThreadReadBehavior(void *tData) {
    char buf[7];
    pthread_detach(pthread_self());
    auto *threadData = (struct ThreadData *) tData;
    while (1) {
        read(threadData->connectionSocketDescriptor, buf, 7);
        printf("Received: %s\n", buf);
    }
    pthread_exit(NULL);
}

void *ThreadWriteBehavior(void *tData) {
    pthread_detach(pthread_self());
    auto *threadData = (struct ThreadData *) tData;
    write(threadData->connectionSocketDescriptor, "OK", 7);
    pthread_exit(NULL);
}

void handleConnection(int connectionSocketDescriptor) {
    int createReadThreadResult = 0, createWriteThreadResult = 0;
    pthread_t thread;

    auto *tData = (ThreadData *) malloc(sizeof(struct ThreadData));
    tData->connectionSocketDescriptor = connectionSocketDescriptor;

    createReadThreadResult = pthread_create(&thread, NULL, ThreadReadBehavior, (void *) tData);
    if (createReadThreadResult) {
        printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", createReadThreadResult);
        exit(-1);
    }

    createWriteThreadResult = pthread_create(&thread, NULL, ThreadWriteBehavior, (void *) tData);
    if (createWriteThreadResult) {
        printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", createWriteThreadResult);
        exit(-1);
    }

    free(tData);

}

int main(int argc, char *argv[]) {
    int serverSocketDescriptor;
    int connectionSocketDescriptor;
    int bindResult;
    int listenResult;
    char reuseAddrVal = 1;
    struct sockaddr_in serverAddress;

    memset(&serverAddress, 0, sizeof(struct sockaddr));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(SERVER_PORT);

    serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketDescriptor < 0) {
        fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
        exit(1);
    }
    setsockopt(serverSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, (char *) &reuseAddrVal, sizeof(reuseAddrVal));

    bindResult = bind(serverSocketDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr));
    if (bindResult < 0) {
        fprintf(stderr, "%s: Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n", argv[0]);
        exit(1);
    }

    listenResult = listen(serverSocketDescriptor, QUEUE_SIZE);
    if (listenResult < 0) {
        fprintf(stderr, "%s: Błąd przy próbie ustawienia wielkości kolejki.\n", argv[0]);
        exit(1);
    }

    while (1) {
        connectionSocketDescriptor = accept(serverSocketDescriptor, NULL, NULL);
        if (connectionSocketDescriptor < 0) {
            fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
            exit(1);
        }

        handleConnection(connectionSocketDescriptor);
    }

    close(serverSocketDescriptor);
    return (0);
}