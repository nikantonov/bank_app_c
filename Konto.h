#include<iostream>
#include<vector> 
#include<memory>
using namespace std;
class Person;
class Konto: public enable_shared_from_this<Konto>{
 string Kontonummer;
 int Kontostand;
 int Disporahmen;
 vector<std::weak_ptr<Person>> zeichnungsberechtigt; //max 10 mind. 1
 //int Gebuehren;
public:
    Konto();
    void einzahlen (unsigned);
    bool auszahlen(unsigned);
    bool ueberweisen(unsigned, Konto&);
    void setzeich(Person&);
    string kontonum();
    shared_ptr<Konto> getShared();
    virtual ostream& print (ostream&)const ;
    virtual void Gebuehren_rechnen(int i)=0;
    
    void Kontostand_setzen1(int i){
        if ((Kontostand - i) >= Disporahmen){Kontostand = Kontostand - i;}
        else
            throw runtime_error("Sie haben zu wenig Geld dafur!");
    }
};

class Girokonto: public Konto{
    int Gebuehren;
public:
    Girokonto():Konto(){
        //if(i<= 0) throw runtime_error("Prozentsatz kann nicht gleich oder kleiner als 0 sein");
        Gebuehren = 10;
    }

    Girokonto(int i):Konto(){
        if(i<= 0) throw runtime_error("Prozentsatz kann nicht gleich oder kleiner als 0 sein");
        Gebuehren = i;
    }
    
    void Gebuehren_rechnen(int i) override{
        Kontostand_setzen1(Gebuehren);
    }
    
    ostream& print(ostream&)const;
};

class Businesskonto: public Konto{
    int Prozentsatz;
public:
    Businesskonto():Konto(){
        //if(i<= 0) throw runtime_error("Prozentsatz kann nicht gleich oder kleiner als 0 sein");
        //if(i > 100) throw runtime_error("Prozentsatz kann nicht > 100 sein");
        
        Prozentsatz = 10;
    }
    Businesskonto(int i):Konto(){
        if(i<= 0) throw runtime_error("Prozentsatz kann nicht gleich oder kleiner als 0 sein");
        if(i > 100) throw runtime_error("Prozentsatz kann nicht > 100 sein");

        Prozentsatz = i;
    }
    
    void Gebuehren_rechnen(int i) override{
        double Prozent = (double)i/100*Prozentsatz;
        Kontostand_setzen1(Prozent);
    }
    
   
    
    
    ostream& print(ostream&)const;
};

inline ostream& operator <<(ostream& o, const Konto& k){
    return k.print(o);
}

static int N = 0;

Konto::Konto(){
    Kontostand = 0;
    Disporahmen = 0;
    //Gebuehren = 0;
    Kontonummer = "Nummer_"+to_string(N++);
}

void Konto::einzahlen (unsigned betrag){
    if ((int)betrag <= 0) {cout << "Es gibt nichts einzuzahlen!"<<endl;return;}
    Kontostand = Kontostand + betrag;
}

bool Konto::auszahlen(unsigned betrag){
    int f = Kontostand - betrag;
    if(f < Disporahmen) throw runtime_error("Nicht Moeglich");
    Gebuehren_rechnen(betrag);
    Kontostand = Kontostand - betrag;
    return true;
    /*if ((int)betrag <= 0) {cout << "Es gibt nichts auszuzahlen!"<<endl;return false;}
    
    if ((int)(Kontostand - betrag) < Disporahmen){cout << "Sie haben zu wenig Geld!"<<endl;return false;}
    else{
        Gebuehren_rechnen(betrag);
        Kontostand = Kontostand - betrag;
        //ziel.einzahlen(betrag);
        return true;
    }*/
}



bool Konto::ueberweisen(unsigned betrag, Konto& ziel){
    if ((int)betrag <= 0) {cout << "Es gibt nichts zu ueberweisen!"<<endl;return false;}
    
    if ((int)(Kontostand - betrag) < Disporahmen){cout << "Sie haben zu wenig Geld!"<<endl;return false;}
    else{
        Gebuehren_rechnen(betrag);
        Kontostand = Kontostand - betrag;
        ziel.einzahlen(betrag);
        return true;
    }
}

shared_ptr<Konto> Konto::getShared(){
    return shared_from_this();
}

string Konto::kontonum(){
    return Kontonummer;
}




/*ostream& Konto::print(ostream& o)const{
    o<<" Kontonummer: " << Kontonummer << endl;
    o<<" Kontostand: " << Kontostand << endl;
    o<<" Disporahmen: "<< Disporahmen <<endl;
    o << " Gebuehren: " << Gebuehren << endl;
    
    for(auto& elem: zeichnungsberechtigt){
        auto y = elem.lock();
        o << y->getName() << endl;
    }
    o <<" Anzahl der berechtigte Personen:"<<zeichnungsberechtigt.size() << endl;
    
    return o;
}*/


