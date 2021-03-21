#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>


class Lik{
public:
	virtual ~Lik() {};
	virtual void IspisiSpecificnosti() const=0;
	virtual double DajObim() const=0;
	virtual double DajPovrsinu() const=0;
	void Ispisi(){
		IspisiSpecificnosti();
        std::cout << "Obim: " << DajObim(); 
        std::cout << " Povrsina: " << DajPovrsinu(); std::cout<<std::endl;
    }
};

class Krug:public Lik{
	double r;
public:
	Krug(double r){ if(r<=0) throw std::domain_error("Neispravni parametri");
		Krug::r = r; }
	void IspisiSpecificnosti() const {std::cout<<"Krug poluprecnika "<<r<<std::endl;}
	double DajObim() const {return 2*r*(4*atan(1));}
	double DajPovrsinu() const {return r*r*(4*atan(1));}
};

class Pravougaonik : public Lik{
	double a,b;
public: 
	Pravougaonik(double a, double b){ if(a<=0 ||b<=0)	throw std::domain_error("Neispravni parametri");
		Pravougaonik::a=a; Pravougaonik::b=b;}
	void IspisiSpecificnosti() const {std::cout<<"Pravougaonik sa stranicama duzine "<<a<<" i "<<b<<std::endl;}
	double DajObim() const {return 2*a + 2*b;}
	double DajPovrsinu()const {return a*b;}
};

class Trougao : public Lik{
	double a,b,c;
public:
	Trougao (double s1,double s2,double s3) : a(s1), b(s2), c(s3) { 
		if ( s1 <= 0 or s2 <= 0 or s3 <= 0 or (s1 + s2) <= s3 or (s1 + s3) <= s2 or (s2 + s3) <= s1 ) 
			throw std::domain_error("Neispravni parametri"); 
	}
	void IspisiSpecificnosti() const {std::cout<<"Trougao sa stranicama duzine "<<a<<", "<<b<<" i "<<c<<std::endl;}
	double DajObim() const {return a+b+c;}
	double DajPovrsinu()const {double s=(a+b+c)/2;	return std::sqrt(s*(s-a)*(s-b)*(s-c)); }
};

int main (){
	std::vector<std::shared_ptr<Lik>> likovi;
	std::cout<<"Koliko zelite likova: ";
	int n;
	std::cin>>n;
	for(int i=0;i<n;i++){
		std::cout<<"Lik "<<i+1<<": ";
		try{
			char c;
			std::cin>>c;
			if(c=='K'){
				double r;
				std::cin>>r;
				char pik= std::cin.peek();
				if(pik==',') likovi.push_back(std::make_shared<Krug>(-1));
				likovi.push_back(std::make_shared<Krug>(r));
			}
			if(c=='P'){
				double a;
				std::cin>>a;
				char zarez;
				std::cin>>zarez;
				double b;
				std::cin>>b;
				char pik = std::cin.peek();
				if(pik == ',') likovi.push_back(std::make_shared<Krug>(-1));
				likovi.push_back(std::make_shared<Pravougaonik>(a,b));
			}
			if(c=='T'){
				double a;
				std::cin>>a;
				char zarez;
				std::cin>>zarez;
				double b;
				std::cin>>b;
				std::cin>>zarez;
				double c;
				std::cin>>c;
				likovi.push_back(std::make_shared<Trougao>(a,b,c));
			}
			if(c!= 'T' && c!='P' && c!='K'){
				likovi.push_back(std::make_shared<Krug>(-1));
			}
		}catch(std::domain_error izu){
			std::cout<<"Pogresni podaci, ponovite unos!"<<std::endl;
			std::cin.ignore(1000, '\n');
			i--;
		}
	}
	std::cout<<std::endl;
	for(auto k: likovi) k->Ispisi();
	return 0;
}