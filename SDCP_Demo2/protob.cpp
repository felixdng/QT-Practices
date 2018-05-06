#include "protob.h"
#include <string.h>

Protob::Protob()
{
    protobEncodeMap.clear();
    protobDecodeMap.clear();

    protobEncodeMap.insert(pair<unsigned int, bool (Protob::*)(const void *, string &)>(MT_Connection, PB_Connect_Encode));
    protobDecodeMap.insert(pair<unsigned int, bool (Protob::*)(const unsigned char *, unsigned int, void **)>(MT_Connection, PB_Connect_Decode));
}

Protob::~Protob()
{
    if (_ConnectData != nullptr) {
        delete[] _ConnectData;
        _ConnectData = nullptr;
    }
}

bool Protob::PB_Connect_Encode(const void *arg, string &pbBuf)
{
    pbBuf = (char *)arg;
    return true;
}

bool Protob::PB_Connect_Decode(const unsigned char *pbBuf, unsigned int size, void **arg)
{
    if (_ConnectData == nullptr)
        _ConnectData = new char[100];

    memset(_ConnectData, 0, 100);
    for (int i = 0; i < (int)size; ++i) {
        if (i >= 100)
            break;
        _ConnectData[i] = pbBuf[i];
    }

    *arg = (void *)_ConnectData;
    return true;
}
