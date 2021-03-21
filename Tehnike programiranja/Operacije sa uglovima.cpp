#include <iostream>
#include <cmath>
#include <stdexcept>
#include <iomanip>

const double PI=4*atan(1.);

class Ugao{
	int deg, min, sec;
public:
template<typename TIP>
	void ReducirajUgao(TIP &stepeni, TIP &minute, TIP &sekunde){
		while(sekunde>=60){sekunde-=60; minute++;};
		while(sekunde<0){sekunde+=60; minute--;};
		
		while(minute>=60){minute-=60; stepeni++;};
		while(minute<0){minute+=60; stepeni--;};
		
		while(stepeni>=360){stepeni-=360;};
		while(stepeni<0){stepeni+=360;};
	}
	void ReducirajRadijane(double &radijani){
		while(radijani>2*PI){radijani-=2*PI;}
		while(radijani<0){radijani+=2*PI;}
	}	
	
	Ugao(double radijani=0){
		ReducirajRadijane(radijani);
		int tmp=radijani*((180*3600)/PI);
		sec=tmp%60;
		
		double pom{tmp/60.};
		min=int(pom)%60;
		
		deg=(min%60)/60;
	}
	Ugao(int stepeni, int minute, int sekunde){
		ReducirajUgao(stepeni,minute,sekunde);
		deg=stepeni;	min=minute;	sec=sekunde;
	}
	
	void Postavi(double radijani){
		ReducirajRadijane(radijani);
		int tmp=radijani*((180*3600)/PI);
		sec=tmp%60;
		
		double pom{tmp/60.};
		min=int(pom)%60;
		
		deg=(min%60)/60;
	}
	void Postavi(double stepeni, double minute,double sekunde){
		ReducirajUgao(stepeni,minute,sekunde);
		deg=stepeni;	min=minute;	sec=sekunde;
	}
	
	double DajRadijane() const{
		return (deg  + (min/60.) + (sec/3600.)) * (PI/180);
	}
	
	void OcitajKlasicneJedinice(int & stepeni, int & minute, int & sekunde){
		stepeni=DajStepene();
		minute=DajMinute();
		sekunde=DajSekunde();
	};
	int DajStepene() const	{return deg;};
	int DajMinute() const	{return min;}
	int DajSekunde() const	{return sec;};
	
	void Ispisi() const{std::cout<<std::fixed<<std::setprecision(5)<<DajRadijane();};
	
	void IspisiKlasicno() const{
		std::cout<<DajStepene()<<"deg "<<DajMinute()<<"min "<<DajSekunde()<<"sec";
	};
	
	Ugao & SaberiSa(const Ugao & u){
		deg+=u.deg;	min+=u.min;	sec+=u.sec;
		ReducirajUgao(deg, min, sec);
		return *this;
	}
	Ugao & PomnoziSa(double x){
		deg*=x;
		min*=x;
		sec*=x;
		this->Postavi(deg,min,sec);
		return *this;
	}
	friend Ugao ZbirUglova(const Ugao & u1, const Ugao & u2);
	friend Ugao ProduktUglaSaBrojem(const Ugao & u, double x);
};

Ugao ZbirUglova(const Ugao & u1, const Ugao & u2){
	Ugao u3;
	u3.deg=u1.deg+u2.deg;	u3.min=u1.min+u2.min;	u3.sec=u1.sec+u2.sec;
	return u3;
}

//Ugao ProduktUglaSaBrojem(const Ugao & u, double x);
int main (){
Ugao u(50, 25, 12), v(500, 300, 200), f(50, 30, 10);
u.SaberiSa(1);
u.IspisiKlasicno(); std::cout << std::endl;
u.SaberiSa(1).SaberiSa(f).Ispisi();
	return 0;
}

/*
const double PI=4*atan(1);

class Ugao{
	double rad;
public:
	void ReducirajUgao(int &stepeni, int &minute, int &sekunde){
		while(sekunde>=60){sekunde-=60; minute++;};
		while(sekunde<0){sekunde+=60; minute--;};
		
		while(minute>=60){minute-=60; stepeni++;};
		while(minute<0){minute+=60; stepeni--;};
		
		while(stepeni>=360){stepeni-=360;};
		while(stepeni<0){stepeni+=360;};
	}
	void ReducirajRadijane(double &radijani){
		while(radijani>2*PI){radijani-=2*PI;}
		while(radijani<0){radijani+=2*PI;}
	}
	
	Ugao(double radijani=0){
		ReducirajRadijane(radijani);
		rad=radijani;
	}
	Ugao(int stepeni, int minute, int sekunde){
		ReducirajUgao(stepeni, minute, sekunde);
		rad=(stepeni + minute/60 + sekunde/3600) * PI/180;	
	}
	void Postavi(double radijani){
		ReducirajRadijane(radijani);
		rad=radijani;
	}
	void Postavi(int stepeni, int minute, int sekunde){
		while(stepeni>360){stepeni-=360;}
		while(stepeni<0){stepeni+=360;}
		rad=(stepeni + minute/60 + sekunde/3600) * PI/180;	
	}
	double DajRadijane() const {return rad;};
	void OcitajKlasicneJedinice (int & stepeni, int & minute, int & sekunde){
		stepeni = DajStepene();
		minute = DajMinute();
		sekunde = DajSekunde();
	}
	int DajStepene() const { return rad * 180/PI; }
    int DajMinute() const { return (DajRadijane()*180/PI-DajStepene())*60; }
    int DajSekunde() const {
        long double sekunde(rad*180/PI-DajStepene());
        sekunde*=60;
        sekunde-=DajMinute();
        sekunde*=60;
        return sekunde;
    }
	void Ispisi() const{std::cout<<std::fixed<<std::setprecision(5)<<DajRadijane();};
	void IspisiKlasicno() const{
		std::cout<<DajStepene()<<"deg "<<DajMinute()<<"min "<<DajSekunde()<<"sec";
	}
	Ugao & SaberiSa(const Ugao & u){
		double pom(rad);
        pom += u.rad;
        ReducirajRadijane(pom);
        rad = pom;
        return *this;
	}
	Ugao & PomnoziSa(double x);
	friend Ugao ZbirUglova(const Ugao & u1, const Ugao & u2);
	friend Ugao ProduktUglaSaBrojem(const Ugao & u, double x);
};

Ugao ZbirUglova(const Ugao & u1, const Ugao & u2){
	Ugao pom;
	pom.rad=u1.rad+u2.rad;
	ReducirajRadijane(pom.rad);
	return pom;
}*/