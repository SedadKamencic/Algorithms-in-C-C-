
#include <iostream>
#include <stdexcept>

template <typename InfoTip>
class iStek {
private:
 void operator =(const iStek&); // Zaštita za dodjelu
 iStek(const iStek&); // Zaštita za konstruktor kopije
public:
 iStek() {} // Podrazumijevani konstruktor
 virtual ~iStek() {} // Bazni destruktor
 virtual void brisi() = 0; // Reinicijalizacija steka
 virtual void stavi(const InfoTip&) = 0; // Stavljanje na stek
 virtual InfoTip skini() = 0; // Skidanje sa steka
 virtual InfoTip& vrh() const = 0; // Vraća vršni el.
 virtual int brojElemenata() const = 0; // Vraća dužinu steka
};



template <typename InfoTip>
class Stek : public iStek<InfoTip> {
private:
 struct Cvor { // Čvor
 InfoTip info; // Informacioni sadržaj
 Cvor *sljedeci; // Pokazivač na sljedbenika
 Cvor (const InfoTip &info, Cvor *sljedeci): // Konstruktor za čvor
 info(info), sljedeci(sljedeci) {}
 };
 Cvor * top; // Vrh steka
 int velicina; // Trenutna veličina steka
 void Kopiraj(const Stek &s); // Kopiranje steka
public:
 Stek() : top(0),velicina(0) {} // Konstruktor steka
 Stek(const Stek &s) { Kopiraj(s); } // Konstruktor kopije
 ~Stek() { brisi(); }
 Stek &operator =(const Stek &s); // Preklopljeni op. dodjele
 void stavi(const InfoTip &info){ // Stavljanje na stek
 top = new Cvor(info, top); velicina++;
 }
 InfoTip skini() ; // Skidanje sa steka
 InfoTip& vrh() const ; // Element na vrhu
 void brisi() ; // Uništavanje steka
 bool JeLiPrazan() const { return top == 0; } // Je li prazan
 int brojElemenata() const { return velicina; } // Dužina steka
 void Prikazi() const; // Prikaz sadržaja
};

template <typename InfoTip> // Kopiranje steka
void Stek<InfoTip>::Kopiraj(const Stek &s) {
 Cvor *q, *p(s.top);
 top = 0;
 velicina = s.velicina;
 while(p != 0) {
 Cvor *n(new Cvor(p->info,0));
 if (top == 0)
 top = n;
 else
 q->sljedeci = n;
 p = p->sljedeci;
 q = n;
 }
}
template <typename InfoTip> // Preklopljeni op. dodjele
Stek<InfoTip> &Stek<InfoTip>::operator =(const Stek &s) {
 if (&s == this)
 return *this;
 brisi();
 Kopiraj(s);
 return *this;
}
template <typename InfoTip> // Skidanje sa steka
InfoTip Stek<InfoTip>::skini() {
 if (top == 0)
 throw "\nStek je prazan!\n";
 InfoTip info(top->info);
 Cvor *p = top->sljedeci;
 delete top;
 top = p;
 velicina--;
 return info;
}
template <typename InfoTip> // Uništavanje steka
void Stek<InfoTip>::brisi() {
 while(!JeLiPrazan())
 skini();
}
template <typename InfoTip> // Vršni element steka
InfoTip& Stek<InfoTip>::vrh() const {
 if (top == 0)
 throw "Stek je prazan!\n";
 return top->info;
}
template <typename InfoTip> // Prikaz sadržaja steka
void Stek<InfoTip>::Prikazi() const {
 Cvor *p(top);
 std::cout << "\nSadrzaj steka:\n";
 while (p != 0) {
 std::cout << p->info << std::endl;
 p = p->sljedeci;
 }
}

int main() {
Stek<int> s;
for (int i(0); i<5; i++)
s.stavi(i);
std::cout << s.vrh()<<" ";
s.vrh()++;
std::cout << s.vrh(); 
    return 0;
}
