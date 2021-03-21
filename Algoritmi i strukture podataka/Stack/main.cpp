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

//unit testovi
bool testBrojElemenata(){
    Stek<int> k;
    int x1=k.brojElemenata();
    k.stavi(1);
    k.stavi(1);
    int x2=k.brojElemenata();
    k.skini();
    int x3=k.brojElemenata();
    if(x1==0 && x2==2 && x3==1) return true;
    return false;
}
bool testOperatorDodjele(){
    Stek<int> a;
    for(int i(0);i<3;i++)
        a.stavi(i);
    Stek<int> b;
        b.stavi(8);
    b=a;
    if(a.vrh()==b.vrh()) return true;
    return false;
}
bool testVrh(){
    Stek<int> a;
    a.stavi(1); a.stavi(2);
    if(a.vrh()==2) return true;
    return false;
}
bool testStavi(){
    Stek<std::string> a;
    a.stavi("abc");
    if(a.vrh()=="abc") return true;
    return false;
}
bool testSkini(){
    Stek<double> a;
    a.stavi(1); a.skini();
    a.stavi(2); a.skini();
    a.stavi(3); a.vrh(); a.skini();
    if(a.brojElemenata()==0) return true;
    return false;
}
bool testBrisi(){
    Stek<std::string> a;
    a.stavi("Danas");
    a.stavi(" je ");
    a.stavi("suncano");
    a.brisi();
    if(a.brojElemenata()==0) return true;
    return false;
}
bool testDaLiJeprazan(){
    Stek<int> a;
    int x1=a.DaLiJeprazan();
    a.stavi(1);
    int x2=a.DaLiJeprazan();
    if(x1==0 && x2==1) return true;
    return false;
}
bool testKopirajuciKonst(){
    Stek<int> a;
    for(int i(5);i<=10;i++)
        a.stavi(i);
    Stek<int> b(a);
    a.skini();
    if(b.vrh()==10) return true;
    return false;
}
void TEST_STEK(){
    std::cout<<"...Test metoda za Stek...\n"<<std::endl;
    if(testBrojElemenata()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testOperatorDodjele()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testVrh()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testStavi()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testSkini()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testBrisi()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testDaLiJeprazan()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testKopirajuciKonst()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"...End of testing file...."<<std::endl;
}


int main(){
    TEST_STEK();
    Stek<int> a;
    a.stavi(5);
    std::cout<<a.skini();
    return 0;
}