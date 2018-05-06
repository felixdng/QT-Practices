#include "protob.h"

Protob::Protob()
{
    protobEncodeMap.clear();
    protobDecodeMap.clear();

    protobEncodeMap.insert(MT_Connection, PB_Connect_Encode);
    protobDecodeMap.insert(MT_Connection, PB_Connect_Decode);
}

Protob::~Protob()
{
    if (_ConnectData != nullptr) {
        delete[] _ConnectData;
        _ConnectData = nullptr;
    }
}

bool Protob::PB_Connect_Encode(const void *arg, QString &pbBuf)
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
