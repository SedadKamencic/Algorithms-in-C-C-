#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Lista.h"

using namespace std;

unsigned int glupihash(int ulaz, 
unsigned int max) {
	return max-1;
}

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
template<typename TK, typename TV>
class HashMapaLan{
    int brElem;
    int kapacitet;
    unsigned int (*h)(TK, unsigned int);
    DvostrukaLista<TK> *elementi;
public:
    HashMapaLan();
    ~HashMapaLan();
    int brojElemenata() const;
    void obrisi();
    void obrisi(const TK &kljuc);
    TV operator[](TK kljuc) const;
    TV& operator[](TK kljuc);
    void definisiHashFunkciju(unsigned int (*fun)(TK, unsigned int));
};
template<typename TK, typename TV>
HashMapaLan<TK,TV>::HashMapaLan(){
    kapacitet = 1000;
    brElem = 0;
    h = nullptr;
    elementi = new DvostrukaLista<TK>[kapacitet];
}
template<typename TK, typename TV>
HashMapaLan<TK, TV>::~HashMapaLan(){
    h = nullptr;
    for(int i(0);i<kapacitet;i++){
        if(elementi[i].brojElemenata() != 0)
            elementi[i].obrisi();
    }
    delete[] elementi;    
}
template<typename TK, typename TV>
int HashMapaLan<TK, TV>::brojElemenata() const{
    return brElem;
}
template<typename TK, typename TV>
void HashMapaLan<TK, TV>::obrisi(){
    for(int i(0);i<kapacitet;i++){
        if(elementi[i].brojElemenata() != 0)
            elementi[i].obrisi();
    }        
    delete elementi;
    kapacitet = 1000;
    brElem = 0;
    elementi = new DvostrukaLista<TK>[kapacitet]{};
}
template<typename TK, typename TV>
void HashMapaLan<TK,TV>::obrisi(const TK &kljuc){
    unsigned j(h(kljuc,kapacitet));
    if(elementi[j].brojElemenata() == 0)
        return;
    else{
        elementi[j].obrisi(kljuc);
    }
    brElem--;
}
template<typename TK, typename TV>
TV HashMapaLan<TK,TV>::operator[](TK kljuc) const{
    int j(h(kljuc,kapacitet));
    if(elementi[j].brojElemenata() == 0){
        return TV();
    }
    else{
        try{
            return elementi[j][kljuc];
        }catch(...){
            return TV();
        }
    }
    return TV();
}
template<typename TK, typename TV>
TV &HashMapaLan<TK,TV>::operator[](TK kljuc){
    int j(h(kljuc,kapacitet));
    if(elementi[j].brojElemenata() == 0){
        elementi[j].kraj();
        elementi[j].dodajIza(TV());
        elementi[j].kraj();
        return elementi[j].trenutni();
    }
    else{
        try{
            return elementi[j][kljuc];
        }catch(...){
            elementi[j].kraj();
            elementi[j].dodajIza(kljuc);
            elementi[j].kraj();
            return elementi[j].trenutni();
        }
    }
    return elementi[j][1];
}
template<typename TK, typename TV>
void HashMapaLan<TK, TV>::definisiHashFunkciju(unsigned int (*fun)(TK, unsigned int)){
    h = fun;
}

int main() {
HashMapaLan<int, int> m;
m.definisiHashFunkciju(glupihash);
// Test ponasanja sa velikim brojem kolizija
m[1] = 100;
m[2] = 200;
m[3] = 300;
m[4] = 400;
m[5] = 500;
cout << m[1] << " ";
m.obrisi(2);
cout << m[1] << " " << m[3] << " ";
cout << m[4] << " " << m[5] << " ";
cout << m[2];
    return 0;
}
