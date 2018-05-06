#include "sdcp_common.h"

SDCP_Common::SDCP_Common()
{

}

SDCP_Common::~SDCP_Common()
{

}

bool SDCP_Common::sessionId2Bytes(const string &id , unsigned char *bytes, int index)
{
    if (id.length() != SessionID_ByteSize*2)
        return false;

    for (int i = 0, j = 0; j < SessionID_ByteSize; i+=2, ++j) {
        char ch1 = 0;
        if (id[i] >= '0' && id[i] <= '9')
            ch1 = id[i] - '0';
        else if (id[i] >= 'a' && id[i] <= 'z')
            ch1 = id[i] - 'a' + 10;
        else if (id[i] >= 'A' && id[i] <= 'Z')
            ch1 = id[i] - 'A' + 10;

        char ch2 = 0;
        if (id[i+1] >= '0' && id[i+1] <= '9')
            ch2 = id[i+1] - '0';
        else if (id[i+1] >= 'a' && id[i+1] <= 'z')
            ch2 = id[i+1] - 'a' + 10;
        else if (id[i+1] >= 'A' && id[i+1] <= 'Z')
            ch2 = id[i+1] - 'A' + 10;

        bytes[index+j] = ((ch1 & 0xf) << 4) | (ch2 & 0xf);
    }
    return true;
}


bool SDCP_Common::bytes2sessionId(const unsigned char *bytes, int startIndex, string &id)
{
    id.resize(SessionID_ByteSize*2);
    id = string(SessionID_ByteSize*2, 0);

    for (int i = 0, j = 0; i < SessionID_ByteSize; ++i, j+=2) {
        char ch1 = (bytes[startIndex+i] >> 4) & 0xf;
        char ch2 = bytes[startIndex+i] & 0xf;

        if (ch1 >=0 && ch1 <=9)
            ch1 += '0';
        else if (ch1 >=10 && ch1 <=15)
            ch1 += 'A' - 10;
        id[j] = ch1;

        if (ch2 >=0 && ch2 <=9)
            ch2 += '0';
        else if (ch2 >=10 && ch2 <=15)
            ch2 += 'A' - 10;
        id[j+1] = ch2;
    }
    return true;
}

unsigned long long SDCP_Common::makeAdler32(const unsigned char *buff, int length)
{
    const unsigned int ADLER32_BASE = 65521;

    unsigned long long adler = 1;
    unsigned long long s1 = adler & 0xffff;
    unsigned long long s2 = (adler >> 16) & 0xffff;

    for (int i = 0; i < length; i++) {
        s1 = (s1 + buff[i]) % ADLER32_BASE;
        s2 = (s2 + s1) % ADLER32_BASE;
    }
    return (s2 << 16) + s1;
}

void SDCP_Common::short2bytes(unsigned short num, unsigned char *bytes, int index)
{
    for (int i = 0; i < Short_ByteSize; ++i) {
        bytes[index+i] = (num >> (Bits_Per_Byte * (Short_ByteSize - i - 1))) & 0xff;
    }
}

void SDCP_Common::int2bytes(unsigned int num, unsigned char *bytes, int index)
{
    for (int i = 0; i < Int_ByteSize; ++i) {
        bytes[index+i] = (num >> (Bits_Per_Byte * (Int_ByteSize - i - 1))) & 0xff;
    }
}

void SDCP_Common::long2bytes(unsigned long long num, unsigned char *bytes, int index)
{
    for (int i = 0; i < Long_ByteSize; i++) {
        bytes[index+i] = (num >> (Bits_Per_Byte * (Long_ByteSize - i - 1))) & 0xff;
    }
}

unsigned short SDCP_Common::bytes2short(const unsigned char *bytes, int startIndex)
{
    unsigned short sourceNumber = 0;
    for (int i = startIndex; i < startIndex + Short_ByteSize; i++) {
        sourceNumber <<= Bits_Per_Byte;
        sourceNumber |= (bytes[i] & 0xff);
    }
    return sourceNumber;
}

unsigned int SDCP_Common::bytes2int(const unsigned char *bytes, int startIndex)
{
    unsigned int sourceNumber = 0;
    for (int i = startIndex; i < startIndex + Int_ByteSize; i++) {
        sourceNumber <<= Bits_Per_Byte;
        sourceNumber |= (bytes[i] & 0xff);
    }
    return sourceNumber;
}

unsigned long long SDCP_Common::bytes2long(const unsigned char *bytes, int startIndex)
{
    unsigned long long sourceNumber = 0;
    for (int i = startIndex; i < startIndex + Long_ByteSize; i++) {
        sourceNumber <<= Bits_Per_Byte;
        sourceNumber |= bytes[i] & 0xff;
    }
    return sourceNumber;
}
