#include <string>
#include <iostream>
#include "Person.pb.h"
#include "AddressBook.pb.h"

using namespace std;

int main()
{
    string data;

//    //encode
//    {
//        ProtobDemo::Person p;
//        p.set_name("Hideto");
//        p.set_id(123);
//        p.set_email("hideto.bj@gmail.com");
//        p.SerializeToString(&data);
//    }

//    //decode
//    {
//        ProtobDemo::Person p;
//        p.ParseFromString(data);
//        cout << "People: " << endl;
//        cout << "Name: " << p.name() << endl;
//        cout << "ID: " << p.id() << endl;
//        cout << "Email: " << p.email() << endl;
//    }

    //encode
    {
        tutorial::AddressBook ab;

        {
            auto p1 = ab.add_person();
            p1->set_name("zhangsan");
            p1->set_id(1);
            p1->set_email("zhangsan@gmail.com");
            auto phone = p1->add_phone();
            phone->set_number("123456789");
            phone->set_type(tutorial::Person::MOBILE);
        }

        {
            auto p2 = ab.add_person();
            p2->set_name("lisi");
            p2->set_id(6);
            auto phone1 = p2->add_phone();
            phone1->set_number("400023456");
            auto phone2 = p2->add_phone();
            phone2->set_number("90015678");
        }

        data.clear();
        ab.SerializeToString(&data);
    }

    //decode
    {
        tutorial::AddressBook ab;
        ab.ParseFromString(data);
        for (int i = 0; i < ab.person_size(); ++i) {
            auto person = ab.person(i);

            cout << "name:" << person.name() << endl;
            cout << "id:" << person.id() << endl;
            if (person.has_email())
                cout << "email:" << person.email() << endl;

            for (int i = 0; i < person.phone_size(); ++i) {
                auto phone = person.phone(i);
                cout << "phone num:" << phone.number() << endl;
                if (phone.has_type())
                    cout << "tpye:" << phone.type() << endl;
            }
        }
    }

  return 0;
}
