/* 
    TP 16/17 (Tutorijal 7, Zadatak 7)
	Autotestove pisao Elmir Hodzic. Sva pitanja,
	prijave gresaka i sugestije slati na mail
	ehodzic3@etf.unsa.ba
	
	Vrsit ce se provjera na prepisivanje tutorijala.
*/
#include <iostream>
#include <set>
#include <list>
#include <vector>
#include <string>

template<typename TIP>
std::set<TIP> Unija(std::set<TIP> set1, std::set<TIP> set2){
	std::set<TIP> novi;
	for(auto it1=set1.begin(); it1!=set1.end(); it1++){
		novi.insert(*it1);
	}
	for(auto it2=set2.begin(); it2!=set2.end(); it2++){
		novi.insert(*it2);
	}
	return novi;
}

template<typename TIP>
std::set<TIP> Presjek(std::set<TIP> set1, std::set<TIP> set2){
	std::set<TIP> novi;
	
	for(auto it1=set1.begin(); it1!=set1.end(); it1++){
		for(auto it2=set2.begin(); it2!=set2.end();it2++){
			if(*it1==*it2){
				novi.insert(*it1);
			}
		}
	}
	return novi;
}
int main (){
	std::set<std::string> string1{"War", "Never", "Changes"};
	std::set<std::string> string2{"In", "Mojave", "Desert", "War", "Never", "Changes"};
	
	std::cout<<"Presjek dva skupa glasi: "<<std::endl;
	std::set<std::string> presjek{Presjek(string1, string2)};
	for(auto k: presjek) std::cout<<k<<" ";
	std::cout<<"\nUnija dva skupa glasi: "<<std::endl;
	std::set<std::string> unija{Unija(string1, string2)};
	for(auto l: unija) std::cout<<l<<" ";
	return 0;
}