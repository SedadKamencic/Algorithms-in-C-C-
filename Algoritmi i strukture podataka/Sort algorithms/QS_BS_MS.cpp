#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <ctime>

using namespace std;

//pomocne funkcije
template<typename TIP>
void MERGE(TIP *niz, int l, int p, int q, int u){
    int i(0), j(q-l), k(l);
    TIP *B(new TIP[u-l+1]);
    for(int m(0);m<u-l+1;m++)
        B[m] = niz[l+m];
    while(i <= p-l && j <= u-l){
        if(B[i]<B[j]){
            niz[k]=B[i];
            i++;
        }
        else{
            niz[k] = B[j];
            j++;
        }
        k++;
    }
    
    while(i <= p-l){
        niz[k] = B[i];
        k++;
        i++;
    }
    while(j <= u-l){
        niz[k] = B[j];
        k++;
        j++;
    }
    delete[] B;
}
template<typename TIP>
void MERGE_SORT(TIP *niz, int l, int u){
    if(u>l){
        int p((l+u-1) / 2);
        int q(p+1);
        MERGE_SORT(niz, l,p);
        MERGE_SORT(niz,q,u);
        MERGE(niz,l,p,q,u);
    }
}

template<typename TIP>
void ZAMJENI(TIP *niz,const int i, const int j){
    TIP tmp = niz[i];
    niz[i] = niz[j];
    niz[j] = tmp;
}
template<typename TIP>
int PARTICIJA(TIP *niz, int first, int last){
    TIP pivot(niz[first]);
    long int p(first+1);
    while(p<last+1){
        if(niz[p] <= pivot)
            p+=1;
        else 
            break;
    }
    for(int i(p+1);i<last+1;i++){
        if(niz[i] < pivot){
            ZAMJENI(niz,p,i);
            p+=1;
        }
    }
    ZAMJENI(niz,first,p-1);
    return p-1;
}
template<typename TIP>
void QUICK_SORT(TIP *niz, int prvi, int zadnji){
    if(prvi < zadnji){
        int j(PARTICIJA(niz,prvi,zadnji));
        QUICK_SORT(niz, prvi, j-1);
        QUICK_SORT(niz, j+1, zadnji);
    }
}

//glavne funkcije
template<typename TIP>
void bubble_sort(TIP *niz, int vel){
    for(int i(vel-1);i>0;i--){
        for(int j(1);j<i+1;j++){
            if(niz[j-1] < niz[j]){
                TIP tmp = niz[j];
                niz[j] = niz[j-1];
                niz[j-1] = tmp;
            }
        }
    }
}
template<typename TIP>
void selection_sort(TIP *niz, int vel){
    for(int i(0);i<vel-1;i++){
        TIP min(niz[i]);
        int pmin(i);
        for(int j(i+1);j<vel;j++){
            if(niz[j] < min){
                min=niz[j];
                pmin = j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }
}
template<typename TIP>
void merge_sort(TIP *niz, int vel){
    int l(0), u(vel-1);
    MERGE_SORT(niz,l,u);
}
template<typename TIP>
void quick_sort(TIP *niz, int vel){
    int prvi(0), zadnji(vel-1);
    QUICK_SORT(niz, prvi, zadnji);
}

void ucitaj(std::string filename, int*& niz, int &vel){
    std::ifstream ulazni_tok(filename);
    char znak;
    int broj(0),i(0);
    if(!ulazni_tok) throw std::domain_error("Otvaranje datoteke nije uspjelo");
    else{
        while(ulazni_tok>>broj>>znak)
            i++;
        ulazni_tok.close();
        vel=i;
        i=0;
        ulazni_tok.open(filename);
        if(!ulazni_tok) throw std::domain_error("Otvaranje datoteke nije uspjelo");
        niz = new int[vel];
        while(ulazni_tok>>broj>>znak)
            niz[i++]=broj;
            
    }
    ulazni_tok.close();
}
void generisi(std::string filename, int vel){
    std::ofstream izlazni_tok(filename);
    if(!izlazni_tok) throw std::domain_error("Problem pri otvaranju datoteke");
    else{
        for(int i(1);i<=vel; i++)
            izlazni_tok << rand()<<",";
    }
    izlazni_tok.close();
}
//unit testovi
bool testBubbleSort(){
    int niz[10] = {5,4,3,2,1,6,7,8,9,10};
    bubble_sort(niz, 10);
    for(int i(0);i<10;i++)
        if(niz[i]!=(i+1)) return false;
    return true;
}
bool testSelectionSort(){
    int niz[10] = {5,4,3,2,1,6,7,8,9,10};
    selection_sort(niz, 10);
    for(int i(0);i<10;i++)
        if(niz[i]!=(i+1)) return false;
    return true;
}
bool testMergeSort(){
    int niz[10] = {5,4,3,2,1,6,7,8,9,10};
    merge_sort(niz, 10);
    for(int i(0);i<10;i++)
        if(niz[i]!=(i+1)) return false;
    return true;    
}
bool testQuickSort(){
    int niz[10] = {5,4,3,2,1,6,7,8,9,10};
    quick_sort(niz, 10);
    for(int i(0);i<10;i++)
        if(niz[i]!=(i+1)) return false;
    return true;    
}
void GlavnaTestFunkcija(){
    std::cout<<"Bubble sort: ";
    if(testBubbleSort()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Selection sort: ";
    if(testSelectionSort()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl; 
    std::cout<<"Merge sort: ";
    if(testMergeSort()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Quick sort: ";
    if(testQuickSort()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;     
}

//Funkcija za poziv funkcija za sortiranje
void Main(){
    std::cout<<"Unesite velicinu niza: ";
    int n;
    std::cin>>n;
    int *niz;
    std::cout<<"Odaberite nacin popunjavanja niza: \n1) Rucno: \n2) Automatski: \n";
    int odabir;
    std::cin>>odabir;
    switch(odabir){
        case 1:
            niz = new int[n];
            std::cout<<"Unesite elemente: ";
            for(int i(0);i<n;i++){
                int broj;
                std::cin>>broj;
                niz[i] = broj;
            }
            break;
        case 2:
            niz = new int[n];
            generisi("random", n);
            ucitaj("random",niz,n);
            break;
        default: std::cout<<"Neispravan unos";
    }
    std::cout<<"Unesite redni broj algoritma koji zelite koristiti: ";
    int i(0);
    std::cout<<"\n1)Bubble sort \n2)Selection sort \n3)Quick sort \n4)Merge sort "<<std::endl;
    do{
        std::cin>>i;
        if(i<0 || i>4) std::cout<<"Neispravan odabir,pokusajte ponovo";
    }while(i<0 || i>4);
    if(i==1){
        clock_t v1=clock();
        bubble_sort(niz,n);
        clock_t v2=clock();
        int vr=(v2-v1) / (CLOCKS_PER_SEC / 1000);
        std::cout<<"\nVrijeme izvrsavanja je "<<vr<<"ms";
    }
    else if(i==2){
        clock_t v1=clock();
        selection_sort(niz,n);
        clock_t v2=clock();
        int vr=(v2-v1) / (CLOCKS_PER_SEC / 1000);
        std::cout<<"\nVrijeme izvrsavanja je "<<vr<<"ms";
    }
    else if(i==3){
        clock_t v1=clock();
        quick_sort(niz,n);
        clock_t v2=clock();
        int vr=(v2-v1) / (CLOCKS_PER_SEC / 1000);
        std::cout<<"\nVrijeme izvrsavanja je "<<vr<<"ms";        
    }
    else{
        clock_t v1=clock();
        merge_sort(niz,n);
        clock_t v2=clock();
        int vr=(v2-v1) / (CLOCKS_PER_SEC / 1000);
        std::cout<<"\nVrijeme izvrsavanja je "<<vr<<"ms";
    }
    bool flag(true);
    for(int i(0);i<n-1;i++)
        if(niz[i]>niz[i+1]) {
            flag = false;
            break;
        }    
    if(flag) std::cout<<"\nNiz je ispravno sortiran!";
        else std::cout<<"\nNiz nije ispravno sortiran!"; 
    std::ofstream izlazni_tok("Zapis");
    if(!izlazni_tok) throw std::domain_error("Problem pri otvaranju");
    for(int i(0);i<n;i++)
        izlazni_tok<<niz[i]<<",";
    izlazni_tok.close();    
    delete[] niz;
}

int main() {
    double niz[] = {0.091696, 0.096885, 0.102076, 0.1609, 0.0346, 0.0173, 0.1609, 0.077855, 0.096885, 0.1609};
    int vel(10);
    bubble_sort(niz, vel);
    for(double x : niz) 
        std::cout<< x<< "\n";
    return 0;
}    