/* 
    TP 16/17 (Tutorijal 7, Zadatak 10)
	Autotestove pisao Elmir Hodzic. Sva pitanja,
	prijave gresaka i sugestije slati na mail
	ehodzic3@etf.unsa.ba
	
	Vrsit ce se provjera na prepisivanje tutorijala.
*/
#include <iostream>
#include <functional>

std::function<int(int)> IteriranaFunkcija(int f(int), int n){
	return
	[f,n](int x){
		for(int i=0;i<n;i++){
			x=f(x);
		}
	return x;
	};
}

int main (){
  	std::cout << IteriranaFunkcija([](int x) { return x * x; }, 2)(3) << std::endl;
	return 0;
}