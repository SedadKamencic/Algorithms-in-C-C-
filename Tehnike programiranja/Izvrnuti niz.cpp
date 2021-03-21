#include <iostream>
#include <type_traits>
#include <vector>
#include <stdexcept>
#include <new>

template<typename NekiTip>
auto KreirajIzvrnutiNiz(NekiTip poc, NekiTip kraj) -> typename std::remove_reference<decltype(*poc)>::type* {
		int brojac{};
		NekiTip temp{poc};
		while(temp!=kraj){
			brojac++; temp++;
		}
		typename std::remove_reference<decltype(*poc)>::type *niz{new typename std::remove_reference<decltype(*poc)>::type [brojac]};	//Mozda i ne uspije
		for(int i(brojac-1);i>=0;i--){
			niz[i]=*poc;
			poc++;
			
		}
		return niz;
}
int main (){
	std::cout<<"Koliko zelite elemenata: ";
	int n;
	std::cin>>n;
	try{
		std::vector<double> vek(n);		//Mozda ne uspije
		std::cout<<"Unesite elemente: ";
		for(int i=0;i<n;i++){
			double broj;
			std::cin>>broj;
			vek.push_back(broj);
	}
	double *pom(KreirajIzvrnutiNiz(vek.begin(), vek.end()));
	auto rezultat(pom);
	
	std::cout<<"Kreirani niz: ";
	for(int i=0;i<n;i++){
		std::cout<< *rezultat <<" ";
		rezultat++;
	}
	delete[] pom;
	}catch(std::bad_alloc){
		std::cout<<"Nedovoljno memorije!";
	}
	return 0;
}