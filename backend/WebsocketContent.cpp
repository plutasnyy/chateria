#include <cstdlib>
#include <cstring>
#include <sha.h>
#include <iostream>
#include "Base64.h"
#include "WebsocketContent.h"

using namespace std;

int websocketSetContent(const char *data, int dataLength, unsigned char *dst, const unsigned int dstLen) {
    unsigned char *message = (unsigned char *) malloc(65535 * sizeof(char));
    int i;
    int dataStartIndex;

    message[0] = 129;

    if (dataLength <= 125) {
        message[1] = (unsigned char) dataLength;
        dataStartIndex = 2;
    } else if (dataLength > 125 && dataLength <= 65535) {
        message[1] = 126;
        message[2] = (unsigned char) ((dataLength >> 8) & 255);
        message[3] = (unsigned char) ((dataLength) & 255);
        dataStartIndex = 4;
    } else {
        message[1] = 127;
        message[2] = (unsigned char) ((dataLength >> 56) & 255);
        message[3] = (unsigned char) ((dataLength >> 48) & 255);
        message[4] = (unsigned char) ((dataLength >> 40) & 255);
        message[5] = (unsigned char) ((dataLength >> 32) & 255);
        message[6] = (unsigned char) ((dataLength >> 24) & 255);
        message[7] = (unsigned char) ((dataLength >> 16) & 255);
        message[8] = (unsigned char) ((dataLength >> 8) & 255);
        message[9] = (unsigned char) ((dataLength) & 255);
        dataStartIndex = 10;
    }

    for (i = 0; i < dataLength; i++) {
        message[dataStartIndex + i] = (unsigned char) data[i];
    }

    for (i = 0; i < dataLength + dataStartIndex; i++) {
        dst[i] = (unsigned char) message[i];
    }

    if (message) {
        free(message);
        message = NULL;
    }

    return i;
}

int websocketGetContent(const char *data, int data_length, unsigned char *dst, const unsigned int dst_len) {
    unsigned int i, j;
    unsigned char mask[4];
    unsigned int packetLength = 0;
    unsigned int lengthCode = 0;
    int indexFirstMask = 0;
    int indexFirstDataByte = 0;

    if ((unsigned char) data[0] != 129) {
        dst = NULL;
        if ((unsigned char) data[0] == 136) {
            /* WebSocket client disconnected */
            return -2;
        }
        /* Unknown error */
        return -1;
    }

    lengthCode = ((unsigned char) data[1]) & 127;

    if (lengthCode <= 125) {
        indexFirstMask = 2;

        mask[0] = data[2];
        mask[1] = data[3];
        mask[2] = data[4];
        mask[3] = data[5];
    } else if (lengthCode == 126) {
        indexFirstMask = 4;

        mask[0] = data[4];
        mask[1] = data[5];
        mask[2] = data[6];
        mask[3] = data[7];
    } else if (lengthCode == 127) {
        indexFirstMask = 10;

        mask[0] = data[10];
        mask[1] = data[11];
        mask[2] = data[12];
        mask[3] = data[13];
    }

    indexFirstDataByte = indexFirstMask + 4;

    packetLength = data_length - indexFirstDataByte;

    for (i = indexFirstDataByte, j = 0; i < data_length && j < dst_len; i++, j++) {
        dst[j] = (unsigned char) data[i] ^ mask[j % 4];
    }

    return packetLength;
}

string encodeAcceptKey(string acceptKey) {
    cout << acceptKey.size() << " " << acceptKey << endl;
    int ind = 0;
    char joinedKeysInChar[61];
    memset(joinedKeysInChar, 0, sizeof joinedKeysInChar);

    for (int i = 0; i < acceptKey.size(); i++) {
        joinedKeysInChar[i] = acceptKey[i];
        ind++;
    }
    for (int i = 0; i < magicKey.size(); i++) {
        joinedKeysInChar[ind + i] = magicKey[i];
    }

    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *) &joinedKeysInChar, strlen(joinedKeysInChar), (unsigned char *) &digest);
    vector<BYTE> myData;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        myData.push_back(digest[i]);
    }
    string encodedData = base64_encode(&myData[0], static_cast<unsigned int>(myData.size()));
    return encodedData;
}