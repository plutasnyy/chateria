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
