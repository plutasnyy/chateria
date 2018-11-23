#include <algorithm>
#include "GlobalData.h"

bool GlobalData::isExistingRoom(const int &id) {
    return roomIdToConnectionDescriptorsMap.find(id) != roomIdToConnectionDescriptorsMap.end();
}

void GlobalData::addClient(int clientConnectionDescriptor, int roomId) {
    printf("Add client\n");
    processingGlobalData.lock();
    sendingMessage.lock();
    if (isExistingRoom(roomId)) {
        roomIdToConnectionDescriptorsMap[roomId].push_back(clientConnectionDescriptor);
    } else {
        list<int> newList;
        newList.push_back(clientConnectionDescriptor);
        roomIdToConnectionDescriptorsMap[roomId] = list<int>(newList);
    }
    sendingMessage.unlock();
    processingGlobalData.unlock();
    printf("Client was added\n");
}

list<int> GlobalData::getConnectionSocketDescriptors(int roomId) {
    list<int> tempList;
    processingGlobalData.lock();
    tempList = roomIdToConnectionDescriptorsMap[roomId];
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
    for(const auto& kv:this->roomIdToConnectionDescriptorsMap){
        if(!kv.second.empty()){
            result.push_back(this->roomIdToNameMap[kv.first]);
        }
    }
    return result;
}
