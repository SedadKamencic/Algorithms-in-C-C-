#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>
#include <ctime>

using namespace std;

#include "mape.h"

unsigned int jednostavna(int kljuc, unsigned int max){
    return kljuc % max;
}

template<typename TK, typename TV>
class HashMapa{
    int brElem;
    int kapacitet;
    std::pair<TK, TV> *elementi;
    unsigned int (*h)(TK, unsigned int);
public:    
    HashMapa();
    ~HashMapa();
    HashMapa(const HashMapa &hm);
    HashMapa &operator =(const HashMapa &hm);
    void definisiHashFunkciju(unsigned int (*fun)(TK, unsigned int));
    void obrisi();
    void obrisi(const TK &kljuc);
    TV &operator [](const TK &kljuc);
    TV operator [](const TK &kljuc) const;
    void Ispisi() const;
    int brojElemenata() const;
};

template<typename TK, typename TV>
HashMapa<TK,TV>::HashMapa(const HashMapa<TK, TV> &hm){
    kapacitet = hm.kapacitet;
    elementi = new std::pair<TK, TV>[kapacitet]{};
    for(int i(0);i<kapacitet;i++){
        elementi[i] = hm.elementi[i];
    }
    brElem = hm.brElem;
    h = hm.h;
}
template<typename TK, typename TV>
HashMapa<TK, TV> &HashMapa<TK, TV>::operator =(const HashMapa<TK, TV> &hm){
    if(this!=&hm){
        delete[] elementi;
        kapacitet = hm.kapacitet;
        elementi = new std::pair<TK, TV>[kapacitet];
        for(int i(0);i<kapacitet;i++)
            elementi[i] = hm.elementi[i];
        brElem = hm.brElem;
        h = hm.h;
    }
    return *this;
}

template<typename TK, typename TV>
HashMapa<TK, TV>::HashMapa(){
    kapacitet = 100;
    brElem = 0;
    elementi = new std::pair<TK, TV>[kapacitet]{};
    h = nullptr;
}
template<typename TK, typename TV>
HashMapa<TK, TV>::~HashMapa(){
    delete[] elementi;
}
template<typename TK, typename TV>
void HashMapa<TK, TV>::definisiHashFunkciju(unsigned int (*fun)(TK, unsigned int)){
    h = fun;
}
template<typename TK, typename TV>
void HashMapa<TK, TV>::obrisi(){
    delete[] elementi;
    kapacitet = 100;
    brElem = 0;
    elementi = new std::pair<TK, TV>[kapacitet]{};
}
template<typename TK, typename TV>
TV HashMapa<TK, TV>::operator [](const TK &kljuc) const{
    unsigned int j = h(kljuc, kapacitet);
    if(elementi[j].first == kljuc)
        return elementi[j].second;
    else{
        int i(j+1);
        do{
            if(i>=kapacitet)
                i=0;
            if(elementi[i].first == kljuc)
                return elementi[i].second;
            else
                i++;
        }while(elementi[i].first != TK() && i!=j);
    }
    return elementi[kapacitet-1].second;
}
template<typename TK, typename TV>
TV &HashMapa<TK, TV>::operator [](const TK &kljuc){
    if(h==nullptr){
        throw std::domain_error("Hash funkcija nije definirana!");
    }
    unsigned int j = h(kljuc, kapacitet);
    if(brElem == kapacitet/2){
        std::pair<TK, TV> *novi = new std::pair<TK, TV>[kapacitet*2]{};
        for(int i(0);i<kapacitet;i++)
            novi[i] = elementi[i];
        kapacitet *=2;
        delete[] elementi;
        elementi = novi;
    }
    
    if(elementi[j].first == kljuc)
        return elementi[j].second;
    else{
        int i(j+1);
        do{
            if(i>=kapacitet)
                i=0;
            if(elementi[i].first == kljuc)
                return elementi[i].second;
            else
                i++;
        }while(elementi[i].first != TK() && i!=j);
    }    
    
    std::pair<TK, TV> tmp = std::make_pair(kljuc, TV());
    
    if(elementi[j].first == TK()){
        elementi[j] = tmp;
        brElem++;
        return elementi[j].second;
    }
    else{
        int i(j+1);
        do{
            if(i>=kapacitet)
                i = 0;
            if(elementi[i].first == TK()){
                elementi[i] = tmp;
                brElem++;
                return elementi[i].second;
            }
            else 
                i++;
        }while(i!=j);
    }
    throw std::domain_error("Neispravno");
    return elementi[0].second;
}
template<typename TK, typename TV>
void HashMapa<TK, TV>::Ispisi() const{
    for(int i(0);i<kapacitet;i++){
        std::cout<<elementi[i].first<<", "<<elementi[i].second<<std::endl;
    }
}
template<typename TK, typename TV>
int HashMapa<TK, TV>::brojElemenata() const{
    return brElem;
}
template<typename TK, typename TV>
void HashMapa<TK, TV>::obrisi(const TK &kljuc){
    unsigned int j = h(kljuc, kapacitet);
    if(elementi[j].first == kljuc){
        elementi[j].first = TK();
        brElem--;
        return;        
    }
    else{
        int i(j+1);
        do{
            if(i>=kapacitet)
                i=0;
            if(elementi[i].first == kljuc){
                elementi[i].first = TK();
                brElem--;
                return;
            }
            else
                i++;
        }while(elementi[i].first != TK() && i!=j);
    }
}

int testBrzineUbacivanja(int odabir){
    HashMapa<int, int> hm;
    NizMapa<int ,int> nm;
    BinStabloMapa<int, int> bsm;
    
    int razlika(0);
    if(odabir == 1){
        hm.definisiHashFunkciju(jednostavna);
        clock_t vrijeme1 = clock();
        for(int i(0);i<10000;i++){
            hm[i] = i+3000;
        }
        clock_t vrijeme2 = clock();
        razlika = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    }
    else if(odabir == 2){
        clock_t vrijeme1 = clock();
        for(int i(0);i<10000;i++){
            nm[i] = i+3000;
        }
        clock_t vrijeme2 = clock();
        razlika = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    }
    else if(odabir == 3){
        clock_t vrijeme1 = clock();
        bsm[5000] = 5000;
        for(int i(0);i<1000;i++)
            bsm[i] = i;
        for(int i(5001);i<7000;i++)
            bsm[i] = i;    
        for(int i(1000);i<5000;i++)
            bsm[i]= i;    
        for(int i(7000);i<10000;i++)
            bsm[i]= i;    
        
        clock_t vrijeme2 = clock(); 
        razlika = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);        
    }    
    
    return razlika;
}
int testBrzinePronalazenja(int odabir){
    int razlika(0);
    HashMapa<int, int> hm;
    NizMapa<int, int> nm;
    BinStabloMapa<int, int> bsm;
    
    if(odabir == 1){
        hm.definisiHashFunkciju(jednostavna);
        for(int i(0);i<10000;i++)
            hm[i] = i+3000;
        clock_t vrijeme1 = clock();
        for(int i(0);i<10000;i++)
            hm[i];
        clock_t vrijeme2 = clock(); 
        razlika = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    }
    else if(odabir == 2){
        for(int i(0);i<10000;i++)
            nm[i] = i+3000;
        clock_t vrijeme1 = clock();
        for(int i(0);i<10000;i++)
            nm[i];
        clock_t vrijeme2 = clock(); 
        razlika = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);        
    }
    else if(odabir == 3){
        bsm[5000] = 5000;
        for(int i(0);i<1000;i++)
            bsm[i] = i;
        for(int i(5001);i<7000;i++)
            bsm[i] = i;    
        for(int i(1000);i<5000;i++)
            bsm[i]= i;    
        for(int i(7000);i<10000;i++)
            bsm[i]= i;    
        
        clock_t vrijeme1 = clock();
        for(int i(0);i<10000;i++)
            bsm[i];
        clock_t vrijeme2 = clock(); 
        razlika = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);        
    }
    
    return razlika;    
}

int main () {
    std::cout<<"Test brzine umetanja elemenata u hash mapu: "<<testBrzineUbacivanja(1)<<"ms."<<std::endl; //~38ms
    std::cout<<"Test brzine pronalazenja elemenata u hash mapu: "<<testBrzinePronalazenja(1)<<"ms."<<std::endl; //~6ms*/
        //ove dobivene brojke su za broj elemenata 10k. Morao sam smanjiti sa 100k na 10k da bih uopste mogao testirati
        //zbog C9 ogranicenja
        //mada bi za pronalazenje elemenata trebalo vise vremena kao i za ubacivanje jer promjenom kapaciteta, mjenja se i
        //vrijednost hash-a, pa HashMapa bude neispravna
    
    std::cout<<std::endl;
    std::cout<<"Test brzine umetanja elemenata u niz mapu: "<<testBrzineUbacivanja(2)<<"ms."<<std::endl; //~2600ms ali za 10 hiljada elemenata
        //za 100k elemenata je predugo vrijeme izvrsavanja, pa c9 obustavi program
    std::cout<<"Test brzine pronalazenja elemenata u niz mapu: "<<testBrzinePronalazenja(2)<<"ms."<<std::endl;//~2400ms ali za 10 hiljada elemenata
        //za 100k elemenata je predugo vrijeme izvrsavanja, pa c9 obustavi program 
    
    std::cout<<std::endl;
    std::cout<<"Test brzine umetanja elemenata u binarno stablo mapa: "<<testBrzineUbacivanja(3)<<"ms."<<std::endl; //~1908ms ali za 10 hiljada elemenata    
    std::cout<<"Test brzine umetanja elemenata u binarno stablo mapa: "<<testBrzinePronalazenja(3)<<"ms."<<std::endl; //~1546ms ali za 10 hiljada elemenata   
    
    return 0;
}