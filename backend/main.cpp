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
#include "ThreadData.h"
#include "GlobalData.h"

#define SERVER_PORT 8000
#define QUEUE_SIZE 5

using namespace std;

GlobalData globalData = GlobalData();
std::mutex mtx;
std::condition_variable cv;

void threadReadFromUserBehavior(ThreadData threadData) {

    printf("Start thread \n");
    char buf[7];
    while (1) {
        printf("After sleep\n");
        read(threadData.getConnectionSocketDescriptor(), buf, 7);
        printf("Received: %s\n", buf);
        string msg(buf);
        globalData.setNewMessage(msg, threadData.getRoomId());
        cv.notify_all();
        printf("Sended\n");
    }

}

void threadReadFromServerBehavior(ThreadData threadData) {
    char buf[7];
    string msg;
    while (1) {
        printf("Loop start\n");
        unique_lock<mutex> lck(mtx);
        printf("Unique lock\n");
        cv.wait(lck);
        printf("Unlocked\n");
        globalData.getNewMessage();
//        for(int i=0;i<2;i++){
//            buf[i] = msg[i];
//        }
        printf("Message prepared\n");
        write(threadData.getConnectionSocketDescriptor(), "NEW", 7);
        printf("Message sended\n");
    }

}

void threadWriteBehavior(ThreadData threadData) {
    write(threadData.getConnectionSocketDescriptor(), "OK", 7);
    sleep(10000);
}

void handleConnection(int connectionSocketDescriptor) {
    thread threads[3];
    auto threadData = ThreadData(connectionSocketDescriptor);
    int roomId = rand() % 1000;
    printf("****** New room ID: %d ****** \n", roomId);
    globalData.addRoomId(roomId);
    threadData.setRoomId(roomId);
    globalData.addClient();
    threads[0] = thread(threadWriteBehavior, threadData);
    threads[1] = thread(threadReadFromUserBehavior, threadData);
    threads[2] = thread(threadReadFromServerBehavior, threadData);

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