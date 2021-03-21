/* 
    TP 16/17 (Tutorijal 11, Zadatak 4)
	Autotestove napisao Haris Hasic. Sve primjedbe/zalbe, sugestije
	i pitanja slati na mail: hhasic2@etf.unsa.ba.
	
	Vrsit ce se provjera na prepisivanje tutorijala (na kraju semestra)
*/
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <iomanip>
class Tim{
	char ime_tima[20];
	int broj_odigranih, broj_pobjeda, broj_nerjesenih, broj_poraza, broj_datih, broj_primljenih, broj_poena;
public:
	Tim (const char ime[]);
	void ObradiUtakmicu(int broj_datih, int broj_primljenih);
	const char *DajImeTima() const{return ime_tima;}
	int DajBrojPoena() const{return broj_poena;}
	int DajGolRazliku() const{return broj_datih-broj_primljenih;};
	void IspisiPodatke() const;
};
Tim::Tim (const char ime[]){
		if(std::strlen(ime)>19) throw std::range_error("Predugacko ime tima");
		std::copy(ime, ime+19, ime_tima);
	broj_odigranih=0;broj_pobjeda=0;broj_nerjesenih=0;broj_poraza=0;broj_datih=0;broj_primljenih=0;broj_poena=0;
}
void Tim::ObradiUtakmicu(int broj_datih, int broj_primljenih) {
    if(broj_datih<0 || broj_primljenih<0) throw std::range_error("Neispravan broj golova");
    Tim::broj_datih += broj_datih;
    Tim::broj_primljenih += broj_primljenih;
    broj_odigranih++;
    if(broj_datih > broj_primljenih) {broj_pobjeda++; broj_poena += 3;}
    if(broj_datih == broj_primljenih) {broj_nerjesenih++; broj_poena += 1;}
    if(broj_datih < broj_primljenih) broj_poraza++;
}
void Tim::IspisiPodatke() const {
    std::cout << std::left << std::setw(20) << ime_tima << std::right << std::setw(4) << broj_odigranih << std::setw(4) << broj_pobjeda <<
    std::setw(4) << broj_nerjesenih << std::setw(4) << broj_poraza << std::setw(4) << broj_datih << std::setw(4) << broj_primljenih <<
    std::setw(4) << broj_poena << std::endl;
}

class Liga{
	const int max_br_timova;
	int broj_timova;
	Tim **liga;
public:
	explicit Liga(int velicina_lige):max_br_timova(velicina_lige),broj_timova(0),liga(nullptr){
		liga=new Tim*[velicina_lige];
		broj_timova=0;
		for(int i=0;i<velicina_lige;i++) liga[i]=nullptr;
	}
	explicit Liga(std::initializer_list<Tim> lista_timova):max_br_timova(lista_timova.size()),broj_timova(lista_timova.size()){
		const int broj(lista_timova.size());
		liga=new Tim*[broj];
		for(int i=0;i<broj;i++){liga[i]=nullptr;}
		auto it=lista_timova.begin();
		try{
			for(int i=0;i<broj;i++){
				liga[i]=new Tim(*it++);
				broj_timova=broj;
			}
		}catch(...){
			for(int i=0;i<broj_timova;i++) delete liga[i];
			delete[] liga;
			throw;
		}
	}
	~Liga(){
		for(int i=0;i<max_br_timova;i++){delete liga[i];}
		delete[] liga;
	}
	Liga(const Liga &a):max_br_timova(a.max_br_timova),broj_timova(a.broj_timova){
		liga = new Tim*[max_br_timova];
		for(int i=0;i<a.max_br_timova;i++){liga[i]=nullptr;}
		try{
			for(int i=0;i<a.max_br_timova;i++) {liga[i]=new Tim(*a.liga[i]);}
		}catch(...){
			for(int i=0;i<broj_timova;i++) delete liga[i];
			delete[] liga;
			throw;
		}
	}
	Liga(Liga &&a):max_br_timova(a.max_br_timova),broj_timova(a.broj_timova){liga=a.liga;}	
	Liga &operator=(const Liga &a){
		if(max_br_timova!=a.max_br_timova) {throw std::logic_error("Nesaglasni kapaciteti liga");}
		for(int i=0;i<max_br_timova;i++){
			delete liga[i];
			liga[i]=nullptr;
		}
		try{
			for(int i=0;i<max_br_timova;i++){
				liga[i]=new Tim(*a.liga[i]);
				broj_timova=a.broj_timova;
			}
		}catch(...){
			for(int i=0;i<broj_timova;i++) delete liga[i];
			delete[] liga;
			throw;
		}
		return *this;
	}
	Liga &operator=(Liga &&a){
		if(max_br_timova!=a.max_br_timova) {throw std::logic_error("Nesaglasni kapaciteti liga");}
		liga=a.liga;
		broj_timova=a.broj_timova;
		return *this;
	}
	void DodajNoviTim(const char ime_tima[]){
		if(broj_timova==max_br_timova) throw std::range_error("Liga popunjena");
		int brojac=0;
		int i=0;
		while(ime_tima[i]!='\n'){
			i++;
			brojac++;
		}
		if(brojac>20) throw std::range_error("Predugacko ime tima");
		liga[broj_timova++]=new Tim(Tim(ime_tima));
	}
	void RegistrirajUtakmicu(const char tim1[], const char tim2[], int rezultat_1, int rezultat_2){
		bool ima_prvi=false, ima_drugi=false;
		for(int i=0;i<broj_timova;i++){
			if(std::strcmp(liga[i]->DajImeTima(), tim1)==0) ima_prvi=true;
			if(std::strcmp(liga[i]->DajImeTima(), tim2)==0) ima_drugi=true;
		}
		if(ima_prvi==false || ima_drugi==false) throw std::logic_error("Tim nije nadjen");
		if(rezultat_1<0 || rezultat_2<0) throw std::range_error("Neispravan broj golova");
		
	}
};	
int main (){
Liga l1{"NK Zeljeznicar", "HSK Zrinjski", "FK Sarajevo"};
Liga l2{"NK Zeljeznicar", "HSK Zrinjski", "FK Sarajevo", "FK Krupa"};

try { l1=l2; }
catch(std::logic_error e) { std::cout << e.what() << std::endl; }
	return 0;
}