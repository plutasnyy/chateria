//
// Created by pluto on 27.10.18.
//

#ifndef BACKEND_THREADDATA_H
#define BACKEND_THREADDATA_H


class ThreadData {
public:
private:
    int connectionSocketDescriptor;
public:
    void setConnectionSocketDescriptor(int connectionSocketDescriptor);

public:
    int getConnectionSocketDescriptor() const;

public:
    ThreadData(int connectionSocketDescriptor);


};


#endif //BACKEND_THREADDATA_H
