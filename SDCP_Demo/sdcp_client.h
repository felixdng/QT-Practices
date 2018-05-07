#ifndef SDCP_CLIENT_H
#define SDCP_CLIENT_H

#include "sdcp_common.h"
#include "protob.h"

#include <QString>

class SDCP_Client : public SDCP_Common, public Protob
{
public:
    SDCP_Client();
    ~SDCP_Client();

    int encode(const QString &sessionId, unsigned int msgType, unsigned int sn, const void *buff);
    int decode(const unsigned char *buff, unsigned int size);

    QString getSessionId() const;
    unsigned int getMsgType() const;
    unsigned int getSN() const;
    unsigned int getUpdGroup() const;
    unsigned short getRespCode() const;
    unsigned short getLength() const;
    unsigned long long getAdler32() const;
    const unsigned char *getEncodeBuffer() const;
    unsigned long long getEncodeLength() const;
    const void *getDecodeData() const;

private:
    // = Disallow copying and assignment
    SDCP_Client(const SDCP_Client &);
    void operator=(const SDCP_Client &);

    QString _SessionId{"0000000000000000"};
    unsigned int _MsgType = 0x0;
    unsigned int _SN = 0;
    unsigned int _UpdGroup = 0x0;
    unsigned short _RespCode = 0;
    unsigned short _Length = 0;
    unsigned long long _Adler32 = 0;
    unsigned char *_EncodeBuffer = nullptr;
    unsigned long long _EncodeLength = 0;
    void *_DecodeData = nullptr;

    unsigned long long _EncodeBufferSize = 0;
};

#endif // SDCP_CLIENT_H
