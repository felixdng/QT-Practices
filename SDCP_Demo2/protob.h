#ifndef PROTOB_H
#define PROTOB_H

#include <string>
#include <map>

using namespace std;

class Protob
{
public:
    Protob();
    ~Protob();

    //message types
    enum {
        MT_Connection = 0x6000F001U,
        //...
    };

protected:
    map<unsigned int, bool (Protob::*)(const void *, string &)> protobEncodeMap;
    map<unsigned int, bool (Protob::*)(const unsigned char *, unsigned int, void **)> protobDecodeMap;

    bool PB_Connect_Encode(const void *arg, string &pbBuf);
    bool PB_Connect_Decode(const unsigned char *pbBuf, unsigned int size, void **arg);

private:
    // = Disallow copying and assignment
    Protob(const Protob &);
    void operator=(const Protob &);

    char *_ConnectData = nullptr;
};

#endif // PROTOB_H
