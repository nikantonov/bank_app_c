#include<iostream>
#include<memory>
#include<vector>
using namespace std;
class Konto;
class Bank;
class Person: public enable_shared_from_this<Person>{
    string Name;
    vector<shared_ptr<Konto>> Konten; //beliebig viele mind. 1
public:
    Person(string);
    Person(string, vector<shared_ptr<Konto>>);
    bool konto_teilen(Konto&, Person&);
    bool neues_konto(int i);
    void kuendigen();
    string getName();
    shared_ptr<Person> getShared();
    bool addKonto(Konto&);
    vector<shared_ptr<Konto>> getKonten();
    ostream& print2(ostream&)const;
};

inline ostream& operator<<(ostream& o, const Person& k){
    return k.print2(o);
}


Person::Person(string Name){
    this->Name = Name;
}

string Person::getName(){
    return Name;
}

vector<shared_ptr<Konto>> Person::getKonten(){
    return Konten;
}

shared_ptr<Person> Person::getShared(){
    return shared_from_this();
}

Person::Person(string Name, vector<shared_ptr<Konto>>Konten){
    this->Name = Name;
    this->Konten = Konten;
}

bool Person::addKonto(Konto& k){
    Konten.push_back(k.getShared());
    k.setzeich(*this);
    return true;
}

bool Person::konto_teilen(Konto& k, Person& p){
    
    p.addKonto(k);
    return true;
}

void Konto::setzeich(Person& p){
    if(zeichnungsberechtigt.size() >= 10) throw runtime_error ("Nicht moeglich, Sie haben schon 10 Personen!");
    
    shared_ptr<Person> sp = p.shared_from_this();
    weak_ptr<Person> wp = sp;
    zeichnungsberechtigt.push_back(wp);
}

bool Person::neues_konto(int i){
    bool result = false;
    
    if (i == 1){
    shared_ptr<Girokonto> sp {new Girokonto};
        Konten.push_back(sp);
        sp->setzeich(*this);
    }else if(i == 2){
        shared_ptr<Businesskonto> sp{new Businesskonto};
        Konten.push_back(sp);
        sp->setzeich(*this);
    }
    else throw runtime_error("1 fur Girokonto, 2 fur Businesskonto");
    //Konten.push_back(sp);
    //sp->setzeich(*this);
    result = true;
    return result;
}

/*void Person::kuendigen(){
    for(auto& elem:Konten) {elem = nullptr; Konten.clear();}
    Bank::kuendigen(*this);
}*/

ostream& Person::print2(ostream& o)const{
    o<<"Name: "<< Name <<endl;
    for(auto& elem: Konten){
        if (elem != nullptr){
        o << "Kontonummer " << elem->kontonum() << endl;
        }
    }
    o << "Anzahl der Konten: " << Konten.size() << endl;
    return o;
}

ostream& Konto::print(ostream& o)const{
    o<<" Kontonummer: " << Kontonummer << endl;
    o<<" Kontostand: " << Kontostand << endl;
    o<<" Disporahmen: "<< Disporahmen <<endl;
    //o << " Gebuehren: " << Gebuehren << endl;
    o << "Zeichnungsberechtigen: "<<endl;
    for(auto& elem: zeichnungsberechtigt){
        auto y = elem.lock();
        o << y->getName() << endl;
    }
    o <<" Anzahl der berechtigte Personen:"<<zeichnungsberechtigt.size() << endl;
    
    return o;
}

ostream& Girokonto::print(ostream& o)const{
    Konto::print(o);
    o << " Gebuehren: " << Gebuehren << endl;
    return o;
}

ostream& Businesskonto::print(ostream& o)const{
    Konto::print(o);
    o << " Prozentsatz: " << Prozentsatz << endl;
    return o;
}



