#include <algorithm>
#include <iostream>
#include "GlobalData.h"

bool GlobalData::isExistingRoom(string roomName) {
    return roomNameToConnectionDescriptorsMap.find(roomName) != roomNameToConnectionDescriptorsMap.end();
}

void GlobalData::addClient(int clientConnectionDescriptor, string roomName) {
    printf("Add client\n");
    processingGlobalData.lock();
    sendingMessage.lock();
    if (isExistingRoom(roomName)) {
        roomNameToConnectionDescriptorsMap[roomName].push_back(clientConnectionDescriptor);
    } else {
        list<int> newList;
        newList.push_back(clientConnectionDescriptor);
        roomNameToConnectionDescriptorsMap[roomName] = list<int>(newList);
    }
    sendingMessage.unlock();
    processingGlobalData.unlock();
    printf("Client was added\n");
}


void GlobalData::removeClient(int clientConnectionDescriptor, string roomName) {
    cout<<"Remove client"<<endl;
    processingGlobalData.lock();
    sendingMessage.lock();
    if (isExistingRoom(roomName)) {
        roomNameToConnectionDescriptorsMap[roomName].remove(clientConnectionDescriptor);
    }
    sendingMessage.unlock();
    processingGlobalData.unlock();
    cout<<"Client removed"<<endl;
}

list<int> GlobalData::getConnectionSocketDescriptors(string roomName) {
    list<int> tempList;
    processingGlobalData.lock();
    tempList = roomNameToConnectionDescriptorsMap[roomName];
    processingGlobalData.unlock();
    printf("Returned %d descriptors\n", static_cast<int>(tempList.size()));
    return tempList;
}

void GlobalData::endSendingMessage() {
    processingGlobalData.lock();
    sendingMessage.unlock();
    printf("SENDING MESSAGE UNLOCK \n");
    processingGlobalData.unlock();
    printf("Message sended\n");
}

void GlobalData::startSendingMessage() {
    processingGlobalData.lock();
    sendingMessage.lock();
    printf("SENDING MESSAGE LOCK \n");
    processingGlobalData.unlock();
}

const map<string, int> &GlobalData::getRoomNameToIdMap() const {
    return roomNameToIdMap;
}

void GlobalData::setRoomNameToIdMap(const map<string, int> &roomNameToIdMap) {
    GlobalData::roomNameToIdMap = roomNameToIdMap;
}

const map<int, string> &GlobalData::getRoomIdToNameMap() const {
    return roomIdToNameMap;
}

void GlobalData::setRoomIdToNameMap(const map<int, string> &roomIdToNameMap) {
    GlobalData::roomIdToNameMap = roomIdToNameMap;
}

list<string> GlobalData::getActivesRoomsNames() {
    list<string> result;
    for(const auto& kv:this->roomNameToConnectionDescriptorsMap){
        if(!kv.second.empty()){
            result.push_back(kv.first);
        }
    }
    return result;
}

