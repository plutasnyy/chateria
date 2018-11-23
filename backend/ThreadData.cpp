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
