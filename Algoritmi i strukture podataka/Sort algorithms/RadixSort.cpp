#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>

using namespace std;

void radixSort(std::vector<int> &a){
    int max(a[0]);
    for(int i(1); i<a.size(); i++)
        if(a[i] > max)
            max = a[i];
            
    for(int cifra(1); max/cifra>0; cifra*=10){
        std::vector<int> histogram(10);
        
        for(int i(0); i<a.size(); i++)
            histogram[(a[i] / cifra) % 10]++;
            
        for(int i(1); i<10; i++)
            histogram[i] += histogram[i-1];
            
        std::vector<int> izlazni;
        izlazni.resize(a.size());
        
        for(int i(a.size()-1); i>=0; i--){
            izlazni[ histogram[ (a[i]/cifra) % 10 ] - 1 ] = a[i];
            histogram[ (a[i]/cifra) % 10 ]--;
        }
        
        for(int i(0); i<a.size(); i++)
            a[i] = izlazni[i];
    }        
}

bool daLiJeList(std::vector<int> a, int i){
    return ( i >= (a.size()/2) && i < a.size() );
}
int lijevoDijete(int i){
    return 2*i + 1;
}
int desnoDijete(int i){
    return 2*i + 2;
}

void popraviDolje(std::vector<int> &a, int i){
    int velicina(a.size());
    while(!daLiJeList(a, i)){
        int veci(lijevoDijete(i));
        int dd(desnoDijete(i));
        if(dd < velicina && a[dd] > a[veci])
            veci = dd;
        if( a[i] > a[veci] )
            return;
        std::swap(a[i], a[veci]);
        i = veci;
    }
}
bool daLiJeList(int velicina, int i){
    return (i >= velicina/2 and i<velicina);
}
void popraviDolje(std::vector<int> &a, int i,int velicina){
    while(!daLiJeList(velicina, i)){
        int veci(lijevoDijete(i));
        int dd(desnoDijete(i));
        if(dd < velicina && a[dd] > a[veci])
            veci = dd;
        if( a[i] > a[veci] )
            return;
        std::swap(a[i], a[veci]);
        i = veci;
    }
}

void stvoriGomilu(std::vector<int> &a){
    int velicina(a.size()-1);
    for(int i(velicina/2); i>=0;i--){
        popraviDolje(a, i);
    }
}

int Roditelj(int i){
    return (i-1) / 2;
}

void popraviGore(std::vector<int> &a, int i){
    while(i != 0 && a[i] > a[Roditelj(i)]){
        std::swap(a[i], a[Roditelj(i)]);
        i = Roditelj(i);
    }
}

void umetniUGomilu(std::vector<int> &a, int x, int &velicina){
    a.push_back(x);
    velicina++;
    popraviGore(a,velicina - 1);
}

int izbaciPrvi(std::vector<int> &a, int &velicina){
    if(velicina == 0){
        throw std::domain_error("Gomila je prazna");
    }
    velicina--;
    std::swap(a[0], a[velicina]);
    if(velicina != 0){
        popraviDolje(a,0,velicina);
    }
    return a[velicina];
}

void gomilaSort(std::vector<int> &a){
    stvoriGomilu(a);
    int velicina(a.size()-1);
    for(int i(a.size()-1);i>0;i--){
        std::swap(a[0],a[i]);
        velicina--;
        popraviDolje(a,0,i);
    }
}

//unit testovi
bool testRadixSort(){
    std::vector<int> vek{8876,9876,6576,2,345,21,2,31121,23451,102910,2242524,2532530};
    std::vector<int> vek_sort(vek);
    {
        radixSort(vek);
    }
    {
        std::sort(vek_sort.begin(),vek_sort.end());
    }
    for(int i(0);i<vek.size();i++)
        if(vek[i] != vek_sort[i])
            return false;
    return true;        
}
bool testStvoriGomilu(){
    std::vector<int> a;
    a.push_back(22);
    a.push_back(32);
    a.push_back(44);
    a.push_back(47);
    a.push_back(50);
    stvoriGomilu(a);
    if(a[0]==50 && a[1]==47 && a[2]==44 && a[3]==22 && a[4]==32){
        return true;
    }
    return false;    
}
bool testUmetniUGomilu(){
    std::vector<int> a;
    int velicina(0);
    umetniUGomilu(a,32,velicina);
    umetniUGomilu(a,44,velicina);
    umetniUGomilu(a,22,velicina);
    umetniUGomilu(a,47,velicina);
    umetniUGomilu(a,50,velicina);
    if(a[0]==50 && a[1]==47 && a[2]==22 && a[3]==32 && a[4]==44){
        return true;
    }
    return false;       
}
bool testizbaciPrvi(){
    std::vector<int> a;
    a.push_back(22);
    a.push_back(32);
    a.push_back(44);
    a.push_back(47);
    a.push_back(50);
    int velicina(a.size());
    stvoriGomilu(a);
    for(int i(0);i<5;i++)
        izbaciPrvi(a,velicina);
    bool flag1(true);    
    for(int i(0);i<4;i++)
        if(!(a[i]<a[i+1])) flag1 = false;
    if(velicina == 0 && flag1)
        return true;
    return false;
}
bool testGomilaSort(){
    std::vector<int> vek{8876,9876,6576,2,345,21,2,31121,23451,102910,2242524,2532530};    
    std::vector<int> vek_sort(vek);
    stvoriGomilu(vek);
    {
        gomilaSort(vek);
    }
    {
        std::sort(vek_sort.begin(),vek_sort.end());
    }
    for(int i(0);i<vek.size();i++)
        if(vek[i] != vek_sort[i])
            return false;
    return true;        
}

void GlavnaTestFunkcija(){
    std::cout<<"Test Radix sort: ";
    if(testRadixSort()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Stvori Gomilu: ";
    if(testStvoriGomilu()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;   
    std::cout<<"Test Umetni u Gomilu: ";
    if(testUmetniUGomilu()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Izbaci Prvi: ";
    if(testizbaciPrvi()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;        
    std::cout<<"Test Gomila Sort: ";
    if(testGomilaSort()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;                
}

int main() {
    GlavnaTestFunkcija();
    return 0;
}