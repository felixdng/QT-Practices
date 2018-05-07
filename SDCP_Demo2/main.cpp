#include <iostream>
#include <fstream>

#include "sdcp_server.h"

using namespace std;

void Print_Bytes(const unsigned char *bytes, int len)
{
    for (int i = 0; i < len; ++i) {
        printf("%02x", (bytes[i]));
        if (i != len-1)
            printf("-");
    }
    printf("\n");
}

int main()
{
#if 0
    ifstream input( "connect.txt", ios::in | ios::binary );
    if(!input) {
        cerr << "Open input file error!" << endl;
        exit( -1 );
    }

    SDCP_Server srv;

    //read from file
    {
        char rbuf[100];
        int count = input.read(rbuf, sizeof(rbuf)).gcount();
        input.close();
        Print_Bytes((const unsigned char *)rbuf, count);

        if (SDCP_Server::Decode_OK == srv.decode((const unsigned char *)rbuf, count)) {
            cout << "sessionid: " <<  srv.getSessionId() << endl;
            printf("msgtype: 0x%08x\n", srv.getMsgType());
            printf("sn: %d\n", srv.getSN());
            printf("length: %d\n", srv.getLength());
            printf("alder32: %lld\n", srv.getAdler32());
            printf("decode data: %s\n", (char *)srv.getDecodeData());
        }
    }
#endif

    //write encode stream to file
    {
        SDCP_Server srv;
        char msg[100] = "hello, the test for server encode!";
        srv.encode(Protob::MT_Connection, 2, 0xffff1111, 200, (void*)msg);
        printf("msgtype: 0x%08x\n", srv.getMsgType());
        printf("sn: %d\n", srv.getSN());
        printf("updgroup: 0x%08x\n", srv.getUpdGroup());
        printf("respcode: %d\n", srv.getRespCode());
        printf("length: %d\n", srv.getLength());
        printf("alder32: %lld\n", srv.getAdler32());
        Print_Bytes(srv.getEncodeBuffer(), srv.getEncodeLength());
        ofstream output( "server-encode.txt", ios::out | ios::binary );
        if (!output) {
            cerr << "Open output file error!" << endl;
            exit(-1);
        }
        output.write((const char*)srv.getEncodeBuffer(), srv.getEncodeLength());
        output.flush();
        output.close();
    }
    return 0;
}
