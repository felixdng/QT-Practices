#include "sdcp_client.h"

SDCP_Client::SDCP_Client() : SDCP_Common(), Protob()
{

}

SDCP_Client::~SDCP_Client()
{
    if (_EncodeBuffer != nullptr) {
        delete[] _EncodeBuffer;
        _EncodeBuffer = nullptr;
    }
}

int SDCP_Client::encode(const QString &sessionId, unsigned int msgType, unsigned int sn, const void *buff)
{
    _SessionId = "0000000000000000";
    _MsgType = 0x0;
    _SN = 0;
    _Length = 0;
    _Adler32 = 0;
    _EncodeLength = 0;
    if (_EncodeBuffer != nullptr)
        memset(_EncodeBuffer, 0, _EncodeBufferSize);

    QString pbBuf{""};
    if (protobEncodeMap.contains(msgType)) {
        auto pbEncode = protobEncodeMap.value(msgType);
        if (false == (this->*pbEncode)(buff, pbBuf))
            return Encode_Protob_Failed;
    }

    //init encode buffer
    _EncodeLength = pbBuf.length() + CHeader_ByteSize;
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

    //sessionid
    _SessionId = sessionId;
    sessionId2Bytes(_SessionId, _EncodeBuffer, 0);

    //msgtype
    _MsgType = msgType;
    int2bytes(_MsgType, _EncodeBuffer, SessionID_ByteSize);

    //sn
    _SN = sn;
    int2bytes(_SN, _EncodeBuffer, SessionID_ByteSize+MsgType_ByteSize);

    //length
    _Length = pbBuf.length();
    short2bytes(_Length, _EncodeBuffer, SessionID_ByteSize+MsgType_ByteSize+SN_ByteSize);

    //adler32
    _Adler32 = makeAdler32((const unsigned char*)pbBuf.toUtf8().data(), pbBuf.length());
    long2bytes(_Adler32, _EncodeBuffer, SessionID_ByteSize+MsgType_ByteSize+SN_ByteSize+Length_ByteSize);

    //protobuf
    if (_Length > 0) {
        for (int i = 0; i < _Length; ++i)
            _EncodeBuffer[SessionID_ByteSize+MsgType_ByteSize+SN_ByteSize+Length_ByteSize+Adler32_ByteSize+i] = pbBuf[i].cell();
    }

    return Encode_OK;
}

int SDCP_Client::decode(const unsigned char *buff, unsigned int size)
{
    _MsgType = 0x0;
    _SN = 0;
    _UpdGroup = 0x0;
    _RespCode = 0;
    _Length = 0;
    _Adler32 = 0;
    _DecodeData = nullptr;

    if (size < SHeader_ByteSize)
        return Decode_Illegal_Length;

    //length
    _Length = bytes2short(buff, MsgType_ByteSize+SN_ByteSize+UpdGroup_ByteSize+RespCode_ByteSize);
    if (size != (unsigned int)(SHeader_ByteSize+_Length))
        return Decode_Illegal_Length;

    //msgtype
    _MsgType = bytes2int(buff, 0);

    //sn
    _SN = bytes2int(buff, MsgType_ByteSize);

    //updgroup
    _UpdGroup = bytes2int(buff, MsgType_ByteSize+SN_ByteSize);

    //respcode
    _RespCode = bytes2short(buff, MsgType_ByteSize+SN_ByteSize+UpdGroup_ByteSize);

    //adler32
    _Adler32 = bytes2long(buff, MsgType_ByteSize+SN_ByteSize+UpdGroup_ByteSize+RespCode_ByteSize+Length_ByteSize);

    const unsigned char *pbBuf = buff+MsgType_ByteSize+SN_ByteSize+UpdGroup_ByteSize+RespCode_ByteSize+Length_ByteSize+Adler32_ByteSize;
    //checksum
    if (_Adler32 == makeAdler32(pbBuf, _Length)) {
        if (_Length > 0) {
            if (protobDecodeMap.contains(_MsgType)) {
                auto pbDecode = protobDecodeMap.value(_MsgType);
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

QString SDCP_Client::getSessionId() const
{
    return _SessionId;
}

unsigned int SDCP_Client::getMsgType() const
{
    return _MsgType;
}

unsigned int SDCP_Client::getSN() const
{
    return _SN;
}

unsigned int SDCP_Client::getUpdGroup() const
{
    return _UpdGroup;
}

unsigned short SDCP_Client::getRespCode() const
{
    return _RespCode;
}

unsigned short SDCP_Client::getLength() const
{
    return _Length;
}

unsigned long long SDCP_Client::getAdler32() const
{
    return _Adler32;
}

const unsigned char *SDCP_Client::getEncodeBuffer() const
{
    return _EncodeBuffer;
}

unsigned long long SDCP_Client::getEncodeLength() const
{
    return _EncodeLength;
}

const void *SDCP_Client::getDecodeData() const
{
    return _DecodeData;
}
