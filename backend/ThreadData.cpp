#include "ThreadData.h"

ThreadData::ThreadData(int connectionSocketDescriptor) : connectionSocketDescriptor(connectionSocketDescriptor) {}

int ThreadData::getConnectionSocketDescriptor() const {
    return connectionSocketDescriptor;
}

void ThreadData::setConnectionSocketDescriptor(int connectionSocketDescriptor) {
    ThreadData::connectionSocketDescriptor = connectionSocketDescriptor;
}

int ThreadData::getRoomId() const {
    return roomId;
}

void ThreadData::setRoomId(int roomId) {
    ThreadData::roomId = roomId;
}

const string &ThreadData::getThreadMessage() const {
    return threadMessage;
}

void ThreadData::setThreadMessage(const string &threadMessage) {
    ThreadData::threadMessage = threadMessage;
}

bool ThreadData::isToClose() const {
    return toClose;
}

void ThreadData::setToClose() {
    ThreadData::toClose = true;
}
