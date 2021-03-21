//NA 2017/2018: Zadaća 1, Zadatak 1
#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <iomanip>

class Vector{
    std::vector<double> vek;
    void TestIndexa(int i) const { if(i<=0 || i>vek.size()) throw std::range_error("Invalid index");}
public:
    explicit Vector(int n) {
        if(n<=0) throw std::range_error("Bad dimension");
        vek.resize(n);
    }
    Vector(std::initializer_list<double> l){ if(l.size()==0) throw std::range_error("Bad dimension");
        vek.resize(l.size());
            std::copy(l.begin(), l.end(), vek.begin());
    }
    int NElems() const { return vek.size(); }
    double &operator[](int i) { return vek[i]; }
    double operator[](int i) const { return vek[i]; } 
    double &operator() (int i) { TestIndexa(i);  return vek[i-1]; }
    double operator() (int i) const { TestIndexa(i); return vek[i-1]; }
    double Norm() const;
    friend double VectorNorm(const Vector &v) { return v.Norm();    }
    double GetEpsilon() const {
        return (this->Norm()) * (std::numeric_limits<double>::epsilon());   
    }
    void Print(char separator = '\n', double eps=-1) const;
    friend void PrintVector(const Vector &v, char separator='\n', double eps=-1) {
        v.Print(separator,eps);
    }
    friend Vector operator +(const Vector &v1, const Vector &v2);
    Vector operator +=(const Vector &v);
    friend Vector operator -(const Vector &v1, const Vector &v2);
    Vector &operator -=(const Vector &v);
    friend Vector operator *(double s, const Vector &v);
    friend Vector operator *(const Vector &v, double s);
    Vector &operator *=(double s);
    friend double operator *(const Vector &v1, const Vector &v2);
    friend Vector operator /(const Vector &v, double s);
    Vector &operator /=(double s);
};
double Vector::Norm() const {
    double suma(0);
    for(int i(0); i<vek.size();i++)
        suma+=(vek[i]*vek[i]);
    return std::sqrt(suma);    
}
void Vector::Print(char separator, double eps) const {
    if(std::abs(eps-(-1)) <= 1e-10){
        eps = this->GetEpsilon();
        if(std::abs(vek[0]) < eps) std::cout<<0;
        else std::cout << vek[0];
        for(int i(1); i<vek.size(); i++)
            if(std::abs(vek[i]) < eps) std::cout<<separator<<0;
            else std::cout <<separator<< vek[i];
    }
    else{
        if(std::abs(vek[0]) < eps) std::cout<<0;
        else std::cout << vek[0];
        for(int i(1); i<vek.size(); i++)
            if(std::abs(vek[i]) < eps) std::cout<<separator<<0;
            else std::cout <<separator<< vek[i];
    }        
    if(separator=='\n') std::cout<<separator;
}
Vector operator +(const Vector &v1, const Vector &v2){
    if(v1.vek.size()!=v2.vek.size()) throw std::domain_error("Incompatible formats");
    Vector v3(v1.vek.size());
    v3+=v1;
    v3+=v2;
    return v3;
}
Vector Vector::operator +=(const Vector &v){
    if(v.vek.size()!=vek.size()) throw std::domain_error("Incompatible formats");
    for(int i(0);i<v.vek.size();i++)
        vek[i]+=v[i];
    return *this;    
}
Vector operator -(const Vector &v1, const Vector &v2){
    if(v1.vek.size()!=v2.vek.size()) throw std::domain_error("Incompatible formats");
    Vector v3(v1.vek.size());
    v3=v1;
    v3-=v2;
    return v3;        
}
Vector& Vector::operator -=(const Vector &v){
    if(v.vek.size()!=vek.size()) throw std::domain_error("Incompatible formats");
    for(int i(0);i<v.vek.size();i++)
        vek[i]-=v[i];
    return *this;        
}
Vector operator *(double s, const Vector &v){
    Vector rez(v.vek.size());
    rez+=v; 
    rez*=s;
    return rez;
}
Vector operator *(const Vector &v, double s){
    Vector rez(v.vek.size());
    rez+=v; rez*=s;
    return rez;        
}
Vector& Vector::operator *=(double s){
    for(int i(0);i<this->NElems();i++)
        vek[i]=vek[i]*s;
    return *this;
}
double operator *(const Vector &v1, const Vector &v2){
    if(v1.vek.size()!=v2.vek.size()) throw std::domain_error("Incompatible formats");
    double rez(0);
    for(int i(0);i<v1.vek.size();i++)
        rez+=(v1[i]*v2[i]);
    return rez;    
}
Vector operator /(const Vector &v, double s){
    Vector rez(v.vek.size());
    for(int i(0);i<v.vek.size();i++){
        if((s-0)<1e-10) throw std::domain_error("Division by zero");
        rez[i]=v[i]/s;
    }
    return rez;
}
Vector& Vector::operator /=(double s){
    if((s-0) < 1e-10) throw std::domain_error("Division by zero");
    for(int i(0);i<vek.size();i++)
        vek[i]/=s;
    return *this;    
}

class Matrix{
    std::vector<std::vector<double>> mat;
public:    
    Matrix(int m, int n);
    Matrix(const Vector &v);
    Matrix(std::initializer_list<std::vector<double>> l);
    int NRows() const {return mat.size();} 
    int NCols() const {return mat[0].size();}
    double *operator[](int i) {return &mat[i][0];}
    const double *operator[](int i) const {return &mat[i][0];}
    double &operator()(int i, int j) {
        if(i<=0 || i>mat.size() || j<=0 || j>mat[0].size()) throw std::range_error("Invalid index");
        return mat[i-1][j-1];
    }
    double operator()(int i, int j) const {
        if(i<=0 || i>mat.size() || j<=0 || j>mat[0].size()) throw std::range_error("Invalid index");
        return mat[i-1][j-1];
    }
    double Norm() const;
    friend double MatrixNorm(const Matrix &m){
        return m.Norm();
    }
    double GetEpsilon() const{
        return this->Norm() * std::numeric_limits<double>::epsilon();
    }
    void Print(int width = 10, double eps = -1) const;
    friend void PrintMatrix(const Matrix &m, int width = 10, double eps = -1){
        m.Print(width,eps);
    }
    friend Matrix operator +(const Matrix &m1, const Matrix &m2);
    Matrix &operator +=(const Matrix &m);
    friend Matrix operator -(const Matrix &m1, const Matrix &m2);
    Matrix &operator -=(const Matrix &m);
    friend Matrix operator *(double s, const Matrix &m);
    friend Matrix operator *(const Matrix &m, double s){
        return s*m;
    }
    Matrix &operator *=(double s);
    friend Matrix operator *(const Matrix &m1, const Matrix &m2);
    Matrix &operator *=(const Matrix &m);
    friend Vector operator *(const Matrix &m, const Vector &v);
    friend Matrix Transpose(const Matrix &m);
    void Transpose();
};
Matrix::Matrix(int m, int n){
    if(m<=0 || n<=0) throw std::range_error("Bad dimension");
    mat.resize(m);
    for(int i(0);i<m;i++)
        mat[i].resize(n);
}
Matrix::Matrix(std::initializer_list<std::vector<double>> l): mat(l){
    if(mat.size()==0 || mat[0].size()==0) throw std::range_error("Bad dimension");
    for(int i(0);i<mat.size();i++)
        if(mat[i].size()!=mat[0].size()) throw std::logic_error("Bad matrix");
}
Matrix::Matrix(const Vector &v){
    mat.resize(1);
    mat[0].resize(v.NElems());
    for(int i(0);i<v.NElems();i++)
        mat[0][i]=v[i];
}
double Matrix::Norm() const{
    double suma(0);
    for(int i(0);i<this->NRows();i++)
        for(int j(0);j<this->NCols();j++)
            suma+=mat[i][j]*mat[i][j];
    suma=std::sqrt(suma);
    return suma;
}
void Matrix::Print(int width, double eps) const{
    if(eps<0) eps = this->GetEpsilon();
    for(int i(0);i<this->NRows();i++){
        for(int j(0);j<this->NCols();j++){
            if(std::abs(mat[i][j]) <= eps) std::cout<<std::setw(width)<<0;
            else std::cout<<std::setw(width)<<mat[i][j];
        }
        std::cout<<"\n";
    }    
}
Matrix operator +(const Matrix &m1, const Matrix &m2){
    if(m1.NRows()!=m2.NRows()) throw std::domain_error("Incompatible formats");
    Matrix rez(m1.NRows(),m1.NCols());
    rez+=m1;
    rez+=m2;
    return rez;        
}
Matrix& Matrix::operator +=(const Matrix &m){
    if(this->NRows()!=m.NRows()) throw std::domain_error("Incompatible formats");
    for(int i(0);i<m.NRows();i++)
        for(int j(0);j<m.NCols();j++)
            mat[i][j]+=m[i][j];
    return *this;        
}
Matrix& Matrix::operator -=(const Matrix &m){
    if(this->NRows()!=m.NRows()) throw std::domain_error("Incompatible formats");
    for(int i(0);i<m.NRows();i++)
        for(int j(0);j<NCols();j++)
            mat[i][j]-=m[i][j];
    return *this;        
}
Matrix operator -(const Matrix &m1, const Matrix &m2){
    if(m1.NRows()!=m2.NRows()) throw std::domain_error("Incompatible formats");
    Matrix rez(m1.NRows(),m1.NCols());
    rez+=m1;
    rez-=m2;
    return rez;  
}
Matrix operator *(double s, const Matrix &m){
    Matrix rez(m.NRows(), m.NCols());
    for(int i(0);i<m.NRows();i++)
        for(int j(0);j<m.NCols();j++)
            rez[i][j]=m[i][j]*s;
    return rez;        
}
Matrix& Matrix::operator *=(double s){
    for(int i(0);i<this->NRows();i++)
        for(int j(0);j<this->NCols();j++)
            mat[i][j]*=s;
    return *this;        
}
Matrix operator *(const Matrix &m1, const Matrix &m2){
    if(m1.NCols()!=m2.NRows()) throw std::domain_error("Incompatible formats");
    Matrix rez(m1.NRows(),m2.NCols());
    double suma_red(0);
    for(int i(0);i<m1.NRows();i++){
        for(int j(0);j<m2.NCols();j++){
            suma_red=0;
            for(int k(0);k<m1.NCols();k++)
                suma_red+=m1[i][k]*m2[k][j];
            rez[i][j]=suma_red;
        }
    }
    return rez;
}
Matrix& Matrix::operator *=(const Matrix &m){
    if(this->NCols()!=m.NRows()) throw std::domain_error("Incompatible formats");
    Matrix rez(this->NRows(),m.NCols());
    double suma_red(0);
    for(int i(0);i<this->NRows();i++){
        for(int j(0);j<m.NCols();j++){
            suma_red=0;
            for(int k(0);k<this->NCols();k++)
                suma_red+=mat[i][k]*m[k][j];
            rez[i][j]=suma_red;
        }
    }
    for(int i(0);i<rez.NRows();i++)
        for(int j(0);j<rez.NCols();j++)
            mat[i][j]=rez[i][j];
    return *this;
}
Vector operator *(const Matrix &m, const Vector &v){
    if(m.NCols()!=v.NElems()) throw std::domain_error("Incompatible formats");
    Vector rez(m.NRows());
    double suma(0);
    for(int i(0);i<m.NRows();i++){
        suma=0;
            for(int k(0);k<m.NCols();k++)
                suma+=(m[i][k]*v[k]);
            rez[i]=suma;
    }    
    return rez;
}
Matrix Transpose(const Matrix &m){
    Matrix rez(m.NCols(), m.NRows());
    for(int i(0);i<m.NCols();i++)
        for(int j(0);j<m.NRows();j++)
            rez[i][j]=m[j][i];
    return rez;
}
void Matrix::Transpose(){
    if(this->NRows()==this->NCols()){
        for(int i(0);i<this->NRows();i++)
            for(int j(i+1);j<this->NCols();j++)
                std::swap(mat[i][j],mat[j][i]);
    }
    else{
        std::vector<std::vector<double>> rez;
        std::vector<double> vek1;
        for(int i(0);i<this->NCols();i++){
            vek1.resize(0);
            for(int j(0);j<this->NRows();j++){
                vek1.push_back(mat[j][i]);
            }
            rez.push_back(vek1);
        }    
        this->mat=rez;
    }
}

int main (){    //pod komentarom su konstrukcije koje izazivaju izuzetke
try{
    std::cout<<std::endl<<".......Test file 1 initialized........"<<std::endl;
    //Vector vek1(-5);
    //Vector vek2{};
    Vector vek1(2);
    Vector vek2{1,2,3,4,5};
    std::cout<<"Prije izmjene velicine vektora su: "<<vek1.NElems()<<" "<<vek2.NElems();
    std::cout<<" Sadrzaj vektora je: "<<std::endl;
    vek1.Print('*');
    std::cout<<"   ";
    PrintVector(vek2, '|');
    
    std::cout<<"\n"<<vek1[1]<<std::endl;
    std::cout<<vek2[3]<<std::endl;
    //vek1(6)=11;
    vek1(1)=5;
    vek1.Print(' ');
    
    std::cout<<"\nNorma za vektor 1: "<<vek1.Norm();
    std::cout<<"\nNorma za vektor 2: "<<VectorNorm(vek2);
    
    std::cout<<"\nEpsiloni : "<<vek1.GetEpsilon()<<" i "<<vek2.GetEpsilon();
    
    //std::cout<<"\nSabiranje razlicitih dimenzija: "; vek1+=vek2;
    Vector vek3{2,4,6,8,10};
    std::cout<<"\nRazlika je: "; (vek3-vek2).Print(' ');
    std::cout<<"\nZbir je: "; (vek2+vek3).Print(' ');
    
    std::cout<<"\nSkalarni produkt vektora je: "<<(vek2*vek3);
    std::cout<<"\nMnozenje vektora skalarom: (vek*a): "; (vek2*5).Print('|');
    std::cout<<"\nMnozenje vektora skalarom: (a*vek): "; (3*vek2).Print('|');
    //(vek2*0).Print('o');
    std::cout<<"\n\n Test metoda klase: "<<std::endl;
    std::cout<<"\nDjeljenje vektora skalarom: "<<std::endl;
    PrintVector(vek3/2);
    
    vek2+=vek3;
    vek2-=vek3;
    std::cout<<std::endl;
    vek2.Print(' ');
    vek3/=5;
    std::cout<<"   ";
    vek3.Print('\'');
    
    std::cout<<std::endl<<"......End of test file for Vector....."<<std::endl;
}catch(std::domain_error izu){
    std::cout<<izu.what()<<std::endl;
}catch(std::range_error izu){
    std::cout<<izu.what()<<std::endl;
}
try{        //pod komentarom su oznaceni pozivi koji bacaju izuzetak
    std::cout<<std::endl<<".......Test file 2 initialized........"<<std::endl;
    //Matrix mat1(-1,2);
    //Matrix mat1{{5,2},{2,5},{6,4,3}};
    //Matrix mat1{{},{},{}};
    //Matrix mat1{};
    std::cout<<"Pozivanje konstruktora koji prima vektor: "<<std::endl;
    Vector vek{1,2,3,4,5};
    Matrix mat1(vek);
    mat1.Print(2);
    
    Matrix mat2{{1,2,3},{4,5,6},{7,8,9}};
    std::cout<<"\nKonstrukcija kvadratne matrice: "<<std::endl;
    PrintMatrix(mat2,2);
    std::cout<<"Broj redova matrice 2: "<<mat2.NRows()<<std::endl;
    std::cout<<"Broj kolona matrice 2: "<<mat2.NCols()<<std::endl;
    
    
    std::cout<<"Test operatora []: "<<std::endl;
    std::cout<<mat2[1][2]<<std::endl;
    mat2[1][2]=mat2[1][2]*2;
    std::cout<<mat2[1][2]<<std::endl;
    mat2[1][2]=6;
    std::cout<<mat2[1][2]<<std::endl;
    
    std::cout<<"Test operatora () i bacanje izuzetaka: "<<std::endl;
    std::cout<<mat2(1,1)<<std::endl;
    /*std::cout<<"Ocekivani izuzetak: "<<std::endl;
    try{
        std::cout<<mat2(-1,0)<<std::endl;
    }catch(std::logic_error izu){
        std::cout<<izu.what()<<std::endl;
    }*/
    //std::cout<<"Prvi element je : "<<mat1(-5,3)<<std::endl;
    std::cout<<"Prvi element je : "<<mat1(1,1)<<std::endl;
    mat1(1,1)=12.3;
    std::cout<<"Nakon izmjene: "<<mat1(1,1)<<std::endl;
    const double a= mat2(1,1);
    std::cout<<"a= "<<a<<std::endl;
    mat2(1,1)=1;
    
    std::cout<<"Norma : "<<mat2.Norm()<<std::endl;
    std::cout<<"Epsilon : "<<mat2.GetEpsilon()<<std::endl;
    
    
    std::cout<<"Testiranje prijateljskih funkcija: "<<std::endl;
    Matrix mat3{{2,4,6},{8,10,12},{14,16,18}};
    std::cout<<"Razlika je: "<<std::endl;
    //PrintMatrix(mat3-mat1);
    PrintMatrix(mat3-mat2,3);
    
    std::cout<<"Zbir je: "<<std::endl;
    //PrintMatrix(mat3+mat1);
    PrintMatrix(mat3+mat2,3);    
    std::cout<<"Mnozenje skalarom: (mat*a)"<<std::endl;
    PrintMatrix(mat3*2);
    
    std::cout<<"Mnozenje skalarom: (a*mat)"<<std::endl;
    PrintMatrix(0.5 * mat3);
    std::cout<<"Mnozenje dviju matrica: "<<std::endl;
    try{
        PrintMatrix(mat1*mat3);
    }catch(...){
        std::cout<<"Greska!"<<std::endl;
    }
    std::cout<<"Mnozenje dvije kvadratne matrice glasi: "<<std::endl;
    //(mat3*mat1).Print(5);
    (mat3*mat2).Print();
    
    std::cout<<"\nTest metoda klase: "<<std::endl;
    mat3+=mat2;
    //mat3+=mat1;
    mat3-=mat2;
    //mat3-=mat2;
    std::cout<<"Matrica 3 mora biti ista nakon izmjena: "<<std::endl;
    mat3.Print(3);
    std::cout<<"Mnozenje dvije matrice: "<<std::endl;
    mat3*=mat2;
    std::cout<<"Matrice pomnozena sa skalarom: "<<std::endl;
    mat3*=3.2;
    mat3.Print(8);
    std::cout<<"Mat-vektor pomnozen sa skalarom: "<<std::endl;
    (mat2*0.5).Print(6);
    
    
    Matrix mat4{{2,4},{6,8},{10,12}};
    std::cout<<"Mnozenje dvije matrice formata 3x3 i 3x2: "<<std::endl;
    (mat3*mat4).Print();
    
    std::cout<<"Transponovana kvadratna matrica :"<<std::endl;
    Transpose(mat3);
    PrintMatrix(mat3);
    
    std::cout<<"Transponovana ne kvadratna matrica : "<<std::endl;
    mat4.Transpose();
    mat4.Print();
    std::cout<<"Ponovo transponovana matrica bi trebala vratitti pocetnu matricu: "<<std::endl;
    Transpose(mat3);
    PrintMatrix(mat3);
    std::cout<<std::endl<<"......End of test file for Matrix....."<<std::endl;
    std::cout<<std::endl<<"Exiting...";
}catch(std::range_error izu){
    std::cout<<izu.what();
}catch(std::logic_error izu){
    std::cout<<izu.what();
}
	return 0;
}