#include <iostream>
#include <stdexcept>
#include <assert.h>

template<typename TIP>
class Lista{
public:
    Lista() {};
    virtual ~Lista() {};
    virtual int brojElemenata() const =0;
    virtual TIP& trenutni() =0;
    virtual TIP trenutni() const =0;
    virtual bool prethodni() =0;
    virtual bool sljedeci() =0;
    virtual void pocetak() =0;
    virtual void kraj() =0;
    virtual void obrisi() =0;
    virtual void dodajIspred(const TIP& el) =0;
    virtual void dodajIza(const TIP& el) =0;
    virtual TIP operator[](int index) const =0;
    virtual TIP &operator[](int index) =0;
};
template<typename TIP>
class NizLista : public Lista<TIP>{
protected:
    int kapacitet;
    int duzina;
    int tekuci;
    TIP* L;
    mutable int brojac[17]={};
public:
    NizLista(int size = 100000){
        brojac[0]++;
        kapacitet= size;
        duzina=0;
        tekuci=1;
        L=new TIP[kapacitet];}
    ~NizLista(){ brojac[1]++; delete[] L; }
    NizLista(const NizLista<TIP> &nova){
        brojac[2]++;
        kapacitet=nova.kapacitet;
        duzina=nova.duzina;
        tekuci=nova.tekuci;
        L=new TIP[kapacitet];
        for(int i(0);i<duzina;i++)
            L[i] = nova.L[i];
    }
    void operator = (const NizLista<TIP> &nova){
        brojac[3]++;
        if(&nova!=this){
            delete[] L;
            kapacitet=nova.kapacitet;
            L = new TIP[kapacitet];
            duzina=nova.duzina;
            tekuci=nova.tekuci;
            for(int i(0);i<duzina;i++)
                L[i] = nova.L[i];
        }
    }
    int brojElemenata() const override { brojac[4]++; return duzina;}
    TIP trenutni() const override {
        brojac[5]++;
        if(duzina==0) throw std::logic_error("Lista je prazna!");
        return L[tekuci-1];
    }
    TIP &trenutni() override {
        brojac[6]++;
        if(duzina==0) throw std::logic_error("Lista je prazna!");
        return L[tekuci-1];
    }
    bool sljedeci() override {
        brojac[7]++;
        if(duzina==0) throw std::logic_error("Lista je prazna!");
        if(tekuci==duzina) return false;
        tekuci++; return true;
    }
    bool prethodni() override{
        brojac[8]++;
        if(duzina==0) throw std::logic_error("Lista je prazna!");
        if(tekuci==0) return  false;
        tekuci--; return true;
    }
    void pocetak() override {
        brojac[9]++;
        if(duzina==0) throw std::logic_error("Lista je prazna!");
        tekuci=0;
    }
    void kraj() override {
        brojac[10]++;
        if(duzina==0) throw std::logic_error("Lista je prazna!");
        tekuci=duzina;
    }
    void obrisi() override {
        brojac[11]++;
        if(duzina==0) throw std::logic_error("Lista je prazna!");
        if(tekuci==duzina-1){tekuci--; duzina--;}
        else{
            for(int i=tekuci;i<duzina-1;i++)
                L[i]=L[i+1];
            duzina--;
            tekuci++;
        }    
    }
    void dodajIza(const TIP &el){
        brojac[12]++;
        if(duzina<kapacitet){
            if(duzina==0) {L[0]=el; duzina++; return;}
            for(int i(duzina);i>=tekuci;i--)
                L[i]=L[i-1];
            L[tekuci]=el;    
            duzina++;
            return;
        }
        else throw std::logic_error("Nedovoljno prostora!");    
    }
    void dodajIspred(const TIP &el){
        brojac[13]++;
        if(duzina<kapacitet){
            if(duzina==0){L[0]=el;duzina++;return;}
            for(int i=duzina;i>=tekuci;i--)
                L[i] = L[i-1];
            L[tekuci] = el;
            tekuci++;
            duzina++;
        }  
        else throw std::logic_error("Nedovoljno prostora!");    
    }
    TIP operator[](int index) const override {
        brojac[14]++;
        if(index<0 ||index > duzina) throw std::logic_error("Neispravan index!");
        return L[index];
    }
    TIP &operator[](int index) override {
        brojac[15]++;
        if(index<0 ||index > duzina) throw std::logic_error("Neispravan index!");
        return L[index];
    }
    void Ispisi() const {
        brojac[16]++;
        for(int i=0;i<duzina;i++)
            std::cout<<L[i]<<" ";
    }
};
template<typename TIP>
class JednostrukaLista : public Lista<TIP>{
    struct Cvor{
        TIP element;
        Cvor* sljedeci;
        Cvor(const TIP &element, Cvor* sljedeci) : element(element), sljedeci(sljedeci) {}
    };
    int lduzina, dduzina;
    Cvor* poc;
    Cvor* kr;
    Cvor* tekuci;
    mutable int brojac[18]={};
public:
    JednostrukaLista(): poc(0), kr(0), tekuci(0), lduzina(0), dduzina(0) {brojac[0]++;}
    void dodaj_na_kraj(const TIP &el){
        brojac[1]++;
        Cvor* novi= new TIP(el,nullptr);
        if(lduzina+dduzina==0){poc=kr=novi;}
        kr->sljedeci=novi;
        kr=novi;
    }
    JednostrukaLista(const TIP &lista): poc(0), kr(0), dduzina(0), lduzina(0){
        brojac[2]++;
        Cvor *p(lista.poc);
        while(p!=0){
            dodaj_na_kraj(p->element);
            p=p->sljedeci;
        }
    }
    ~JednostrukaLista(){
        brojac[3]++;
        while(poc!=0){
            tekuci = poc;
            poc= poc->sljedeci;
            delete tekuci;
        }
        poc=0; kr=0; tekuci=0;
    }
    TIP trenutni() const override {
        brojac[4]++;
        if(lduzina+dduzina==0) throw std::logic_error("Lista je prazna!");
        if(tekuci==0) return poc->element;
        return tekuci->sljedeci->element;
    }
    TIP &trenutni() override {
        brojac[5]++;
        if(lduzina+dduzina==0) throw std::logic_error("Lista je prazna!");
        if(tekuci==0) return poc->element;
        return tekuci->sljedeci->element;
    }
    bool prethodni() override {
        brojac[6]++;
        if(lduzina==0) return false;
        if(lduzina==1) {tekuci->sljedeci=nullptr; lduzina--; dduzina++; return true;}
        Cvor *p(poc);
        while(p->sljedeci!=tekuci)
            p=p->sljedeci;
        tekuci=p;
        lduzina--;
        dduzina++;
        return true;
    }
    bool sljedeci() override{
        brojac[7]++;
        if(lduzina==0) { tekuci=poc; lduzina++; dduzina--; return true;}
        if(dduzina==0) return false;
        tekuci=tekuci->sljedeci;
        lduzina++;
        dduzina--;
        return true;
    }
    void pocetak() override {
        brojac[8]++;
        if(lduzina+dduzina==0) throw std::logic_error("Lista je prazna!");
        tekuci=nullptr;
        dduzina+=lduzina;
        lduzina=0;
    }
    void kraj() override {
        brojac[9]++;
        if(lduzina+dduzina==0) throw std::logic_error("Lista je prazna!");
        tekuci=kr;
        lduzina+=dduzina;
        dduzina=0;
    }
    void obrisi() override {
        brojac[10]++;
        if(lduzina+dduzina==0) throw std::logic_error("Lista je prazna!");
        Cvor* temp;
        if(lduzina==0){
            temp=poc;
            poc=temp->sljedeci;
        }
        else{
            temp=tekuci->sljedeci;
            tekuci->sljedeci=temp->sljedeci;
        }
        if(dduzina==1) kr=tekuci;
        delete temp;
        dduzina--;
    }
    void dodajIspred(const TIP &el) override{
        brojac[11]++;
        Cvor* p= new Cvor(el, 0);
        if(lduzina==0){
            p->sljedeci=poc;
            poc=p;
            if(lduzina+dduzina==0) 
                kr=p;
        }
        else{
            p->sljedeci=tekuci->sljedeci;
            tekuci->sljedeci=p;
            if(lduzina==0)
                kr=tekuci->sljedeci;
        }
        dduzina++;
    }
    void dodajIza(const TIP &el) override{
        brojac[12]++;
        Cvor *novi= new Cvor(el,0);
        if(lduzina==0){
            novi->sljedeci=poc;
            poc=novi;
            dduzina++;
            return;
        }
        if(dduzina==0){  kr->sljedeci=novi; lduzina++; return;}
        novi->sljedeci=tekuci->sljedeci->sljedeci;
        tekuci->sljedeci=novi;
        dduzina++;
    }
    TIP idi_na(int index) const {
        brojac[13]++;
        Cvor* p(poc);
        while(index!=0){
            p=p->sljedeci;
            index--;
        }
        return p->element;
    }
    TIP &idi_na(int index) {
        brojac[14]++;
        Cvor* p(poc);
        while(index!=0){
            p=p->sljedeci;
            index--;
        }
        return p->element;
    }
    TIP operator[](int index) const { brojac[15]++;  if(index<0 || index>lduzina+dduzina) throw std::logic_error("Neispravan index");
        return idi_na(index);}
    TIP &operator[](int index){ brojac[16]++; if(index<0 || index>lduzina+dduzina) throw std::logic_error("Neispravan index");
        return idi_na(index);}  
    int brojElemenata() const override{
        brojac[17]++;
        return dduzina + lduzina;
    }    
};
    void testOperatorIndexiranja() {
        NizLista<int> l;
        for(int i(0);i<5;i++)
            l.dodajIza(i);
        std::cout<<"Operator []: ";
        if(l[1]==4) {std::cout<<"OK"<<std::endl; return;}
        else std::cout<<"WRONG"<<std::endl;
    }
    void testOperatorIndexRef(){
        NizLista<int> l;
        for(int i(0);i<5;i++)
            l.dodajIza(i);
        std::cout<<"Operator[] referenca: ";
        int& x=l[1];
        x=5;
        if(l[1]==x) {std::cout<<"OK"<<std::endl; return;}
        else std::cout<<"WRONG"<<std::endl;
    }
    void testBrojElemenata(){
        NizLista<int> l;
        for(int i(0);i<5;i++)
            l.dodajIza(i);    
        l.kraj();
        std::cout<<"Broj elemenata: ";
        if(l.trenutni() == l[l.brojElemenata()-1]) {std::cout<<"OK"<<std::endl; return;}
        std::cout<<"BAD"<<std::endl;
    }
    void testDodajIspred(){
        NizLista<int> l;
        for(int i(0);i<5;i++)
            l.dodajIspred(i);
        std::cout<<"Dodaj ispred: ";
        if(l[3]==3) {std::cout<<"OK"<<std::endl; return;}
        else std::cout<<"WRONG"<<std::endl;
    }
    void testDodajIza(){
        NizLista<int> l;
        for(int i(0);i<5;i++)
            l.dodajIza(i);
        std::cout<<"Dodaj iza: ";
        if(l[3]==2) {std::cout<<"OK"<<std::endl; return;}
        else std::cout<<"WRONG"<<std::endl;
    }
    void testObrisi(){
        std::cout<<"Obrisi: ";
        NizLista<int> l;
        l.dodajIspred(1);
        l.dodajIspred(2);
        l.obrisi();
        l.obrisi();
        if(l.brojElemenata()==0) {std::cout<<"OK"<<std::endl; return;}
        else std::cout<<"WRONG"<<std::endl;
    }
    void testPrethodni(){
        std::cout<<"Prethodni: ";
        NizLista<int> l;
        l.dodajIspred(1);
        l.dodajIspred(2);
        l.prethodni();
        if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
        else std::cout<<"WRONG"<<std::endl;
    }
    void testSljedeci(){
        std::cout<<"Sljedeci: ";
        NizLista<int> l;
        l.dodajIza(1);
        l.dodajIza(2);
        l.sljedeci();
        if(l.trenutni()==2) std::cout<<"OK"<<std::endl;
        else std::cout<<"WRONG"<<std::endl;
    }
    void testOperatorDodjele(){
        std::cout<<"Operator dodjele: ";
        NizLista<int> l;
        for(int i(0);i<5;i++)
            l.dodajIza(i);
        NizLista<int> k;
        k=l;
        if(k[1]==l[1]) std::cout<<"OK"<<std::endl;
        else std::cout<<"WRONG"<<std::endl;
    }
    void testKopirajuciKonst(){
        std::cout<<"Kopirajuci konstruktor: ";
        NizLista<int> l;
        for(int i(0);i<5;i++)
            l.dodajIza(i);        
        NizLista<int> b(l);
        b.obrisi();
        if(l.trenutni()!=b.trenutni()) std::cout<<"OK"<<std::endl;
        else std::cout<<"WRONG"<<std::endl;
    }
void testJPOperatorIndexiranja(){
        JednostrukaLista<int> l;
        for(int i(0);i<5;i++)
            l.dodajIza(i);
        std::cout<<"Operator []: ";
        if(l[1]==4) {std::cout<<"OK"<<std::endl; return;}
        else std::cout<<"WRONG"<<std::endl;
    }
void testJPBrojElemenata(){
    JednostrukaLista<int> l;
    for(int i(0);i<5;i++)
        l.dodajIza(i);    
    l.kraj();
    std::cout<<"Broj elemenata: ";
    if(l.trenutni() == l[l.brojElemenata()-1]) {std::cout<<"OK"<<std::endl; return;}
    std::cout<<"WRONG"<<std::endl;
} 
void testJPDodajIspred(){
    JednostrukaLista<int> l;
    for(int i(0);i<5;i++)
        l.dodajIspred(i);
    std::cout<<"Dodaj ispred: ";
    if(l.trenutni()==4) {std::cout<<"OK"<<std::endl; return;}
    else std::cout<<"WRONG"<<std::endl;
}
void testJPDodajIza(){
    JednostrukaLista<int> l;
    for(int i(0);i<5;i++)
        l.dodajIza(i);
    std::cout<<"Dodaj iza: ";
    if(l.trenutni()==0) {std::cout<<"OK"<<std::endl; return;}
    else std::cout<<"WRONG"<<std::endl;
}
void testJPObrisi(){
    std::cout<<"Obrisi: ";
    JednostrukaLista<int> l;
    l.dodajIspred(1);
    l.dodajIspred(2);
    l.obrisi();
    l.obrisi();
    if(l.brojElemenata()==0) {std::cout<<"OK"<<std::endl; return;}
    else std::cout<<"WRONG"<<std::endl;
}
void testJPPrethodni(){
    std::cout<<"Prethodni: ";
    JednostrukaLista<int> l;
    l.dodajIspred(1);
    l.dodajIspred(2);
    l.prethodni();
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"WRONG"<<std::endl;
}
void testJPSljedeci(){
    std::cout<<"Sljedeci: ";
    JednostrukaLista<int> l;
    l.dodajIza(1);
    l.dodajIza(2);
    l.sljedeci();
    if(l.trenutni()==1) std::cout<<"OK"<<std::endl;
    else std::cout<<"WRONG"<<std::endl;
}
void testJPOperatorDodjele(){
    std::cout<<"Operator dodjele: ";
    JednostrukaLista<int> l;
    for(int i(0);i<5;i++)
        l.dodajIza(i);
    JednostrukaLista<int> k;
    k=l;
    if(k[1]==l[1]) std::cout<<"OK"<<std::endl;
    else std::cout<<"WRONG"<<std::endl;
}
void testJPKopirajuciKonst(){
    std::cout<<"Kopirajuci konstruktor: ";
    JednostrukaLista<int> l;
    for(int i(0);i<5;i++)
        l.dodajIza(i);        
    JednostrukaLista<int> b(l);
    b.obrisi();
    if(l.trenutni()!=b.trenutni()) std::cout<<"OK"<<std::endl;
    else std::cout<<"WRONG"<<std::endl;
}
void TEST_NIZ_LISTA(){
    std::cout<<"Test NizLista......\n"<<std::endl;
    testBrojElemenata();
    testOperatorIndexiranja();
    testOperatorIndexRef();
    testDodajIspred();
    testObrisi();
    testDodajIza();
    testPrethodni();
    testSljedeci(); 
    testOperatorDodjele();
    testKopirajuciKonst();
    std::cout<<"...End of file..\n"<<std::endl;
}
void TEST_JP_LISTA(){
    std::cout<<"Test Jednostruka Lista......\n"<<std::endl;    
    testJPOperatorIndexiranja();
    testJPBrojElemenata();
    testJPDodajIspred();
    testJPDodajIza();
    testJPObrisi();
    testJPPrethodni();
    testJPSljedeci();
    testJPOperatorDodjele();
    testJPKopirajuciKonst();
    std::cout<<"...End of file..\n"<<std::endl;
}

int main(){
    TEST_NIZ_LISTA();
    TEST_JP_LISTA();
    return 0;
}