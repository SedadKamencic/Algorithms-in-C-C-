#include <iostream>
#include <utility>
#include <stdexcept>

using namespace std;
//Bazna
template<typename TK, typename TV>
class Mapa{
public:
    Mapa(){};
    virtual ~Mapa(){};
    virtual TV operator[](TK kljuc) const =0;
    virtual TV &operator[](TK kljuc) =0;
    virtual int brojElemenata() const =0;
    virtual void obrisi() =0;
    virtual void obrisi(const TK &kljuc) =0;
};

//Niz Mapa
template<typename TK, typename TV>
class NizMapa : public Mapa<TK, TV>{
private:
    std::pair<TK, TV> *nizM;
    int kap;
    int br_elem;
public:
    NizMapa();
    ~NizMapa();
    NizMapa(const NizMapa &m);
    NizMapa& operator =(const NizMapa &novi);
    TV operator[](TK kljuc) const override;
    TV &operator[](TK kljuc) override;
    int brojElemenata() const override {
        return br_elem;
    }
    int kapacitetMape() const{
        return kap;
    }
    void obrisi() override;
    void obrisi(const TK &kljuc) override;
    void ispisi() const{
        for(int i(0);i<br_elem;i++)
        std::cout<<nizM[i].first<<","<<nizM[i].second<<" "<<std::endl;;
    }
    
};
template<typename TK, typename TV>
NizMapa<TK, TV>::NizMapa(const NizMapa &m){
    br_elem =0;
    kap = m.kapacitetMape();
    nizM = new std::pair<TK, TV>[kap];
    for(int i(0);i<m.brojElemenata();i++)
        nizM[i] = m.nizM[i];
    br_elem = m.brojElemenata();    
}
template<typename TK, typename TV>
NizMapa<TK, TV>& NizMapa<TK, TV>::operator =(const NizMapa<TK, TV> &m){
    if(this!=&m){
        delete[] nizM;
        br_elem=0;
        kap = m.kapacitetMape();
        nizM = new std::pair<TK, TV>[kap];
        for(int i(0);i<m.brojElemenata();i++) {
            nizM[i] = m.nizM[i];
        }
        br_elem = m.brojElemenata();
    }        
    return *this;
}

template<typename TK, typename TV>
NizMapa<TK, TV>::NizMapa(){
    kap=100;
    br_elem =0;
    nizM = new std::pair<TK, TV>[kap];
}
template<typename TK, typename TV>
NizMapa<TK, TV>::~NizMapa(){
    br_elem = 0;
    kap=0;
    delete[] nizM;
}
//constantna verzija
template<typename TK, typename TV>
TV NizMapa<TK, TV>::operator[](TK kljuc) const{
    for(int i(0);i<kap;i++){
        if(kljuc == nizM[i].first){
            return nizM[i].second;
        }    
    }        
    return TV();
}
//non-const verzija
template<typename TK, typename TV>
TV &NizMapa<TK, TV>::operator[](TK kljuc){
    for(int i(0);i<br_elem;i++){
        if(nizM[i].first == kljuc)
            return nizM[i].second;
    }
    if(brojElemenata() == kapacitetMape()){
        std::pair<TK, TV> *pom;
        pom = new std::pair<TK, TV>[2*kap];
        kap=2*kap;
        for(int i(0);i<br_elem;i++)
            pom[i]=nizM[i];
        delete[] nizM;
        nizM=pom;
    }
    std::pair<TK, TV> temp(kljuc, TV());
    nizM[br_elem] = temp;
    br_elem++;
    return nizM[br_elem-1].second;
}
template<typename TK, typename TV>
void NizMapa<TK, TV>::obrisi(){
    delete[] nizM;
    br_elem =0;
    nizM = new std::pair<TK, TV>[100];
    kap=100;
} 
template<typename TK, typename TV>
void NizMapa<TK, TV>::obrisi(const TK &kljuc){
    for(int i(0);i<br_elem;i++)
        if(nizM[i].first == kljuc){
            std::pair<TK, TV> temp(nizM[br_elem-1].first, nizM[br_elem-1].second);
            nizM[br_elem-1].first = nizM[i].first;      nizM[br_elem-1].second=nizM[i].second;
            nizM[i].first = temp.first;     nizM[i].second = temp.second;
            br_elem--;
            return;
        }
    throw std::domain_error("Ne postoji odgovarajuci kljuc");    
}


//Unit testovi
bool testPovecanjeKapaciteta() {
    bool flag1(false), flag2(false);
    NizMapa<int, int> a;
    int i(0);
    for(;i<100;i++)
        a[i]=i;
    if(a.kapacitetMape() == 100) flag1=true;
    a[i]=i;
    if(a.kapacitetMape()==200) flag2 = true;
    if(flag1 == true && flag2 == true ) return true;
    return false;
}
bool testConstOperatorUglaste(){
    bool flag1(false), flag2(false);
    NizMapa<int, int> a;
    for(int i(0);i<5;i++)
        a[i]=i;
    {
        const NizMapa<int, int> b(a);
        if(b[0]==0 && b[4]==4) flag2 = true;
    }
    if(a[0]==0 && a[4]==4) flag1 = true;
    if(flag1 == true && flag2 == true ) return true;
    return false;
}
bool testNonConstOperatoraUglaste(){
    bool flag1(false), flag2(false), flag3(false);
    NizMapa<int, int> a;
    for(int i(0);i<5;i++)
        a[i]=i;
    if(a[3]==3) flag1=true;
    a[3]=10;
    if(a[3]==10) flag2 = true;
    for(int i(5);i<=100;i++)
        a[i]=i;
    if(a[100] == 100 && a.kapacitetMape()==200) flag3 = true;
    if(flag1==true && flag2==true && flag3==true) return true;
    return false;
}
bool testBrisanjeKljuca(){
    bool flag1(false), flag2(false);
    NizMapa<int, int> a;
    for(int i(0);i<5;i++)
        a[i]=i;
    {
        a.obrisi(3);
        try{
            if(a[3] == 0) flag1=true;
        }catch(...){
            flag1=false;
        }
    }
    try{
        a.obrisi(5);
    }catch(std::domain_error izu){
        flag2=true;
    }
    if(flag1 == true and flag2 == true) return true;
    return false;
}
bool testObrisiBezParametara(){
    NizMapa<int, int> a;
    for(int i(0);i<500;i++)
        a[i]=i;
    a.obrisi();
    if(a.kapacitetMape()==100 && a.brojElemenata()==0 && a[0]==0) return true;
    return false;
}
bool testFunkcionalnosti(){
    bool flag1(false), flag2(false);
    NizMapa<int, int> a;
    for(int i(0);i<100;i++)
        a[i]=i;
    {
            NizMapa<int, int> b(a);
            NizMapa<int, int> c;
            c=a;
            b[596] = 20;
            c[311] = 5;
            a.obrisi(0);
            if(b[0]==0) flag1 = true;
            if(c[0]==0) flag2 = true;
    }        
    if(a[0]==0 && flag1== true && flag2==true) return true;
    return false;
}

void GlavnaTestFunkcija(){
    std::cout<<"Test povecanja kapaciteta: ";
    if(testPovecanjeKapaciteta()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test const operatora []: ";
    if(testConstOperatorUglaste()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Non const operatora []: ";
    if(testNonConstOperatoraUglaste()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test obrisi kljuc: ";
    if(testBrisanjeKljuca()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl; 
    std::cout<<"Test obrisi metoda: ";
    if(testObrisiBezParametara()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test funkcionalnosti: ";
    if(testFunkcionalnosti()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;         
}

int main() {
    GlavnaTestFunkcija();
    return 0;
}
