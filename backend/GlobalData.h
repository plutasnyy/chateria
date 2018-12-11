#ifndef BACKEND_GLOBALDATA_H
#define BACKEND_GLOBALDATA_H

#include <string>
#include <list>
#include <mutex>
#include <map>

using namespace std;

class GlobalData {

private:
    bool isExistingRoom(string roomName);

    mutex processingGlobalData, sendingMessage;
    map<string, list < int>> roomNameToConnectionDescriptorsMap;
public:

    void addClient(int clientConnectionDescriptor, string roomName);

    list<int> getConnectionSocketDescriptors(string roomName);

    void endSendingMessage();

    list <string> getActivesRoomsNames();

    void startSendingMessage();

    void removeClient(int i, string basic_string);
};


#endif //BACKEND_GLOBALDATA_H
