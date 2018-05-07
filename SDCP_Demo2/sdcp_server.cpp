#include "sdcp_server.h"
#include <string.h>

SDCP_Server::SDCP_Server() : SDCP_Common(), Protob()
{

}

SDCP_Server::~SDCP_Server()
{
    if (_EncodeBuffer != nullptr) {
        delete[] _EncodeBuffer;
        _EncodeBuffer = nullptr;
    }
}

int SDCP_Server::encode(unsigned int msgType, unsigned int sn, unsigned int updGroup,
                        unsigned short respCode, const void *buff)
{
    _MsgType = 0x0;
    _SN = 0;
    _UpdGroup = 0x0;
    _RespCode = 0;
    _Length = 0;
    _Adler32 = 0;
    _EncodeLength = 0;
    if (_EncodeBuffer != nullptr)
        memset(_EncodeBuffer, 0, _EncodeBufferSize);

    string pbBuf{""};
    if (protobEncodeMap.find(msgType) != protobEncodeMap.end()) {
        auto pbEncode = protobEncodeMap.at(msgType);
        if (false == (this->*pbEncode)(buff, pbBuf))
            return Encode_Protob_Failed;
    }

    //init encode buffer
    _EncodeLength = pbBuf.length() + SHeader_ByteSize;
    if (_EncodeBuffer == nullptr) {
        _EncodeBuffer = new unsigned char[_EncodeLength];
        _EncodeBufferSize = _EncodeLength;
    }
    else {
        if (_EncodeLength > _EncodeBufferSize) {
            delete[] _EncodeBuffer;
            _EncodeBuffer = new unsigned char[_EncodeLength];
            _EncodeBufferSize = _EncodeLength;
        }
    }

    //msgtype
    _MsgType = msgType;
    int2bytes(_MsgType, _EncodeBuffer, 0);

    //sn
    _SN = sn;
    int2bytes(_SN, _EncodeBuffer, MsgType_ByteSize);

    //updgroup
    _UpdGroup = updGroup;
    int2bytes(_UpdGroup, _EncodeBuffer, MsgType_ByteSize+SN_ByteSize);

    //respcode
    _RespCode = respCode;
    short2bytes(_RespCode, _EncodeBuffer, MsgType_ByteSize+SN_ByteSize+UpdGroup_ByteSize);

    //length
    _Length = pbBuf.length();
    short2bytes(_Length, _EncodeBuffer, MsgType_ByteSize+SN_ByteSize+UpdGroup_ByteSize+RespCode_ByteSize);

    //adler32
    _Adler32 = makeAdler32((const unsigned char*)pbBuf.data(), (int)pbBuf.length());
    long2bytes(_Adler32, _EncodeBuffer, MsgType_ByteSize+SN_ByteSize+UpdGroup_ByteSize+RespCode_ByteSize+Length_ByteSize);

    //protobuf
    if (_Length > 0) {
        for (int i = 0; i < _Length; ++i)
            _EncodeBuffer[SessionID_ByteSize+MsgType_ByteSize+SN_ByteSize+Length_ByteSize+Adler32_ByteSize+i] = pbBuf[i];
    }

    return Encode_OK;
}

int SDCP_Server::decode(const unsigned char *buff, unsigned int size)
{
    _SessionId = "0000000000000000";
    _MsgType = 0x0;
    _SN = 0;
    _Length = 0;
    _Adler32 = 0;
    _DecodeData = nullptr;

    if (size < CHeader_ByteSize)
        return Decode_Illegal_Length;

    //length
    _Length = bytes2short(buff, SessionID_ByteSize+MsgType_ByteSize+SN_ByteSize);
    if (size != (unsigned int)(CHeader_ByteSize+_Length))
        return Decode_Illegal_Length;

    //sessionid
    bytes2sessionId(buff, 0, _SessionId);

    //msgtype
    _MsgType = bytes2int(buff, SessionID_ByteSize);

    //sn
    _SN = bytes2int(buff, SessionID_ByteSize+MsgType_ByteSize);

    //adler32
    _Adler32 = bytes2long(buff, SessionID_ByteSize+MsgType_ByteSize+SN_ByteSize+Length_ByteSize);

    const unsigned char *pbBuf = buff+SessionID_ByteSize+MsgType_ByteSize+SN_ByteSize+Length_ByteSize+Adler32_ByteSize;
    //checksum
    if (_Adler32 == makeAdler32(pbBuf, _Length)) {
        if (_Length > 0) {
            if (protobDecodeMap.find(_MsgType) != protobDecodeMap.end()) {
                auto pbDecode = protobDecodeMap.at(_MsgType);
                if (false == (this->*pbDecode)(pbBuf, _Length, &_DecodeData))
                    return Decode_Protob_Failed;
            }
            else
                return Decode_Protob_Udf;
        }
    }
    else {
        return Decode_Adler32_Failed;
    }

    return Decode_OK;
}

string SDCP_Server::getSessionId() const
{
    return _SessionId;
}

unsigned int SDCP_Server::getMsgType() const
{
    return _MsgType;
}

unsigned int SDCP_Server::getSN() const
{
    return _SN;
}

unsigned int SDCP_Server::getUpdGroup() const
{
    return _UpdGroup;
}

unsigned short SDCP_Server::getRespCode() const
{
    return _RespCode;
}

unsigned short SDCP_Server::getLength() const
{
    return _Length;
}

unsigned long long SDCP_Server::getAdler32() const
{
    return _Adler32;
}

const unsigned char *SDCP_Server::getEncodeBuffer() const
{
    return _EncodeBuffer;
}

unsigned long long SDCP_Server::getEncodeLength() const
{
    return _EncodeLength;
}

const void *SDCP_Server::getDecodeData() const
{
    return _DecodeData;
}
