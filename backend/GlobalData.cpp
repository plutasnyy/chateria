#include <algorithm>
#include "GlobalData.h"

string &GlobalData::getNewMessage() {
    printf("I want get message\n");
    string tempMessage;
    processingGlobalData.lock();
    quantityOfWaitingClients--;
    tempMessage = newMessage;
    printf("MESSAGES TO RECEIVE %d\n", quantityOfWaitingClients);
    if(quantityOfWaitingClients == 0 ){
        sendingMessage.unlock();
    }
    processingGlobalData.unlock();
    printf("Message returned\n");
    return tempMessage;
}

void GlobalData::setNewMessage(const string &newMessage, const int &roomIdForNewMessage) {
    printf("I want set message \n");
    processingGlobalData.lock();
    printf("PROCESSING LOCK\n");
    sendingMessage.lock();
    printf("SENDING MESSAGE LOCK \n");
    quantityOfWaitingClients = quantityOfClients;
    this->roomIdForNewMessage = roomIdForNewMessage;
    GlobalData::newMessage = newMessage;
    processingGlobalData.unlock();
    printf("Message set\n");
}

int GlobalData::getRoomIdForNewMessage() {
    int tempRoomId;
    processingGlobalData.lock();
    tempRoomId = roomIdForNewMessage;
    processingGlobalData.unlock();
    return tempRoomId;
}

const list<int> &GlobalData::getRoomsId() {
    list<int> tempRoomsId;
    processingGlobalData.lock();
    tempRoomsId = roomsId;
    processingGlobalData.unlock();
    return roomsId;
}

void GlobalData::addRoomId(const int &roomsId) {
    printf("Add room id\n");
    processingGlobalData.lock();
    sendingMessage.lock();
    GlobalData::roomsId.push_back(roomsId);
    sendingMessage.unlock();
    processingGlobalData.unlock();
    printf("Added room id\n");
}

bool GlobalData::isExistingRoom(const int &id) {
    return find(roomsId.begin(), roomsId.end(), id) != roomsId.end();
}

GlobalData::GlobalData() {
    quantityOfClients = 0;
    newMessage = "";
}

void GlobalData::addClient() {
    printf("Add client\n");
    processingGlobalData.lock();
    sendingMessage.lock();
    quantityOfClients++;
    sendingMessage.unlock();
    processingGlobalData.unlock();
    printf("Client was added\n");
}
