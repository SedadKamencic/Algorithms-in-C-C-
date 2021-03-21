#include <iostream>
#include <stdexcept>
#include <ctime>

#include "NizMapa.h"

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
int testDodavanjaElemenata(int odabir){
    BinStabloMapa<int, int> m;
    NizMapa<int, int> nm;
    
    if(odabir == 1){
        clock_t vrijeme1 = clock();
        for(int i(0);i<10000;i++)
            m[i] = rand();
        clock_t vrijeme2 = clock();
        int ukupnoVrijeme( (vrijeme2 - vrijeme1)/ (CLOCKS_PER_SEC / 1000) );
        return ukupnoVrijeme;
    }
    else{
        clock_t vrijeme1 = clock();
        for(int i(0);i<10000;i++)
            nm[i] = rand();
        clock_t vrijeme2 = clock();
        int ukupnoVrijeme( (vrijeme2 - vrijeme1)/ (CLOCKS_PER_SEC / 1000) );
        return ukupnoVrijeme;        
    }
    return 0;
}
int testPristupaElemenata(int odabir){
    BinStabloMapa<int, int> m;
    NizMapa<int, int> nm;
    if(odabir == 1){
        for(int i(0);i<4000;i++)
            m[i] = i;
            
        clock_t vrijeme1 = clock();    
        for(int i(4000);i>=0;i--)
            m[i];
        clock_t vrijeme2 = clock();
        int ukupnoVrijeme( (vrijeme2 - vrijeme1)/ (CLOCKS_PER_SEC / 1000) );
        return ukupnoVrijeme;     
    }
    else{
        for(int i(0);i<4000;i++)
            nm[i] = i;
            
        clock_t vrijeme1 = clock();    
        for(int i(4000);i>=0;i--)
            nm[i];
        clock_t vrijeme2 = clock();
        int ukupnoVrijeme( (vrijeme2 - vrijeme1)/ (CLOCKS_PER_SEC / 1000) );
        return ukupnoVrijeme;        
    }
}

void testPerformansi(){
    int vrijeme1(testDodavanjaElemenata(2));
    int vrijeme2(testDodavanjaElemenata(1));
    std::cout<<"Ukupno vrijeme dodavanja elemenata u binarno stablo : "<<vrijeme1<<"ms."<<std::endl;
    std::cout<<"Ukupno vrijeme dodavanja elemenata u niz mapa :       "<<vrijeme2<<"ms."<<std::endl;
    std::cout<<"Razlika vremena: ------------------------------------ "<<vrijeme1-vrijeme2<<"ms."<<std::endl;
    std::cout<<"\n";
    
    vrijeme1 = testPristupaElemenata(2);
    vrijeme2 = testPristupaElemenata(1);
    std::cout<<"Ukupno vrijeme pristupa postojecim elementima za za binarno stablo: "<<vrijeme1<<"ms."<<std::endl;
    std::cout<<"Ukupno vrijeme pristupa postojecim elementima za za niz mapa:       "<<vrijeme2<<"ms."<<std::endl;
    std::cout<<"Razlika vremena: -------------------------------------------------- "<<vrijeme1-vrijeme2<<"ms."<<std::endl;
    std::cout<<"\n";
    std::cout<<"Binarno stablo je brze zbog svoje strukture koja omogucava brzi pristup elementima za razliku od Niz Mapa gdje se svaki put";
    std::cout<<" prilikom poziva operatora[] trosi vrijeme na sekvencijalno ispitivanje svakog elementa iz kolekcije na jednakost, te uzmimo u obzir";
    std::cout<<" da svaki put kad prekoracimo kapacitet, utrosi se i znacajno vrijeme za prosirenje kapaciteta, te binarno stablo ima i bolju memorijsku iskoristenost."<<std::endl;
}

int main(){
    testPerformansi();
    return 0;
}