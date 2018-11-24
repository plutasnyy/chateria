#ifndef BACKEND_THREADDATA_H
#define BACKEND_THREADDATA_H

#include <string>

using namespace std;

class ThreadData {
public:
    void setConnectionSocketDescriptor(int connectionSocketDescriptor);

    int getRoomId() const;

    void setRoomId(int roomId);

    int getConnectionSocketDescriptor() const;

    ThreadData(int connectionSocketDescriptor);

private:
    int connectionSocketDescriptor;
    int roomId;
public:
    const string &getThreadMessage() const;

    void setThreadMessage(const string &threadMessage);

private:
    string threadMessage;
    bool toClose;
public:
    bool isToClose() const;

    void setToClose();

};


#endif //BACKEND_THREADDATA_H
