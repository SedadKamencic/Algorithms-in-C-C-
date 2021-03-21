#include <iostream>
#include <stdexcept>
#include <cmath>

class Sat{
    int sati, minute, sekunde;
    static bool DaLiJeIspravno(int x,int y, int z) {
        if((x>=0 && x<24) && (y>=0 && y<60) && (z>=0 && z<60)) return true;
        return false;
    }    
public:
    Sat(int x=0,int y=0, int z=0){PostaviNormalizirano(x,y,z);}
    void Postavi(int x, int y, int z) {
        if(DaLiJeIspravno(x,y,z)){
		    Sat::sati = x; Sat::minute = y; Sat::sekunde = z;
        }else{
            throw std::domain_error("Neispravno vrijeme");
        }
	}
	int DajSate() const {return Sat::sati;}
	int DajMinute() const {return Sat::minute;}
	int DajSekunde()const {return Sat::sekunde;}
	void PostaviNormalizirano(int x, int y, int z){
		if(DaLiJeIspravno(x,y,z)){
			Sat::sati=x; Sat::minute=y; Sat::sekunde=z;}
		else{
			int pomocna=0;
			//int rezultat=0;
			if(z>59){pomocna=z/60; z=z-pomocna*60; y=y+pomocna;}
			if(z<0) {pomocna=std::abs(z)/60; z=(pomocna+1)*60+z; y=y-pomocna-1;}
			
			if(y>59) {pomocna=y/60; y=y-pomocna*60; x=x+pomocna;}
			if(y<0) {pomocna=std::abs(y)/60; y=(pomocna+1)*60+y; x=x-pomocna-1;}
			
			if(x>23) {pomocna=x/24; x=x-pomocna*24;}
			if(x<0) {pomocna=std::abs(x)/24; x=(pomocna+1)*24+x;}
			
			Sat::sekunde=z; Sat::minute=y; Sat::sati=x;
		}
		
	}
	
	friend std::ostream &operator << (std::ostream &tok, const Sat &s); 
	//++a odnosno --a	
	friend Sat &operator ++(Sat &s);
	friend Sat &operator --(Sat &s);
	//a++ odnosno a--
	friend Sat operator ++(Sat &s, int);
	friend Sat operator --(Sat &s, int);
	friend Sat &operator +=(Sat &s, int n);
	friend Sat &operator -=(Sat &s, int n);
	friend int operator -(Sat v1, Sat v2);
	friend Sat operator +(const Sat &s, int n);
	friend Sat operator -(const Sat &s, int n);
	friend Sat PretvoriUSate(int n);
};
static bool DaLiJeIspravno(int x,int y, int z) {
        if((x>=0 && x<24) && (y>=0 && y<60) && (z>=0 && z<60)) return true;
        return false;
    }    
Sat PretvoriUSate(int n){
	Sat s;
	int pom;
	s.sati=n/3600;
	pom=(n-s.sati*3600);
	s.minute=pom/60;
	pom=(pom-s.minute*60);
	s.sekunde=pom;
	return s;
}
std::ostream &operator << (std::ostream &tok, const Sat &s){
    if(s.DajSate()<10) tok<<"0";
        tok<<s.DajSate()<<":";
    if(s.DajMinute()<10) tok<<"0";
        tok<<s.DajMinute()<<":";
    if(s.DajSekunde()<10) tok<<"0";
        tok<<s.DajSekunde();
    return tok;    
}
Sat &operator ++(Sat &s){
	s.sekunde++;
	s.PostaviNormalizirano(s.DajSate(), s.DajMinute(), s.DajSekunde());
	return s;
}
Sat operator ++(Sat &s, int){
	Sat pom(s);
	s.sekunde++;
	s.PostaviNormalizirano(s.sati,s.minute,s.sekunde);
	return pom;
}
Sat &operator --(Sat &s){
	s.sekunde--;
	s.PostaviNormalizirano(s.DajSate(), s.DajMinute(), s.DajSekunde());
	return s;
}
Sat operator --(Sat &s, int){
	Sat pom(s);
	s.sekunde--;
	s.PostaviNormalizirano(s.sati,s.minute,s.sekunde);
	return pom;
}
Sat &operator +=(Sat &s, int n){
	s.sekunde+=n;
	s.PostaviNormalizirano(s.sati,s.minute,s.sekunde);
	return s;
}
Sat &operator -=(Sat &s, int n){
	Sat s3=PretvoriUSate(n);
	s.sati-=s3.sati; s.minute-=s3.minute; s.sekunde-=s3.sekunde;
	s.PostaviNormalizirano(s.sati, s.minute, s.sekunde);
	return s;
}
int operator -(Sat v1, Sat v2){
	int rezultat = (v1.sati-v2.sati)*3600 + (v1.minute-v2.minute)*60 + (v1.sekunde-v2.sekunde);
	return rezultat;
}
Sat operator +(const Sat &s, int n){
	Sat s3(s);
	s3.sekunde+=n; 
	s3.PostaviNormalizirano(s3.sati,s3.minute,s3.sekunde);
	return s3;
}
Sat operator -(const Sat &s, int n){
	Sat s3(s);
	s3.sekunde-=n; 
	s3.PostaviNormalizirano(s3.sati,s3.minute,s3.sekunde);
	return s3;	
}

int main (){
	std::cout<<"Zdravo, unesite vrijeme koje zelite modelirati: ";
	int x,y,z;
	while(1){
		std::cin>>x>>y>>z;
		if(DaLiJeIspravno(x,y,z)) break;
		std::cout<<"Neispravno vrijeme!"<<std::endl;
		std::cout<<"Unesite ispravno vrijeme: ";
	}
	Sat s(x,y,z);
	std::cout<<"\nVrijeme koje ste unijeli glasi: "<<s;
	std::cout<<"\nUnesite broj sekundi za kolik zelite pomjeriti vrijeme: ";
	int n;	
	while(1){
		std::cin>>n;
		if(n>0) break;
		std::cout<<"\nVrijeme ne moze biti negativno! Unesite ponovo: ";
	}
	s+=n;
	std::cout<<"\nVrijeme pomjereno za "<<n<<" sekundi glasi: "<<s;
	s-=n; s-=n;
	std::cout<<"\nVrijeme pomjereno za -"<<n<<" sekundi glasi: "<<s;
  return 0;
}