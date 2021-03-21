#include <iostream>
#include <stdexcept>

template<typename TIP>
class Iterator;

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
class DvostrukaLista : public Lista<TIP>{
    struct Cvor{
        TIP element;
        Cvor *nextp;
        Cvor *backp;
        Cvor(const TIP &el, Cvor *nextp, Cvor *backp=0) : element(el), nextp(nextp), backp(backp) {
            if(nextp!=0) nextp->backp=this;
            if(backp!=0) backp->nextp=this;
        }
    };
    int lduzina, dduzina;
    Cvor *poc;
    Cvor *kr;
    Cvor *curr;
public:
    DvostrukaLista() : lduzina(0), dduzina(0), poc(0), kr(0), curr(0) {}
    ~DvostrukaLista() ;
    DvostrukaLista(const DvostrukaLista<TIP> &n);
    DvostrukaLista &operator =(const DvostrukaLista<TIP> &n);
    int brojElemenata() const override { return lduzina+dduzina; }
    TIP& trenutni() override { return curr->element; }
    TIP trenutni() const override { return curr->element; }
    bool prethodni() override;
    bool sljedeci() override ;
    void pocetak() override ;
    void kraj() override ;
    void dodajIspred(const TIP &el) override ;
    void dodajIza(const TIP &el) override ;
    void obrisi() override ;
    TIP operator[](int index) const override;
    TIP &operator[](int index) override ;
    friend class Iterator<TIP>;
};
template<typename TIP>
DvostrukaLista<TIP>::DvostrukaLista(const DvostrukaLista<TIP> &n){
    poc=kr=curr=0;
    lduzina=dduzina=0;
    Cvor *p = n.poc;
    while( (lduzina+dduzina) != n.brojElemenata() ){
        dodajIza(p->element);
        if((lduzina+dduzina)!=1) sljedeci();
        p=p->nextp;
    }
    curr=n.curr;
}
template<typename TIP>
DvostrukaLista<TIP>& DvostrukaLista<TIP>::operator =(const DvostrukaLista<TIP> &n){
    if(&n!=this){
        while(poc!=0){
            curr=poc;
            poc=poc->nextp;
            delete curr;
        }
        poc=kr=curr=0;
        lduzina=dduzina=0;
        Cvor *p = n.poc;        
        while( (lduzina+dduzina) != n.brojElemenata() ){
            dodajIza(p->element);
            if((lduzina+dduzina)!=1) sljedeci();
            p=p->nextp;
        }
        curr=n.curr;    
    }
    return *this;
}
template<typename TIP>
DvostrukaLista<TIP>::~DvostrukaLista() {
    while(poc!=0){
        curr=poc;
        poc=poc->nextp;
        delete curr;
    }
}
template<typename TIP>
bool DvostrukaLista<TIP>::prethodni() {
    if(lduzina==0||dduzina+lduzina==0) return false;
    else if(lduzina==1) {
        curr = poc;
        lduzina--;
        dduzina++;
        return true;
    }    
    curr=curr->backp;
    lduzina--;
    dduzina++;
    return true;
}
template<typename TIP>
bool DvostrukaLista<TIP>::sljedeci() {
    if(dduzina==1||dduzina+lduzina==0) return false;
    curr=curr->nextp;
    lduzina++;
    dduzina--;
    return true;
}
template<typename TIP>
void DvostrukaLista<TIP>::pocetak() {
    curr=poc;
    dduzina+=lduzina;
    lduzina=0;
}
template<typename TIP>
void DvostrukaLista<TIP>::kraj() {
    curr=kr;
    lduzina+=dduzina;
    lduzina-=1;
    dduzina=1;
}
template<typename TIP>
void DvostrukaLista<TIP>::dodajIspred(const TIP &el) {
    if(brojElemenata()==0){
        Cvor *novi(new Cvor(el,0,0));
        poc=kr=curr=novi;
        dduzina++;
        return;
    }
    else if(curr==poc){
        Cvor *novi(new Cvor(el,curr,0));
        curr->backp=novi;
        poc=novi;
        lduzina++;
        return;
    }
    else{
        Cvor *novi(new Cvor(el,0,0));
        curr->backp->nextp=novi;
        novi->backp=curr->backp;
        curr->backp=novi;
        novi->nextp=curr;
        lduzina++;
        
    }
}
template<typename TIP>
void DvostrukaLista<TIP>::dodajIza(const TIP &el) {
    if(brojElemenata()==0){
        Cvor *novi(new Cvor(el,0,0));
        poc=kr=curr=novi;
        dduzina++;
        return;
    }
    else if(dduzina==1){
        Cvor *novi(new Cvor(el, 0, curr));
        curr->nextp=novi;
        kr=novi;
        dduzina++;
        return;
    }
    else{
        Cvor *novi(new Cvor(el,curr->nextp,curr));
        curr->nextp->backp=novi;
        curr->nextp=novi;
        dduzina++;
        return;
    }
}
template<typename TIP>
void DvostrukaLista<TIP>::obrisi() {
    if(dduzina==1 && lduzina==0){
        delete poc;
        curr=poc=kr=0;
        dduzina--;
    }
    else if(poc==curr){
        curr=curr->nextp;
        curr->backp=0;
        delete poc;
        poc=curr;
        dduzina--;
    }
    else if(kr==curr){
        kr=curr->backp;
        kr->nextp=0;
        delete curr;
        curr=kr;
        lduzina--;
    }
    else{
        Cvor *p(curr->nextp);
        curr->nextp->backp=curr->backp;
        curr->backp->nextp=curr->nextp;
        delete curr;
        curr=p;
        dduzina--;
    }
}
template<typename TIP>
TIP DvostrukaLista<TIP>::operator[](int index) const {
    if(index==0) return poc->element;
    if(index==brojElemenata()) return kr->element;
    Cvor *p(poc);
    while(index!=0){
        p=p->nextp;
        index--;
    }
    return p->element;
}
template<typename TIP>
TIP& DvostrukaLista<TIP>::operator[](int index) {
    if(index==0) return poc->element;
    if(index==brojElemenata()) return kr->element;
    Cvor *p(poc);
    while(index!=0){
        p=p->nextp;
        index--;
    }
    return p->element;
}

//Iterator clasa
template<typename TIP>
class Iterator{
    DvostrukaLista<TIP> *lista;
    typename DvostrukaLista<TIP>::Cvor *trenutniLista;
public:
    Iterator(const DvostrukaLista<TIP> &n);
    Iterator(const Lista<TIP> &nlist);
    TIP trenutni();
    bool sljedeci();
};

template<typename TIP>
Iterator<TIP>::Iterator(const DvostrukaLista<TIP> &n){
    lista = &n;
    trenutniLista = &n.poc;
}
template<typename TIP>
Iterator<TIP>::Iterator(const Lista<TIP> &nlist){
    lista = (DvostrukaLista<TIP>*)(&nlist);
    trenutniLista = lista->poc;
}
template<typename TIP>
TIP Iterator<TIP>::trenutni(){
    return trenutniLista->element;
}
template<typename TIP>
bool Iterator<TIP>::sljedeci(){
    if(trenutniLista->nextp!=nullptr){
        trenutniLista=trenutniLista->nextp;
        return true;
    }
    return false;
}

//dajMaksimum funkcija
template<typename TIP>
TIP dajMaksimum(const Lista<TIP> &n){
    Iterator<TIP> it(n);
    TIP max(n[0]);
    while(it.sljedeci()){
        if(it.trenutni()>=max) max=it.trenutni();
    }
    return max;
}

//Unit test
bool testKonstruktor(){
    DvostrukaLista<int> lista;
    if(lista.brojElemenata()==0) return true;
    else return false;
}
bool testTrenutni(){
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    lista.dodajIspred(3);
    lista.dodajIza(2);
    if(lista.trenutni()==1) return true;
    return false;
}
bool testConstTrenutni(){
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    const int broj(lista.trenutni());
    lista.trenutni()=5;
    lista.dodajIspred(3);
    lista.dodajIza(2);
    if(broj==1 && lista.trenutni()==5) return true;
    return false;   
}
bool testSljedeci(){
    bool greska=true;
    DvostrukaLista<int> lista;
    if(lista.sljedeci()) greska=false;
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.sljedeci();
    if(lista.trenutni()==2 && greska==true) return true;
    return false;
}
bool testPrethodni(){
    bool greska=true;
    DvostrukaLista<int> lista;
    if(lista.prethodni()) greska=false;
    lista.dodajIspred(1);
    lista.dodajIspred(2);
    lista.prethodni();
    if(lista.trenutni()==2 && greska==true) return true;
    return false;
}
bool testPocetak(){
    DvostrukaLista<int> l;
    l.dodajIza(1);
    l.dodajIza(2);
    l.dodajIza(3);
    l.sljedeci(); l.sljedeci();
    int broj = l.trenutni();
    l.pocetak();
    if(broj==2 && l.trenutni()==1) return true;
    return false;
}
bool testDodajIza(){
    DvostrukaLista<int> l;
    l.dodajIza(1);
    int broj1 = l.trenutni();
    l.dodajIza(2);
    int broj2 = l.trenutni();
    l.dodajIza(3);
    l.sljedeci();
    int broj3 = l.trenutni();
    if(broj1==broj2 && broj3 == 3) return true;
    return false;
}
bool testDodajIspred(){
    DvostrukaLista<int> l;
    l.dodajIspred(1);
    int broj1=l.trenutni();
    l.dodajIspred(2);
    int broj2 = l.trenutni();
    l.dodajIspred(3);
    l.prethodni();
    int broj3 = l.trenutni();
    if(broj1 == broj2 && broj3==3) return true;
    return false;
}
bool testObrisi(){
    DvostrukaLista<int> l;
    for(int i=1;i<=5;i++)
        l.dodajIza(i);
    l.sljedeci(); l.sljedeci();
    l.obrisi();
    if(l.trenutni()==3) return true;
    return false;
}
bool testOperatorUglaste(){
    DvostrukaLista<int> l;
    for(int i=0;i<5;i++)
        l.dodajIza(i);
    if(l[1]==4) return true;
    return false;
}
bool testKopirajuci(){
    bool flag = false;
    DvostrukaLista<int> dl1;
    for(int i=0;i<5;i++)
        dl1.dodajIspred(i);
    {
        DvostrukaLista<int> dl2(dl1);
        if(dl2[5]==0 && dl2[0]==1) flag=true;
    }
    if(dl1[0]==1 && flag) return true;
    return false;
}
bool testOperatorDodjele(){
    bool flag = false;
    DvostrukaLista<int> dl1;
    for(int i=0;i<5;i++)
        dl1.dodajIspred(i);
    {
        DvostrukaLista<int> dl2;
        for(int i=6;i<12;i++)
            dl2.dodajIspred(i);
        dl2=dl1;    
        if(dl2[5]==0 && dl2[0]==1) flag=true;
    }
    if(dl1[0]==1 && flag) return true;
    return false;    
}
void GlavnatestFunkcija(){
    std::cout<<"Test konstruktor: ";
    if(testKonstruktor()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test trenutni: ";
    if(testTrenutni()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test const trenutni: ";
    if(testConstTrenutni()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test sljedeci: ";
    if(testSljedeci()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test prethodni: ";
    if(testPrethodni()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test Pocetak: ";
    if(testPocetak()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test Dodaj Iza: ";
    if(testDodajIza()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test Dodaj Ispred: ";
    if(testDodajIspred()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test Obrisi: ";
    if(testObrisi()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test Operator[]: ";
    if(testOperatorUglaste()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test Kopirajuci: ";
    if(testKonstruktor()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;
    std::cout<<"Test Operator Dodjele: ";
    if(testOperatorDodjele()) std::cout<<"OK"<<std::endl;
        else std::cout<<"Bad"<<std::endl;         
}

int main (){
GlavnatestFunkcija();
    return 0;
}