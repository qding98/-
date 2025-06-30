#ifndef MANA_PERSON_H
#define MANA_PERSON_H
#include<string>
using namespace std;
class Person {
public:
    Person(string name, string gender)
        : name(name), gender(gender) {}
    
    virtual ~Person() {}
    
    string getName() const { return name; }
    string getGender() const { return gender; }
    void setName(const string& name) { this->name = name; }
    void setGender(const string &newgender) {
        if (newgender == "男" || newgender == "女") {
            gender = newgender;
        } else {
            cout << "性别输入不合法，请重新输入。" << endl;
        }
    }
protected:
    string name;
    string gender;
};
#endif
