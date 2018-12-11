#include <algorithm>
#include <iostream>
#include "GlobalData.h"

bool GlobalData::isExistingRoom(string roomName) {
    return roomNameToConnectionDescriptorsMap.find(roomName) != roomNameToConnectionDescriptorsMap.end();
}

void GlobalData::addClient(int clientConnectionDescriptor, string roomName) {
    printf("Add client\n");
    cout << "Try to lock SENDING MESSAGE for added client" << endl;
    sendingMessage.lock();
    cout << "SENDING LOCK" << endl;
    if (isExistingRoom(roomName)) {
        roomNameToConnectionDescriptorsMap[roomName].push_back(clientConnectionDescriptor);
    } else {
        list<int> newList;
        newList.push_back(clientConnectionDescriptor);
        roomNameToConnectionDescriptorsMap[roomName] = list<int>(newList);
    }
    sendingMessage.unlock();
    printf("SENDING MESSAGE UNLOCK \n");
    printf("Client was added\n");
}


void GlobalData::removeClient(int clientConnectionDescriptor, string roomName) {
    cout << "Remove client" << endl;
    cout << "Try to lock SENDING MESSAGE for removed client" << endl;
    sendingMessage.lock();
    cout << "SENDING LOCK" << endl;
    if (isExistingRoom(roomName)) {
        roomNameToConnectionDescriptorsMap[roomName].remove(clientConnectionDescriptor);
    }
    sendingMessage.unlock();
    printf("SENDING MESSAGE UNLOCK \n");
    cout << "Client was removed" << endl;
}

list<int> GlobalData::getConnectionSocketDescriptors(string roomName) {
    cout << "Try to lock SENDING MESSAGE for getting connection socket" << endl;
    sendingMessage.lock();
    printf("SENDING MESSAGE LOCK \n");
    list<int> tempList;
    tempList = roomNameToConnectionDescriptorsMap[roomName];
    printf("Returned %d descriptors\n", static_cast<int>(tempList.size()));
    sendingMessage.unlock();
    printf("SENDING MESSAGE UNLOCK \n");
    return tempList;
}

void GlobalData::endSendingMessage() {
    sendingMessage.unlock();
    printf("SENDING MESSAGE UNLOCK \n");
    printf("Message was sent\n");
}

void GlobalData::startSendingMessage() {
    cout << "Try lock SENDING message" << endl;
    sendingMessage.lock();
    printf("SENDING MESSAGE LOCK \n");
}

list<string> GlobalData::getActivesRoomsNames() {
    list<string> result;
    for (const auto &kv:this->roomNameToConnectionDescriptorsMap) {
        if (!kv.second.empty()) {
            result.push_back(kv.first);
        }
    }
    return result;
}

