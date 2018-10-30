//
// Created by pluto on 27.10.18.
//

#include "ThreadData.h"

ThreadData::ThreadData(int connectionSocketDescriptor) : connectionSocketDescriptor(connectionSocketDescriptor) {}

int ThreadData::getConnectionSocketDescriptor() const {
    return connectionSocketDescriptor;
}

void ThreadData::setConnectionSocketDescriptor(int connectionSocketDescriptor) {
    ThreadData::connectionSocketDescriptor = connectionSocketDescriptor;
}
