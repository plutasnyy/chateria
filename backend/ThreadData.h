#ifndef BACKEND_THREADDATA_H
#define BACKEND_THREADDATA_H

#include <string>

using namespace std;

class ThreadData {
public:
    ThreadData(int connectionSocketDescriptor);

    int getConnectionSocketDescriptor() const;

    const string &getThreadMessage() const;

    void setThreadMessage(const string &threadMessage);

    bool isToClose() const;

    void setToClose();

    const string &getRoomName() const;

    void setRoomName(const string &roomName);

private:
    int connectionSocketDescriptor;
    string threadMessage;
    string roomName = "";
    bool toClose = false;

};


#endif //BACKEND_THREADDATA_H
