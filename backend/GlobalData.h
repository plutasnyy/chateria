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
    map<string, int> roomNameToIdMap;
    map<int, string> roomIdToNameMap;
public:

    const map<int, string> &getRoomIdToNameMap() const;

    void addClient(int clientConnectionDescriptor, string roomName);

    list<int> getConnectionSocketDescriptors(string roomName);

    void endSendingMessage();

    const map<string, int> &getRoomNameToIdMap() const;

    void setRoomNameToIdMap(const map<string, int> &roomNameToIdMap);

    list <string> getActivesRoomsNames();

    void setRoomIdToNameMap(const map<int, string> &roomIdToNameMap);

    void startSendingMessage();

    void removeClient(int i, string basic_string);
};


#endif //BACKEND_GLOBALDATA_H
