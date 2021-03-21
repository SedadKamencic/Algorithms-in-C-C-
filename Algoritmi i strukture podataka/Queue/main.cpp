#include <iostream>
#include <stdexcept>
#include <cmath>

template<typename TIP>
class Red {
    struct Cvor{
        TIP element;
        Cvor* sljedeci=0;
        Cvor(const TIP &el, Cvor* sljedeci) : element(el), sljedeci(sljedeci){};
    };    
    Cvor* poc;
    Cvor* kraj;
    int duzina;
    void kopiraj(const Red &novi);
public:
    Red():poc(0),kraj(0),duzina(0) {};
    ~Red(){ brisi(); }
    Red(const Red &novi) { kopiraj(novi); }
    Red& operator =(const Red &novi);
    void brisi();
    void stavi(const TIP& el);
    TIP skini();
    TIP& celo() const;
    int brojElemenata() const {return duzina;}
    bool DaLiJePrazan() const {return poc;}
};
template<typename TIP>
Red<TIP>& Red<TIP>::operator =(const Red<TIP> &novi){
    if(&novi!=this){
        brisi();
        kopiraj(novi);
    }
    return *this;
}
template<typename TIP>
void Red<TIP>::brisi() {
    while(DaLiJePrazan())
        skini();
}
template<typename TIP>
void Red<TIP>::stavi(const TIP& el){
    Cvor* p(new Cvor(el,0));
    if(poc==0)  
        poc=kraj=p;
    else{
        kraj->sljedeci=p;
        kraj=kraj->sljedeci;
    }
    duzina++;
}
template<typename TIP>
TIP Red<TIP>::skini(){
    if(!brojElemenata()) throw std::logic_error("Red je prazan, nemam sta izbaciti");
    TIP x(poc->element);
    Cvor *p(poc);
    if(poc==kraj)
        poc=kraj=0;
    else
        poc=poc->sljedeci;
    delete p;
    duzina--;
    return x;
}
template<typename TIP>
TIP& Red<TIP>::celo() const {
    if(poc==0) throw std::logic_error("Red je prazan, nemam sta za vratiti");
    return poc->element;
}
template<typename TIP>
void Red<TIP>::kopiraj(const Red &novi){
    poc=kraj=0;
    duzina=0;
    Cvor *p(novi.poc);
    while(p!=0){
        stavi(p->element);
        p=p->sljedeci;
    }
}
//unit testovi
bool testCelo(){
    int x(0);
    try{
        Red<int> a;
        a.celo();
    }catch(std::logic_error izu){
        x++;
    }
    Red<int> a;
    a.stavi(5);
    if(a.celo()==5) x++;
    if(x==2) return true;
    return false;
}
bool testSkini(){
    Red<int> a;
    a.stavi(5);
    a.stavi(2);
    if(a.skini()==5) return true;
    return false;
}
bool testStavi(){
    Red<int> a;
    const int x = 1;
    for(int i(1);i<=5;i++)
        a.stavi(i*x);
        a.skini();
    if(a.celo()==2*x) return true;
    return false;
}
bool testBrisi(){
    Red<int> a;
    for(int i(0);i<5;i++)
        a.stavi(i);
    a.brisi();
    if(a.brojElemenata()==0) return true;
    return false;
}
bool testOperatorDodjele(){
    Red<int> a;
    for(int i(0);i<5;i++)
        a.stavi(i);
    Red<int> b;
    b=a;
    if(b.celo()==a.celo()) return true;
    return false;
}
bool testKopirajuci(){
    Red<int> a;
    for(int i(0);i<5;i++)
        a.stavi(i);
    Red<int> b(a);
    if(b.celo()==a.celo()) return true;
    return false;    
}

void TEST_RED(){
    std::cout<<"...Testing Red....\n"<<std::endl;
    if(testCelo()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testSkini()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testStavi()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl; 
    if(testBrisi()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    if(testOperatorDodjele()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl; 
    if(testKopirajuci()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;        
    std::cout<<"...End of file...\n"<<std::endl;
}

int main() {
    TEST_RED();
    return 0;
}