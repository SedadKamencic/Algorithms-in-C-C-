#ifndef NIZMAPA
#define NIZMAPA

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
#endif