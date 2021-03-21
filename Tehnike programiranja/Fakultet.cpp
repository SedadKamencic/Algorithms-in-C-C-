#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm> 


class ApstraktniStudent{
	std::string ime;
	std::string prezime;
	int broj_indeksa;
	int broj_polozenih;
	double prosjek;
public:
	ApstraktniStudent(std::string ime, std::string prezime, int broj_indeksa):ime(ime), 
		prezime(prezime), broj_indeksa(broj_indeksa), broj_polozenih(0), prosjek(5){}
	virtual ~ApstraktniStudent(){}	
	std::string DajIme() const {return ime;} 
	std::string DajPrezime() const{return prezime;}
	int DajBrojIndeksa() const{return broj_indeksa;}
	int DajBrojPolozenih() const{return broj_polozenih;}
	double DajProsjek() const{return prosjek;}
	void RegistrirajIspit(int ocj){
		if(ocj<5 || ocj>10) throw std::domain_error("Neispravna ocjena");
		if(!(ocj==5)) {
			double suma = prosjek * broj_polozenih;
			broj_polozenih++;
			prosjek=(suma + ocj) / broj_polozenih;
		}
	}
	void PonistiOcjene(){
		broj_polozenih=0;
		prosjek=5;
	}
	virtual void IspisiPodatke () const=0;
	virtual ApstraktniStudent* DajKopiju() const=0;
};
class StudentMaster : public ApstraktniStudent{
	int god_zavrsetka;
public:
	StudentMaster(std::string ime, std::string prezime,int broj_indeksa, int god_zavrsetka):
		ApstraktniStudent(ime,prezime,broj_indeksa), god_zavrsetka(god_zavrsetka){}
	void IspisiPodatke() const override {
		std::cout<<"Student master studija "<<DajIme()<<" "<<DajPrezime()<<", sa brojem indeksa "<<DajBrojIndeksa()<<","<<std::endl;
		std::cout<<"zavrsio bachelor studij godine "<<god_zavrsetka<<",ima prosjek "<<DajProsjek()<<"."<<std::endl;
	}
	ApstraktniStudent* DajKopiju() const override {return new StudentMaster(*this);}
};
class StudentBachelor: public ApstraktniStudent{
//
public:
	StudentBachelor(std::string ime, std::string prezime,int broj_indeksa):
		ApstraktniStudent(ime,prezime,broj_indeksa){}
	void IspisiPodatke() const override {
		std::cout<<"Student bachelor studija "<<DajIme()<<" "<<DajPrezime()<<", sa brojem indeksa "<<DajBrojIndeksa()<<","<<std::endl
		<<"ima prosjek "<<DajProsjek()<<"."<<std::endl;
	}
	ApstraktniStudent* DajKopiju() const override {return new StudentBachelor(*this);}
};

class Fakultet {
	std::vector < ApstraktniStudent* > vek;
	void  Test () const {
		for ( int i = 0; i < vek.size(); i++) if (vek[i] == nullptr) throw std::domain_error("jaja");
	}
public: 
	Fakultet(){}
	~Fakultet(){for(int i=0;i<vek.size();i++) delete vek[i];}
	Fakultet(const Fakultet &novi){
		for(int i=0;i<novi.vek.size();i++){
			vek.push_back(novi.vek[i]->DajKopiju());
		}
	}
	Fakultet(Fakultet &&novi){
		vek=novi.vek;
		novi.vek.resize(0);
	}

	Fakultet &operator = (const Fakultet &novi) {
		int brojst(0);
		for (int i(0); i<novi.vek.size(); i++) if(novi.vek[i] != nullptr) brojst++;
		for (int i = 0; i < vek.size(); i++) { delete vek[i]; vek[i] = nullptr; }
		int a = 0;
		vek.resize(brojst);
		for (int i = 0; i < novi.vek.size(); i++) {
			if (novi.vek[i] != nullptr) vek[a] = novi.vek[i]->DajKopiju();
			a++;
		}
	return *this;
	}
	Fakultet &operator =(Fakultet &&novi){
		for(int i(0);i<vek.size();i++){
			delete vek[i];
			vek[i]=nullptr;
		}
		vek.resize(0);
		std::swap(vek,novi.vek);
		return *this;
	}	
	void UpisiStudenta (std::string name, std::string last, int br) {
		for(int i=0;i<vek.size();i++) if((*vek[i]).DajBrojIndeksa()==br) throw std::logic_error("Student sa zadanim brojem indeksa vec postoji"); 
			vek.push_back(new StudentBachelor(name,last,br));
	}
	void UpisiStudenta (std::string name, std::string last, int br, int god) {
		for(int i=0;i<vek.size();i++){ if((*vek[i]).DajBrojIndeksa() == br) throw std::logic_error("Student sa zadanim brojem indeksa vec postoji");} 
		vek.push_back(new StudentMaster(name,last,br,god));
	}		
	void ObrisiStudenta(int broj_indeksa){
		int izuzetak(0);
		for(int i=0;i<vek.size();i++) {
			if((*vek[i]).DajBrojIndeksa()==broj_indeksa) {
				delete vek[i]; 
			}
		}
		if(broj_indeksa==izuzetak) throw std::logic_error("Student sa zadanim brojem indeksa ne postoji");
	}
	ApstraktniStudent &operator [] (int br) const {
		int izuzetak(0);
		for(int i(0); i<vek.size();i++){
			if((*vek[i]).DajBrojIndeksa()==br){
				izuzetak=1;  
				return *vek[i]; 
			} 
		}
		if(izuzetak==0) throw std::logic_error("Student sa zadanim brojem indeksa ne postoji");   
	}
	void IspisiSveStudente () const {
		std::vector<ApstraktniStudent*> vek2;
		vek2=vek;
		sort(vek2.begin(), vek2.end(), [] (ApstraktniStudent *a, ApstraktniStudent *b) {
			return a->DajBrojIndeksa() < b->DajBrojIndeksa(); });
		sort(vek2.begin(), vek2.end(), [] (ApstraktniStudent *a, ApstraktniStudent *b) {
			return a->DajProsjek() > b->DajProsjek(); });
		for(int i(0);i<vek2.size();i++){	
			vek2[i]->IspisiPodatke();}
	}
};


int main (){
try{
  
  Fakultet f1;
  f1.UpisiStudenta("Meho", "Mehic", 3412);
  f1.UpisiStudenta("Ibro", "Ibric", 3124, 2015);
  f1.ObrisiStudenta(3412);
  f1.IspisiSveStudente();
  
}catch(std::domain_error iyu){
	std::cout<<iyu.what();
}catch(...){
	std::cout<<"oke";
}

	return 0;
}
