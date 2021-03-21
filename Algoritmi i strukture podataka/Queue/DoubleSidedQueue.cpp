#include <iostream>
#include <stdexcept>

template<typename TIP>
class DvostraniRed {
    struct Cvor{
        TIP element;
        Cvor* nextp;
        Cvor* backp;
        Cvor(TIP element, Cvor* nextp, Cvor* backp) : element(element), nextp(nextp), backp(backp){
            if(nextp!=nullptr) nextp->backp=this;
            if(backp!=nullptr) backp->nextp=this;
        }
    };
    Cvor* poc;
    Cvor* kr;
    int duzina;
public:
    DvostraniRed():poc(0), kr(0), duzina(0) {}
    ~DvostraniRed(){ brisi();}
    DvostraniRed(const DvostraniRed &n);
    DvostraniRed &operator=(const DvostraniRed &n);
    void brisi();
    int brojElemenata() const { return duzina; }
    bool daLiJePrazan() const { if(duzina==0) return true; return false; }
    void staviNaVrh(const TIP &el);
    TIP skiniSaVrha();
    void staviNaCelo(const TIP &el);
    TIP skiniSaCela();
    TIP& vrh() const;
    TIP& celo() const;
};
template<typename TIP>
DvostraniRed<TIP>::DvostraniRed(const DvostraniRed<TIP> &n){
    poc=kr=0;
    duzina=0;
    Cvor *p = n.poc;
    while(duzina!=n.brojElemenata()){
        staviNaVrh(p->element);
        p=p->nextp;
    }
}
template<typename TIP>
DvostraniRed<TIP>& DvostraniRed<TIP>::operator=(const DvostraniRed &n){
    if(&n!=this){
        brisi();
        poc=kr=0;
        duzina=0;
        Cvor *p = n.poc;
        while(duzina!=n.brojElemenata()){
            staviNaVrh(p->element);
            p=p->nextp;
        }
    }
    return *this;
}
template<typename TIP>
void DvostraniRed<TIP>::brisi(){
    while(!daLiJePrazan())
        skiniSaVrha();
}
template<typename TIP>
void DvostraniRed<TIP>::staviNaVrh(const TIP &el){
    if(daLiJePrazan()){
        Cvor *novi = new Cvor(el,0,0);
        poc=kr=novi;
        duzina++;
    }
    else{
        Cvor *novi = new Cvor(el,0,kr);
        kr->nextp=novi;
        kr=novi;
        duzina++;
    }
}
template<typename TIP>
TIP DvostraniRed<TIP>::skiniSaVrha(){
    if(brojElemenata()==0) throw std::logic_error("Dvostrani Red je prazan");
    if(brojElemenata()==1){
        TIP rez = kr->element;
        delete kr;
        kr=poc=nullptr;
        duzina--;
        return rez;
    }
    TIP rez = kr->element;
    Cvor *p = kr;
    kr->backp->nextp=nullptr;
    kr=kr->backp;
    delete p;
    duzina--;
    return rez;
}
template<typename TIP>
void DvostraniRed<TIP>::staviNaCelo(const TIP &el){
    if(daLiJePrazan()){
        Cvor *novi=new Cvor(el,0,0);
        poc=kr=novi;
        duzina++;
    }
    else{
        Cvor *novi = new Cvor(el,poc,0);
        poc->backp=novi;
        poc=novi;
        duzina++;
    }
}
template<typename TIP>
TIP DvostraniRed<TIP>::skiniSaCela(){
    if(daLiJePrazan()) throw std::logic_error("Dvostrani Red je prazan");
    if(brojElemenata()==1){
        TIP rez = poc->element;
        delete poc;
        poc=kr=nullptr;
        duzina--;
        return rez;
    }
    TIP rez  = poc->element;
    Cvor *p = poc;
    poc->nextp->backp=nullptr;
    poc=poc->nextp;
    delete p;
    duzina--;
    return rez;
}
template<typename TIP>
TIP& DvostraniRed<TIP>::vrh() const {
    if(daLiJePrazan()) throw std::logic_error("Dvostrani Red je prazan");
    return kr->element;
}
template<typename TIP>
TIP& DvostraniRed<TIP>::celo() const {
    if(daLiJePrazan()) throw std::logic_error("Dvostrani Red je prazan");
    return poc->element;
}

//Unit testovi
bool testStaviNaVrh(){
    DvostraniRed<int> dr;
    for(int i=0;i<10;i++)
        dr.staviNaVrh(i);
    if(dr.brojElemenata()==10) return true;
    return false;
}
bool testSkiniSaVrha(){
    bool flag = false;
    DvostraniRed<int> dr;
    //test jedan element
    dr.staviNaVrh(1);
    if(dr.skiniSaVrha()==1) flag=true;
    //test vise elemenata
    for(int i=1;i<=5;i++)
        dr.staviNaVrh(i);
    if(dr.skiniSaVrha()==5 && flag==true) return true;
    return false;
}
bool testStaviNaCelo(){
    DvostraniRed<int> dr;
    for(int i=0;i<10;i++)
        dr.staviNaCelo(i);
    if(dr.brojElemenata()==10) return true;
    return false;
}
bool testSkiniSaCela(){
    bool flag = false;
    DvostraniRed<int> dr;
    //test jedan element
    dr.staviNaCelo(1);
    if(dr.skiniSaCela()==1) flag = true;
    //test vise elemenata
    for(int i=0;i<5;i++)
        dr.staviNaCelo(i);
    if(dr.skiniSaCela()==4 && flag) return true;
    return false;
}
bool testCeloIVrh(){
    DvostraniRed<int> dr;
    for(int i=0;i<5;i++)
        dr.staviNaVrh(i);
    if(dr.celo()==0 && dr.vrh()==4) return true;
    return false;
}
bool testBrisi(){
    bool flag = false;
    DvostraniRed<int> dr;
    for(int i=1;i<=1000;i++)
        dr.staviNaVrh(i);
    if(dr.brojElemenata()==1000) flag=true;
    dr.brisi();
    if(dr.daLiJePrazan() && flag) return true;
    return false;
}
bool testKopirajuci(){
    bool flag=false;
    DvostraniRed<int> dr1;
    for(int i=1;i<=5;i++)
        dr1.staviNaVrh(i);
    {
        DvostraniRed<int> dr2(dr1);
        if(dr2.celo()==1 && dr2.vrh()==5) flag=true;
    }
    if(dr1.celo()==1 && dr1.vrh()==5 && flag) return true;
    return false;
}
bool testDodjela(){
    bool flag=false;
    DvostraniRed<int> dr1;
    for(int i=1;i<=5;i++)
        dr1.staviNaVrh(i);
    {
        DvostraniRed<int> dr2;
        for(int i=5;i<12;i++)
            dr2.staviNaVrh(i);
        dr2=dr1;    
        if(dr2.celo()==1 && dr2.vrh()==5) flag=true;
    }
    if(dr1.celo()==1 && dr1.vrh()==5 && flag) return true;
    return false;
}

void GlavnaTestFunkcija(){
    std::cout<<"Test stavi na vrh: ";
    if(testStaviNaVrh()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test skini sa vrha: ";
    if(testSkiniSaVrha()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl; 
    std::cout<<"Test stavi na celo: ";
    if(testStaviNaCelo()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test skini sa cela: ";
    if(testSkiniSaCela()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test celo i vrh: ";
    if(testCeloIVrh()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test brisi: ";
    if(testBrisi()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test kopirajuci: ";
    if(testKopirajuci()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test operator =: ";
    if(testDodjela()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;        
}

int main() {
    GlavnaTestFunkcija();
    return 0;
}
