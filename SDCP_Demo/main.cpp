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

    QString sessid{"57D99D89caab482aa0e9a0a803eed3ba"};
    char msg[100] = "hello, the test for client encode!";
    SDCP_Client cli;
    cli.encode(sessid, Protob::MT_Connection, 1, msg);
//    qDebug() << "sessionid: " <<  cli.getSessionId();
//    printf("msgtype: 0x%08x\n", cli.getMsgType());
//    printf("length: %d\n", cli.getLength());
//    printf("encode length: %llu\n", cli.getEncodeLength());
    Print_Bytes(cli.getEncodeBuffer(), cli.getEncodeLength());

    ofstream output( "connect.txt", ios::out | ios::binary );
    if (!output) {
        cerr << "Open output file error!" << endl;
        exit(-1);
    }
    output.write((const char*)cli.getEncodeBuffer(), cli.getEncodeLength());
    output.flush();
    output.close();

    return a.exec();
}
