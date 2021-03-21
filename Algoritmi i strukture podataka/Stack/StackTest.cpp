#include <iostream>
#include <vector>

#include "stek.h"

int ispisIndexa;
int ispisUSteku;
std::string ispisRijeci;

void pretraga(Stek<std::vector<int>> &s, int trazeni){
    if(s.brojElemenata() == 0) {
        ispisRijeci = "Nema elementa";
        std::cout<<ispisRijeci; 
        return; 
    }
    
    std::vector<int> a(s.skini());
    if(a.size() != 0){
        if(a[a.size()-1] < trazeni){
            ispisRijeci = "Nema elementa";
            std::cout<<ispisRijeci;
            s.stavi(a);
            return; 
        }
    }    
    if(a.size()!=0 && (trazeni >= a[0] && trazeni <= a[a.size()-1])){
        if (trazeni == a[0]){
            ispisIndexa=0;  ispisUSteku=s.brojElemenata();
            std::cout<<ispisIndexa<<" "<<ispisUSteku;
            s.stavi(a);
            return;
        }    
        else if(trazeni == a[a.size()-1]){
            ispisIndexa=a.size()-1; ispisUSteku=s.brojElemenata();
            std::cout<<ispisIndexa<<" "<<ispisUSteku;
            s.stavi(a);
            return;
        }    
        int poc = 0, kraj = a.size()-1;
        while(poc <= kraj){
            int curr = (poc + kraj) / 2;
            if(trazeni > a[curr]) poc = curr + 1;
            else if(trazeni < a[curr]) kraj = curr-1;
            else if(trazeni == a[curr]){
                ispisIndexa=curr;  ispisUSteku=s.brojElemenata();
                std::cout<<ispisIndexa<<" "<<ispisUSteku;
                s.stavi(a);
                return;
            }
        }
    } 
    pretraga(s,trazeni);
    s.stavi(a);
    return;
}

bool testPrazanStek(){
    Stek<std::vector<int>> s;
    pretraga(s,11);
    if(ispisRijeci=="Nema elementa") return true;
    return false;
}
bool testStekSaSvimPraznimVek(){
    Stek<std::vector<int>> s;
    std::vector<int> a = {};
    std::vector<int> b = {};
    std::vector<int> c = {};
    s.stavi(a); s.stavi(b); s.stavi(c);
    pretraga(s,5);
    if(ispisRijeci=="Nema elementa") return true;
    return false;
}
bool testStekSaNekimPraznimVek(){
    Stek<std::vector<int>> s;
    std::vector<int> a = {1,2,3};
    std::vector<int> b = {};
    std::vector<int> c = {4,5,6};
    s.stavi(a); s.stavi(b); s.stavi(c);
    pretraga(s,2);
    if(ispisIndexa==1 && ispisUSteku==0) return true;
    return false;
}
bool testPunStek(){
    Stek<std::vector<int>> s;
    std::vector<int> a = {1,2,3};
    std::vector<int> b = {4,5,6,7,12,15,21,130};
    std::vector<int> c = {140,365,446};
    std::vector<int> d = {447,560,1200};
    s.stavi(a); s.stavi(b); s.stavi(c); s.stavi(d);
    pretraga(s,5);
    if(ispisIndexa == 1 && ispisUSteku==1) return true;
    return false;
}
bool testNemaElementa(){
    Stek<std::vector<int>> s;
    std::vector<int> a = {1,2,3};
    std::vector<int> b = {4,5,6,7,12,15,21,130};
    std::vector<int> c = {140,365,446};
    std::vector<int> d = {447,560,1200};
    s.stavi(a); s.stavi(b); s.stavi(c); s.stavi(d);
    pretraga(s,131);
    if(ispisRijeci=="Nema elementa") return true;
    return false;
}

void GlavnaTestFunkcija(){
    std::cout<<"Test praznog steka: ";
    if(testPrazanStek()) std::cout<<" OK"<<std::endl;
        else std::cout << " BAD" << std::endl;
    std::cout<<"Test stek sa svim praznnim vektorima: ";
    if(testStekSaSvimPraznimVek()) std::cout<<" OK"<<std::endl;
        else std::cout << " BAD" << std::endl;
    std::cout<< "Test stek sa nekim praznim vektorima: ";
    if(testStekSaNekimPraznimVek()) std::cout<<" OK"<<std::endl;
        else std::cout << " BAD" << std::endl;
    std::cout<<"Test pun stek: ";
    if(testPunStek()) std::cout<<" OK"<<std::endl;
        else std::cout << " BAD" << std::endl;
    std::cout<<"Test nije pronadjen element: ";
    if(testNemaElementa()) std::cout<<" OK"<<std::endl;
        else std::cout << " BAD" << std::endl;
}

int main() {
    GlavnaTestFunkcija();
    return 0;
}
