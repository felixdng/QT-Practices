#ifndef PROTOB_H
#define PROTOB_H

#include <QString>
#include <QHash>

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
    QHash<unsigned int, bool (Protob::*)(const void *, QString &)> protobEncodeMap;
    QHash<unsigned int, bool (Protob::*)(const unsigned char *, unsigned int, void **)> protobDecodeMap;

    bool PB_Connect_Encode(const void *arg, QString &pbBuf);
    bool PB_Connect_Decode(const unsigned char *pbBuf, unsigned int size, void **arg);

private:
    // = Disallow copying and assignment
    Protob(const Protob &);
    void operator=(const Protob &);

    char *_ConnectData = nullptr;
};

#endif // PROTOB_H
