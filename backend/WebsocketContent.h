#ifndef BACKEND_WEBSOCKETCONTENT_H
#define BACKEND_WEBSOCKETCONTENT_H

#include <string>

using namespace std;

const string magicKey = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";


int websocketSetContent(const char *data, int dataLength, unsigned char *dst, const unsigned int dstLen);

int websocketGetContent(const char *data, int data_length, unsigned char *dst, const unsigned int dst_len);

string encodeAcceptKey(string acceptKey);

#endif //BACKEND_WEBSOCKETCONTENT_H
