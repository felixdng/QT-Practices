#ifndef SDCP_SERVER_H
#define SDCP_SERVER_H

#include "sdcp_common.h"
#include "protob.h"

class SDCP_Server : public SDCP_Common, public Protob
{
public:
    SDCP_Server();
    ~SDCP_Server();

    int encode(unsigned int msgType, unsigned int sn, unsigned int updGroup,
               unsigned short respCode, const void *buff);
    int decode(const unsigned char *buff, unsigned int size);

    string getSessionId() const;
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
    SDCP_Server(const SDCP_Server &);
    void operator=(const SDCP_Server &);

    string _SessionId{"0000000000000000"};
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

#endif // SDCP_SERVER_H
