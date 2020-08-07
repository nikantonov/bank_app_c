#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
using namespace std;
class Konto;
class Person;
class Bank{
    string Name;
    vector<shared_ptr<Person>> Kunden;
public:
    Bank(string);
    void neuerKunde(string,int);
    ostream& print2(ostream&)const;
    void kuendigen(Person&);
    vector<shared_ptr<Person>> getKunden();

};

inline ostream& operator <<(ostream& o, const Bank& k){
    return k.print2(o);
}


Bank::Bank(string Name){
    this->Name = Name;
}

void Person::kuendigen(){
    for(auto& elem:Konten) {elem = nullptr; Konten.clear();}
    //Bank::kuendigen(*this);
}



void Bank::neuerKunde(string name, int i){
    shared_ptr<Person> pt {new Person(name)};
    Kunden.push_back(pt);
    pt->neues_konto(i);
}

void Bank::kuendigen(Person& p){
    p.kuendigen();
    for(auto& k: Kunden){
        if(k->getName() == p.getName()){
            k = nullptr;
        }
    }
    auto i = find(Kunden.begin(), Kunden.end(), nullptr);
    if (i != Kunden.end()) Kunden.erase(i);
  
}

vector<shared_ptr<Person>> Bank::getKunden(){
    return Kunden;
}

ostream& Bank::print2(ostream& o)const{
    o<<"Name: "<< Name <<endl;
    //o << "Kunden: " << endl;
    o << "Anzahl der Kunden " << Kunden.size() << endl;
    for(auto& elem: Kunden){
        elem->print2(o);
    }
    return o;
}

