#ifndef BACKEND_THREADDATA_H
#define BACKEND_THREADDATA_H


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

};


#endif //BACKEND_THREADDATA_H
