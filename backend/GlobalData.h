#ifndef BACKEND_GLOBALDATA_H
#define BACKEND_GLOBALDATA_H

#include <string>
#include <list>
#include <mutex>

using namespace std;

class GlobalData {
public:
    string &getNewMessage();

    void setNewMessage(const string &newMessage, const int &roomIdForNewMessage);

    int getRoomIdForNewMessage();

    GlobalData();

    const list<int> &getRoomsId();

    void addRoomId(const int &roomsId);

    bool isExistingRoom(const int &id);

    void addClient();

private:
    int roomIdForNewMessage;
    int quantityOfClients;
    int quantityOfWaitingClients;
    mutex processingGlobalData, sendingMessage;
    string newMessage;
    list<int> roomsId;

};


#endif //BACKEND_GLOBALDATA_H
