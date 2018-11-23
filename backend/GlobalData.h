#ifndef BACKEND_GLOBALDATA_H
#define BACKEND_GLOBALDATA_H

#include <string>
#include <list>
#include <mutex>
#include <map>

using namespace std;

class GlobalData {

public:
    const map<int, string> &getRoomIdToNameMap() const;

    void addClient(int clientConnectionDescriptor, int roomId);

    list<int> getConnectionSocketDescriptors(int roomId);

    void endSendingMessage();

    const map<string, int> &getRoomNameToIdMap() const;

    void setRoomNameToIdMap(const map<string, int> &roomNameToIdMap);

    list <string> getActivesRoomsNames();

    void setRoomIdToNameMap(const map<int, string> &roomIdToNameMap);

    void startSendingMessage();

private:
    bool isExistingRoom(const int &id);

    mutex processingGlobalData, sendingMessage;
    map<int, list < int>> roomIdToConnectionDescriptorsMap;
    map<string, int> roomNameToIdMap;
    map<int, string> roomIdToNameMap;
};


#endif //BACKEND_GLOBALDATA_H
