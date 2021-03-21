#include <iostream>
#include <stdexcept>
#include <ctime>
#include <utility>

#ifndef MAPE
#define MAPE

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
template<typename TK, typename TV>
TV NizMapa<TK, TV>::operator[](TK kljuc) const{
    for(int i(0);i<kap;i++){
        if(kljuc == nizM[i].first){
            return nizM[i].second;
        }    
    }        
    return TV();
}
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

//Stablo
template<typename TK, typename TV>
class BinStabloMapa : public Mapa<TK, TV>{
    struct Cvor{
        TK kljuc;
        TV vrijednost;
        Cvor *lijevi, *desni, *roditelj;
    };
    
    Cvor* korijen;
    int brEleme;
public:
    BinStabloMapa();
    ~BinStabloMapa();
    int brojElemenata() const override;
    TV &operator[](TK kljuc) override;
    TV operator[](TK kljuc) const override;
    void obrisi() override;
    void obrisi(const TK &kljuc) override;
    void ObrisiSve(Cvor *r);
};

template<typename TK, typename TV>
BinStabloMapa<TK, TV>::BinStabloMapa(){
    korijen = nullptr;
    brEleme = 0;
}
template<typename TK, typename TV>
BinStabloMapa<TK, TV>::~BinStabloMapa(){
    ObrisiSve(korijen);
}
template<typename TK, typename TV>
int BinStabloMapa<TK, TV>::brojElemenata() const {
    return brEleme;
}
template<typename TK, typename TV>
TV &BinStabloMapa<TK, TV>::operator[](TK kljuc){
    Cvor *y;
    Cvor *leteci(korijen);
    while(leteci != nullptr){
        y = leteci;
        if(kljuc == leteci->kljuc)
            return leteci->vrijednost;
        else if(kljuc > leteci->kljuc)
            leteci = leteci->desni;
        else
            leteci = leteci->lijevi;
    }
    if(brEleme == 0){
        korijen = new Cvor();
        korijen->kljuc = kljuc; korijen->vrijednost = TV();
        korijen->lijevi = nullptr; korijen->desni=nullptr; korijen->lijevi = nullptr;
        brEleme++;
        return korijen->vrijednost;
    }
    else{
        Cvor *novi = new Cvor();
        novi->kljuc = kljuc; novi->vrijednost = TV(); novi->lijevi = nullptr;
        novi->desni = nullptr;
        novi->roditelj = nullptr;
        novi->roditelj = y;
        
        if(novi->kljuc > novi->roditelj->kljuc){
            y->desni = novi;
            brEleme++;
            return novi->vrijednost;
        }
        else{
            y->lijevi = novi;
            brEleme++;
            return novi->vrijednost;            
        }    
    }
    return korijen->vrijednost;
}
template<typename TK, typename TV>
TV BinStabloMapa<TK, TV>::operator [](TK kljuc) const{
    Cvor* leteci;
    leteci = korijen;
    while(leteci != nullptr && kljuc != leteci->kljuc){
        if(kljuc < leteci->kljuc)
            leteci = leteci->lijevi;
        else 
            leteci = leteci->desni;
    }
    if(leteci == nullptr)
        return TV();
    return leteci->vrijednost;
}
template<typename TK, typename TV>
void BinStabloMapa<TK, TV>::obrisi(){
    obrisi(korijen->kljuc);
}
template<typename TK, typename TV>
void BinStabloMapa<TK, TV>::obrisi(const TK &kljuc){
    Cvor* p(korijen);
    Cvor* roditelj(nullptr);
    
    while(p != nullptr && kljuc != p->kljuc){
        roditelj = p;
        if(kljuc < p->kljuc)
            p = p->lijevi;
        else
            p = p->desni;
    }
    
    if(p == nullptr)
        return;
    
    Cvor *m(nullptr);    
    if(p->lijevi == nullptr)
        m = p->desni;
    else{
        if( p->desni == nullptr)
            m = p->lijevi;
        else{
            Cvor *pm(p);
            m = p->lijevi;
            Cvor *tmp(m->desni);
            
            while(tmp != nullptr){
                pm = m;
                m = tmp;
                tmp = m->desni;
            }
            if(pm != p){
                pm->desni = m->lijevi;
                m->lijevi = p->lijevi;
            }
            m->desni = p->desni;
        }    
    }
    
    if(roditelj == nullptr)
        korijen = m;
    else{
        if( p == roditelj->lijevi )
            roditelj->lijevi = m;
        else
            roditelj->desni = m;
    }
    
    delete p;
    brEleme--;
    return;
}
template<typename TK, typename TV>
void BinStabloMapa<TK, TV>::ObrisiSve(BinStabloMapa<TK, TV>::Cvor *r){
    if(r){
        ObrisiSve(r->lijevi);
        ObrisiSve(r->desni);
        brEleme--;
        delete r;
    }
}

#endif