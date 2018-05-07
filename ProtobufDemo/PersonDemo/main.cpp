#include <string>
#include <iostream>
#include "Person.pb.h"

using namespace std;

int main()
{
    string data;

    //encode
    {
        ProtobDemo::Person p;
        p.set_name("Hideto");
        p.set_id(123);
        p.set_email("hideto.bj@gmail.com");
        p.SerializeToString(&data);
    }

    //decode
    {
        ProtobDemo::Person p;
        p.ParseFromString(data);
        cout << "People: " << endl;
        cout << "Name: " << p.name() << endl;
        cout << "ID: " << p.id() << endl;
        cout << "Email: " << p.email() << endl;
    }

  return 0;
}
