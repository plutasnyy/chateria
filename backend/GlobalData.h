#ifndef BACKEND_GLOBALDATA_H
#define BACKEND_GLOBALDATA_H

#include <string>
#include <list>
#include <mutex>
#include <map>

using namespace std;

class GlobalData {
public:
    void addClient(int clientConnectionDescriptor, int roomId);

    list<int> getConnectionSocketDescriptors(int roomId);

    void endSendingMessage();

private:
    bool isExistingRoom(const int &id);
    mutex processingGlobalData, sendingMessage;
    map<int,list<int>> roomIdToConnectionDescriptorsMap;

};


#endif //BACKEND_GLOBALDATA_H
