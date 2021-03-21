#ifndef STEK_H
#define STEK_H

#include <iostream>
#include <stdexcept>
#include <string>

template<typename TIP>
class iStek{
    iStek(const iStek &novi) =delete;
    void operator =(const iStek &novi) =delete;
public:
    iStek(){};
    virtual ~iStek(){} ;
    virtual void brisi() =0;
    virtual void stavi(const TIP &el) =0;
    virtual TIP skini() =0;
    virtual TIP& vrh() const =0;
    virtual int brojElemenata() const =0;
};

template<typename TIP>
class Stek : public iStek<TIP>{
    struct Cvor{
        TIP element;
        Cvor* sljedeci;
        Cvor(const TIP &element, Cvor *sljedeci) : element(element), sljedeci(sljedeci) {};
    };
    Cvor* top;
    int duzina;
    void kopiraj(const Stek &novi);
public:
    Stek() : top(0), duzina(0) {};
    TIP skini() override;
    ~Stek() { brisi(); }
    Stek(const Stek &novi) { kopiraj(novi); };
    Stek &operator =(const Stek &novi);
    void brisi() override;
    TIP& vrh() const override;
    void stavi(const TIP &el) override;
    bool DaLiJeprazan() const {return top;}
    int brojElemenata() const override { return duzina; } 
};
template<typename TIP>
void Stek<TIP>::brisi() {
    while(DaLiJeprazan())
        skini();
}
template<typename TIP>
TIP Stek<TIP>::skini() {
    if(top==0) throw std::logic_error("Stek je prazan, nema sta za skinuti");
    TIP x= top->element;
    Cvor* p(top);
    top=top->sljedeci;
    delete p;
    duzina--;
    return x;
}
template<typename TIP>
void Stek<TIP>::stavi(const TIP &el) {
    Cvor *novi = new Cvor(el,top);
    top=novi;
    duzina++;
}
template<typename TIP>
TIP& Stek<TIP>::vrh() const{
    if(!brojElemenata()) throw std::logic_error("Nema elemenata za vracanje");
    return top->element;
}
template<typename TIP>
void Stek<TIP>::kopiraj(const Stek &novi){
    top=0;
    duzina=novi.duzina;
    Cvor *q, *p(novi.top);
    while(p!=0){
        Cvor *n(new Cvor(p->element,0));
        if(top==0)
            top = n;
        else
            q->sljedeci = n;
            p=p->sljedeci;
            q=n;
         
    }
}
template<typename TIP>
Stek<TIP> &Stek<TIP>::operator =(const Stek &novi){
    if(&novi!=this){
        brisi();
        kopiraj(novi);
    }
    return *this;
}
#endif