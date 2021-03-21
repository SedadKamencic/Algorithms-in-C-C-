#include <iostream>
#include <stdexcept>

#ifndef LISTA
#define LISTA

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
    void obrisi(const TIP &kljuc);
    TIP operator[](int index) const override;
    //TIP &operator[](int index) override ;
    TIP &operator[](int index) override;
    friend class Iterator<TIP>;
};
template<typename TIP>
TIP& DvostrukaLista<TIP>::operator[](int kljuc){
    Cvor *p(poc);
    while(p){
        if(p->element == kljuc){
            return p->element;
        }
        p = p->nextp;
    }
    throw std::domain_error("Catch!");
    return p->element;
}
template<typename TIP>
void DvostrukaLista<TIP>::obrisi(const TIP &kljuc){
    int i(brojElemenata());
    pocetak();
    while(i>0){
        if(trenutni() == kljuc)
            obrisi();
        i--;    
    }    
}

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
/*
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
}*/

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
#endif