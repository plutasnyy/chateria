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
#include <string.h>
#include <thread>
#include <condition_variable>
#include <time.h>
#include <sstream>
#include <iostream>
#include <sha.h>
#include <bits/stdc++.h>
#include "ThreadData.h"
#include "GlobalData.h"
#include "Base64.h"
#include "WebsocketContent.h"

#define SERVER_PORT 8000
#define QUEUE_SIZE 5

#define BUF_SIZE 2048

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
    cout << "Started reading thread\n";
    char *buffer = new char[BUF_SIZE];
    char *buffer3 = "SIEMANKO NOWA WIADOMOSC";
    unsigned char *buffer2 = reinterpret_cast<unsigned char *>(new char[BUF_SIZE]);
    char *buffer4 = new char[BUF_SIZE];
    unsigned char *buffer5 = reinterpret_cast<unsigned char *>(new char[BUF_SIZE]);
    int desc = threadData.getConnectionSocketDescriptor();
    read(desc, buffer, BUF_SIZE);
    printf("Received: %s\n*************END****************\n", buffer);
    string handshakeMessage = buffer;
    string findString = "Sec-WebSocket-Key: ";
    unsigned long secInd = handshakeMessage.find("Sec-WebSocket-Key: ") + findString.size();
    string key = handshakeMessage.substr(secInd, 24);

    string returnMessage = "HTTP/1.1 101 Switching Protocols\r\n"
                           "Upgrade: websocket\r\n"
                           "Connection: Upgrade\r\n"
                           "Sec-WebSocket-Accept: " + encodeAcceptKey(key) + "\r\n\r\n";
    string returnMessage2 = "dupa " + returnMessage;
    cout << returnMessage << endl;
    write(desc, returnMessage.c_str(), returnMessage.size());
    sleep(2);
    cout<<"WYSYLAM"<<endl;
    websocketSetContent(buffer3, strlen(buffer3), buffer2, BUF_SIZE);
    write(desc, buffer2, BUF_SIZE);
    while (1) {
        read(desc, buffer4, BUF_SIZE);
        printf("Received: %s\n*************END****************\n", buffer4);
        websocketGetContent(buffer4, BUF_SIZE, buffer5, BUF_SIZE);
        cout<< buffer5 <<endl;
        //thread sendMessageThread;
        //string msg(buf);
        // sendMessageThread = thread(sendMessageForRoom, "AA12", threadData.getRoomId());
        // sendMessageThread.detach();
    }
}

void handleConnection(int connectionSocketDescriptor) {
    cout << "Handle connection for: " << connectionSocketDescriptor << endl;
    thread threads[1];
    auto threadData = ThreadData(connectionSocketDescriptor);
    int roomId = -1;
    globalData.addClient(connectionSocketDescriptor, roomId);
    threads[0] = thread(threadReadFromUserBehavior, threadData);
    threads[0].join();
}

int main(int argc, char *argv[]) {
//    cout<< encodeAcceptKey("jyhRMwvMcQHoJ+6ng45kIg==");
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
