#include <QCoreApplication>
#include <QDebug>
#include "sdcp_common.h"
#include "sdcp_client.h"

#include <fstream>
#include <iostream>
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if 0
    //write encode stream to file
    {
        QString sessid{"57D99D89caab482aa0e9a0a803eed3ba"};
        char msg[100] = "hello, the test for client encode!";
        SDCP_Client cli;
        cli.encode(sessid, Protob::MT_Connection, 1, msg);
        ofstream output( "connect.txt", ios::out | ios::binary );
        if (!output) {
            cerr << "Open output file error!" << endl;
            exit(-1);
        }
        output.write((const char*)cli.getEncodeBuffer(), cli.getEncodeLength());
        output.flush();
        output.close();
    }
#endif

#if 1
    //read from file
    {
        ifstream input( "server-encode.txt", ios::in | ios::binary );
        if(!input) {
            cerr << "Open input file error!" << endl;
            exit( -1 );
        }

        char rbuf[100];
        int count = input.read(rbuf, sizeof(rbuf)).gcount();
        input.close();
        printf("count: %d\n", count);
        Print_Bytes((const unsigned char *)rbuf, count);

        SDCP_Client cli;
        if (SDCP_Client::Decode_OK == cli.decode((const unsigned char *)rbuf, count)) {
            printf("msgtype: 0x%08x\n", cli.getMsgType());
            printf("sn: %d\n", cli.getSN());
            printf("updgroup: 0x%08x\n", cli.getUpdGroup());
            printf("respcode: %d\n", cli.getRespCode());
            printf("length: %d\n", cli.getLength());
            printf("alder32: %lld\n", cli.getAdler32());
            printf("decode data: %s\n", (char *)cli.getDecodeData());
        }
    }
#endif
    return a.exec();
}
