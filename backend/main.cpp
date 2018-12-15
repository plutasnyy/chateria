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
#include <stddef.h>
#include "ThreadData.h"
#include "GlobalData.h"
#include "Base64.h"
#include "WebsocketContent.h"
#include "json.hpp"

#define SERVER_PORT 8000
#define QUEUE_SIZE 5

#define BUF_SIZE 2048

using namespace std;
using json = nlohmann::json;

GlobalData globalData = GlobalData();

void doHandshake(int descriptor);

string getMessageFromEncodedBuffer(char *buffer);

void processThreadMessage(string basic_string, ThreadData &data);

void sendMessageForRoom(string msg, string roomName) {
    cout << "New thread to send message to " << roomName << endl;
    int frameSize = 0;
    unsigned char *buffer = reinterpret_cast<unsigned char *>(new char[BUF_SIZE]);
    frameSize = websocketSetContent(msg.c_str(), static_cast<int>(msg.size()), buffer, BUF_SIZE);
    cout << "Size: " << frameSize << endl;
    const list<int> &clientDescriptorsList = globalData.getConnectionSocketDescriptors(roomName);
    globalData.startSendingMessage();
    for (int clientDescriptor : clientDescriptorsList) {
        write(clientDescriptor, buffer, frameSize);
    }
    globalData.endSendingMessage();
}

void sendMessageForUser(string msg, int connectionSocketDescriptor) {
    int frameSize = 0;
    unsigned char *buffer = reinterpret_cast<unsigned char *>(new char[BUF_SIZE]);
    frameSize = websocketSetContent(msg.c_str(), static_cast<int>(msg.size()), buffer, BUF_SIZE);
    globalData.startSendingMessage();
    cout << "SIZE: " << frameSize << "BUFFER: " << buffer << endl;
    write(connectionSocketDescriptor, buffer, frameSize);
    globalData.endSendingMessage();
    cout << "Message sent" << endl;
}

void processMessage(char *readMessageBuffer, ThreadData &threadData) {
    cout << "Processing message" << endl;
    string message = getMessageFromEncodedBuffer(readMessageBuffer);
    cout << "Decoded message: " << message << endl;
    string fullMessage = threadData.getThreadMessage();
    for (char &c : message) {
        if (1 == c) {
            cout << "Starting to process thread messages" << endl;
            processThreadMessage(fullMessage, threadData);
            cout << "Message processed" << endl;
            threadData.setThreadMessage("");
            cout << "Message cleared" << endl;
            return;
        }
        fullMessage += c;
    }
    threadData.setThreadMessage(fullMessage);
    cout << "Message processed" << endl;
}

void addUserToRoom(string roomName, int connectionSocketDescriptor) {
    globalData.addClient(connectionSocketDescriptor, roomName);
    cout << "Thread add user added" << endl;
}

void removeUserFromRoom(string roomName, int connectionSocketDescriptor) {
    globalData.removeClient(connectionSocketDescriptor, roomName);
    cout << "User removed" << endl;
}

void processThreadMessage(string threadMessage, ThreadData &threadData) {
    json receivedJson = json::parse(threadMessage);
    string action = receivedJson.at("action");
    if (action == "GET_ROOMS") {
        json msg, rooms(globalData.getActivesRoomsNames());
        msg["action"] = "ROOM_LIST";
        msg["roomList"] = rooms;
        sendMessageForUser(msg.dump(), threadData.getConnectionSocketDescriptor());
    } else if (action == "ADD_TO_ROOM") {
        string roomName = receivedJson.at("room");
        addUserToRoom(roomName, threadData.getConnectionSocketDescriptor());
        threadData.setRoomName(roomName);
    } else if (action == "CLOSE") {
        threadData.setToClose();
    } else if (action == "EXIT_ROOM") {
        string roomName = receivedJson.at("room");
        removeUserFromRoom(roomName, threadData.getConnectionSocketDescriptor());
        threadData.setRoomName("");
    } else if (action == "MESSAGE") {
        string roomName = receivedJson.at("room");
        sendMessageForRoom(receivedJson.dump(), roomName);
    } else if (action == "PING") {
        sendMessageForUser(receivedJson.dump(), threadData.getConnectionSocketDescriptor());
    }
}

string getMessageFromEncodedBuffer(char *readMessageBuffer) {
    unsigned char *encryptedIncommingMessageBuffer = reinterpret_cast<unsigned char *>(new char[BUF_SIZE]);
    memset(encryptedIncommingMessageBuffer, 0, sizeof encryptedIncommingMessageBuffer);
    websocketGetContent(readMessageBuffer, BUF_SIZE, encryptedIncommingMessageBuffer, BUF_SIZE);
    string decodedMessage(reinterpret_cast<char *>(encryptedIncommingMessageBuffer));
    int size = 0;
    for (char &c : decodedMessage) {
        size++;
        if (1 == c) break;
    }
    return decodedMessage.substr(0, size);
}

void threadReadFromUserBehavior(ThreadData threadData) {
    cout << "Started reading thread\n";
    int desc = threadData.getConnectionSocketDescriptor(), size = 0;
    char *readMessageBuffer = new char[BUF_SIZE];
    while (1) {
        cout << "Waiting for a message" << endl;
        size = read(desc, readMessageBuffer, BUF_SIZE);
        printf("Received: \n*************START****************\n%s\n*************END****************\n",
               readMessageBuffer);
        if (size == 0) {
            if (threadData.getRoomName() != "") {
                removeUserFromRoom(threadData.getRoomName(), threadData.getConnectionSocketDescriptor());
                threadData.setRoomName("");
            }
            break;
        }

        processMessage(readMessageBuffer, threadData);

        cout << "Message processed" << endl;
        if (threadData.isToClose()) {
            cout << "Thread to close" << endl;
            break;
        }
    }
    cout << "End of reading" << endl;
}

void handleConnection(int connectionSocketDescriptor) {
    cout << "Handle connection for: " << connectionSocketDescriptor << endl;
    doHandshake(connectionSocketDescriptor);
    auto threadData = ThreadData(connectionSocketDescriptor);
    cout << "Do proccess for: " << connectionSocketDescriptor << endl;
    thread thread(threadReadFromUserBehavior, threadData);
    thread.detach();
}

void doHandshake(int connectionSocketDescriptor) {
    char *buffer = new char[BUF_SIZE];
    int size = 0;
    size = read(connectionSocketDescriptor, buffer, BUF_SIZE);
    printf("Received: %d bits\n*************END****************\n", size);
//    printf("Received: %s\n*************END****************\n", buffer);
    string handshakeMessage = buffer;
    string findString = "Sec-WebSocket-Key: ";
    unsigned long secInd = handshakeMessage.find("Sec-WebSocket-Key: ") + findString.size();
    string key = handshakeMessage.substr(secInd, 24);

    string returnMessage = "HTTP/1.1 101 Switching Protocols\r\n"
                           "Upgrade: websocket\r\n"
                           "Connection: Upgrade\r\n"
                           "Sec-WebSocket-Accept: " + encodeAcceptKey(key) + "\r\n\r\n";
//    cout << "Send: " << returnMessage << endl;
    cout << "Key sent" << endl;
    write(connectionSocketDescriptor, returnMessage.c_str(), returnMessage.size());
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
        fprintf(stderr, "%s: Error while creating a websocket..\n", argv[0]);
        exit(1);
    }
    setsockopt(serverSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, (char *) &reuseAddrVal, sizeof(reuseAddrVal));

    bindResult = bind(serverSocketDescriptor, (struct sockaddr *) &serverAddress, sizeof(struct sockaddr));
    if (bindResult < 0) {
        fprintf(stderr, "%s: Error while creating an ip connection.\n", argv[0]);
        exit(1);
    }

    listenResult = listen(serverSocketDescriptor, QUEUE_SIZE);
    if (listenResult < 0) {
        fprintf(stderr, "%s:Error while setting the size of a queue.\n", argv[0]);
        exit(1);
    }

    while (1) {
        cout << "Listening" << endl;
        connectionSocketDescriptor = accept(serverSocketDescriptor, NULL, NULL);
        if (connectionSocketDescriptor < 0) {
            fprintf(stderr, "%s: Error while creating a socket for the connection.\n", argv[0]);
            exit(1);
        }
        handleConnection(connectionSocketDescriptor);
    }

    close(serverSocketDescriptor);
    return (0);
}
