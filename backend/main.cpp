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
#include <thread>
#include <condition_variable>
#include <time.h>
#include <sstream>
#include "ThreadData.h"
#include "GlobalData.h"

#define SERVER_PORT 8000
#define QUEUE_SIZE 5

using namespace std;

GlobalData globalData = GlobalData();

void sendMessageForRoom(string msg, int roomId) {
    printf("New thread to send message for %d\n", roomId);
    const list<int> &clientDescriptorsList = globalData.getConnectionSocketDescriptors(roomId);
    for (int clientDescriptor : clientDescriptorsList) {
        write(clientDescriptor, "NEW", 7);
    }
    globalData.endSendingMessage();
}

void threadReadFromUserBehavior(ThreadData threadData) {
    printf("Start thread \n");
    char buf[7];
    while (1) {
        printf("While\n");
        read(threadData.getConnectionSocketDescriptor(), buf, 7);
        printf("Received: %s\n", buf);
        thread sendMessageThread;
        //string msg(buf);
        sendMessageThread = thread(sendMessageForRoom, "AA12", threadData.getRoomId());
        sendMessageThread.detach();
    }

}

void threadWriteBehavior(ThreadData threadData) {
    char x[7];
    x[0] = 'O';
    x[1] = 'K';
    x[2] = static_cast<char>(threadData.getRoomId() + '0');
    x[3] = '.';
    write(threadData.getConnectionSocketDescriptor(), x, 7);
}

void handleConnection(int connectionSocketDescriptor) {
    thread threads[2];
    auto threadData = ThreadData(connectionSocketDescriptor);
    int roomId = rand() % 2;
    threadData.setRoomId(roomId);
    printf("****** New room ID: %d ****** \n", roomId);

    globalData.addClient(connectionSocketDescriptor, roomId);
    threads[0] = thread(threadWriteBehavior, threadData);
    threads[1] = thread(threadReadFromUserBehavior, threadData);

    for (auto &th : threads) th.detach();
}

int main(int argc, char *argv[]) {

    srand(time(NULL));
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
        printf("Zaczynam nasluchiwanie:\n");
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