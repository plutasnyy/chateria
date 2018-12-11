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

private:
    int connectionSocketDescriptor;
    string threadMessage;
    bool toClose = false;

};


#endif //BACKEND_THREADDATA_H
