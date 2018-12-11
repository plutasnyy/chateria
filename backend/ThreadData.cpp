#include "ThreadData.h"

ThreadData::ThreadData(int connectionSocketDescriptor) : connectionSocketDescriptor(connectionSocketDescriptor) {}

int ThreadData::getConnectionSocketDescriptor() const {
    return connectionSocketDescriptor;
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
