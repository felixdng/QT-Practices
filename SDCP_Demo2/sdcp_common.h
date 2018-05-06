#ifndef SDCP_COMMON_H
#define SDCP_COMMON_H

#include <string>

using std::string;

class SDCP_Common
{
public:
    SDCP_Common();
    ~SDCP_Common();

    //byte size of header info
    enum {
        SessionID_ByteSize = 16U,
        MsgType_ByteSize   = 4U,
        SN_ByteSize        = 4U,
        Length_ByteSize    = 2U,
        Adler32_ByteSize   = 8U,
        UpdGroup_ByteSize  = 4U,
        RespCode_ByteSize  = 2U,

        //byte size of client's header
        CHeader_ByteSize = SessionID_ByteSize + MsgType_ByteSize + SN_ByteSize + Length_ByteSize + Adler32_ByteSize,

        //byte size of server's header
        SHeader_ByteSize = MsgType_ByteSize + SN_ByteSize + UpdGroup_ByteSize + RespCode_ByteSize + Length_ByteSize + Adler32_ByteSize
    };

    enum {
        Short_ByteSize  = 2U,
        Int_ByteSize    = 4U,
        Long_ByteSize   = 8U,
        Bits_Per_Byte   = 8U,
    };

    // encode and decode status
    enum {
        Encode_OK = 0,
        Encode_Protob_Failed = -1,

        Decode_OK = 0,
        Decode_Illegal_Length = -1,
        Decode_Protob_Failed = -2,
        Decode_Adler32_Failed = -3,
        Decode_Protob_Udf = -4,
    };

    bool sessionId2Bytes(const string &id , unsigned char *bytes, int index);
    bool bytes2sessionId(const unsigned char *bytes, int startIndex, string &id);
    unsigned long long  makeAdler32(const unsigned char *buff, int length);

    void short2bytes(unsigned short num, unsigned char *bytes, int index);
    void int2bytes(unsigned int num, unsigned char *bytes, int index);
    void long2bytes(unsigned long long num, unsigned char *bytes, int index);

    unsigned short bytes2short(const unsigned char *bytes, int startIndex);
    unsigned int bytes2int(const unsigned char *bytes, int startIndex);
    unsigned long long bytes2long(const unsigned char *bytes, int startIndex);

private:
    // = Disallow copying and assignment
    SDCP_Common(const SDCP_Common &);
    void operator=(const SDCP_Common &);
};

#endif // SDCP_COMMON_H
