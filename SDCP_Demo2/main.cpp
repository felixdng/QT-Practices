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
    ifstream input( "connect.txt", ios::in | ios::binary );
    if(!input) {
        cerr << "Open input file error!" << endl;
        exit( -1 );
    }

    char rbuf[100];
    int count = input.read(rbuf, sizeof(rbuf)).gcount();
    input.close();
    Print_Bytes((const unsigned char *)rbuf, count);

    SDCP_Server srv;
    if (SDCP_Server::Decode_OK == srv.decode((const unsigned char *)rbuf, count)) {
        cout << "sessionid: " <<  srv.getSessionId();
        printf("msgtype: 0x%08x\n", srv.getMsgType());
        printf("sn: %d\n", srv.getSN());
        printf("length: %d\n", srv.getLength());
        printf("alder32: %lld\n", srv.getAdler32());
        printf("decode data: %s\n", (char *)srv.getDecodeData());
    }

    return 0;
}
