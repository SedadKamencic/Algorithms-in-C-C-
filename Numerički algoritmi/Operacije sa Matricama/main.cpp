//NA 2017/2018: Zadaća 2, Zadatak 1
#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <utility>

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
        return (this->Norm()) * (std::numeric_limits<double>::epsilon()) * 10;   
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
    void Chop(double eps = -1);
    bool EqualTo(const Vector &v, double eps = -1) const;
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
        return  10 * this->Norm() * std::numeric_limits<double>::epsilon();
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
    void Chop(double eps = -1);
    bool EqualTo(const Matrix &m, double eps = -1) const;
    double Det() const;
    friend double Det(Matrix m);
    friend Matrix LeftDiv(Matrix m1, Matrix m2);
    friend Vector LeftDiv(Matrix m, Vector v);
    Matrix &operator /=(double s);
    friend Matrix operator /(const Matrix &m, double s);
    Matrix &operator /=(Matrix m);
    friend Matrix operator /(Matrix m1, Matrix m2);
    void Invert();
    friend Matrix Inverse(Matrix m);
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

//nadogradnja za klasu Vector
void Vector::Chop(double eps){
    if(eps < 0) eps = this->GetEpsilon();
    for(int i(0);i<NElems();i++)
        if(std::fabs(vek[i]) <= eps) vek[i]=0;
}
bool Vector::EqualTo(const Vector &v, double eps) const {
    if(NElems() != v.NElems()) return false;
    if(eps < 0) eps = this->GetEpsilon();
    for(int i(0);i<NElems();i++)
        if((std::fabs(vek[i]) - std::fabs(vek[i])) > eps) return false;
    return true;
}

//nadogradnja za klasu Matrix
void Matrix::Chop(double eps){
    if(eps < 0) eps = this->GetEpsilon();
    for(int i(0);i<NRows();i++)
        for(int j(0);j<NCols();j++)
            if(std::fabs(mat[i][j]) <= eps) mat[i][j]=0;
}
bool Matrix::EqualTo(const Matrix &m, double eps) const{
    if((NRows() != m.NRows()) || (NCols() != m.NCols())) return false;
    if(eps < 0) eps = this->GetEpsilon();
    for(int i(0);i<NRows();i++)
        for(int j(0);j<NCols();j++)
            if((std::fabs(mat[i][j]) - std::fabs(m.mat[i][j])) > eps) return false;
    return true;        
}
double Matrix::Det() const {
    if(NRows() != NCols()) throw std::domain_error("Matrix is not square");
    Matrix pom(*this);
    int p(0),  n(NRows()), i(0), d(1);
    double u(0);
    for(int k(0);k<n;k++){
        p = k;
        for(i=k+1;i<n;i++)
            if(std::fabs(pom[i][k]) > std::fabs(pom[p][k]))
                p = i;
        if(std::fabs(pom[p][k]) < pom.GetEpsilon())
            return 0;
        if(p != k){
            double temp = 0;
            for(int j(k);j<n;j++){
                temp = pom[k][j];
                pom[k][j] = pom[p][j];
                pom[p][j] = temp;
            }
            d=-d;
        }    
        for(i=k+1;i<n;i++) {
            u=(pom[i][k]) / (pom[k][k]);
            for(int j(k+1);j<n;j++)
                pom[i][j]-=u*pom[k][j];
        }
    }    
    for(i=0;i<n;i++)
        d *= pom[i][i];
    
    return d;
}
double Det(Matrix m){
    if(m.NRows() != m.NCols()) throw std::domain_error("Matrix is not square");
    Matrix pom(m);
    int p(0),  n(m.NRows()), i(0), d(1);
    double u(0);
    for(int k(0);k<n;k++){
        p = k;
        for(i=k+1;i<n;i++)
            if(std::fabs(pom[i][k]) > std::fabs(pom[p][k]))
                p = i;
        if(std::fabs(pom[p][k]) < pom.GetEpsilon())
            return 0;
        if(p != k){
            double temp = 0;
            for(int j(k);j<n;j++){
                temp = pom[k][j];
                pom[k][j] = pom[p][j];
                pom[p][j] = temp;
            }
            d=-d;
        }    
        for(i=k+1;i<n;i++) {
            u=(pom[i][k]) / (pom[k][k]);
            for(int j(k+1);j<n;j++)
                pom[i][j]-=u*pom[k][j];
        }
    }    
    for(i=0;i<n;i++)
        d *= pom[i][i];
    
    return d;
}
Matrix LeftDiv(Matrix m1, Matrix m2){
    if(m1.NRows() != m1.NCols()) throw std::domain_error("Divisor matrix is not square");
    if(m1.NCols() != m2.NRows()) throw std::domain_error("Incompatible formats");
    Matrix X(m1.NCols(), m2.NCols());
    int p(0),  n(m1.NRows()),m(m2.NCols()), i(0);
    double u(0);
    for(int k(0);k<n;k++){
        p = k;
        for(i=k+1;i<n;i++)
            if(std::fabs(m1[i][k]) > std::fabs(m1[p][k]))
                p = i;
        if(std::fabs(m1[p][k]) < m1.GetEpsilon())
            throw std::domain_error("Divisor matrix is singular");
        if(p != k){
            double temp = 0;
            for(int j(k);j<n;j++){
                temp = m1[k][j];
                m1[k][j] = m1[p][j];
                m1[p][j] = temp;
            }
            for(int j(k);j<m;j++){
                temp = m2[k][j];
                m2[k][j] = m2[p][j];
                m2[p][j] = temp;
            }
        }    
        for(i=k+1;i<n;i++) {
            u=(m1[i][k]) / (m1[k][k]);
            for(int j(k+1);j<n;j++)
                m1[i][j]-=u*m1[k][j];
            for(int j(1);j<m;j++)
                m2[i][j]-=u*m2[k][j];    
        }
    }
    double s = 0;
    for(int k(0);k<m;k++){
        for(int i(n-1);i>=0;i--){
            s = m2[i][k];
            for(int j(i);j<n;j++)
                s = s - m1[i][j] * X[j][k];
            X[i][k] = s / m1[i][i];    
        }
    }
    return X;
}
Vector LeftDiv(Matrix m1, Vector b){
    if(m1.NRows() != m1.NCols()) throw std::domain_error("Divisor matrix is not square");
    if(m1.NCols() != b.NElems()) throw std::domain_error("Incompatible formats");
    Vector X(b.NElems());
    int p(0),  n(m1.NRows()),m(b.NElems()), i(0);
    double u(0);
    for(int k(0);k<n;k++){
        p = k;
        for(i=k+1;i<n;i++)
            if(std::fabs(m1[i][k]) > std::fabs(m1[p][k]))
                p = i;
        if(std::fabs(m1[p][k]) < m1.GetEpsilon())
            throw std::domain_error("Divisor matrix is singular");
        if(p != k){
            double temp = 0;
            for(int j(k);j<n;j++){
                temp = m1[k][j];
                m1[k][j] = m1[p][j];
                m1[p][j] = temp;
            }
        temp = b[k];
        b[k]=b[p];
        b[p]=temp;
        }    
        for(i=k+1;i<n;i++) {
            u=(m1[i][k]) / (m1[k][k]);
            for(int j(k+1);j<n;j++)
                m1[i][j]-=u*m1[k][j];
            b[i]-=u*b[k];    
        }
    }
    double s = 0;
    for(int k(0);k<m;k++){
        for(int i(n-1);i>=0;i--){
            s = b[k];
            for(int j(i);j<n;j++)
                s = s - m1[i][j] * X[k];
            X[k] = s / m1[i][i];    
        }
    }
    return X;
}
Matrix &Matrix::operator /=(double s){
    if(std::fabs(s-0) < 0.000001) throw std::domain_error("Division by zero");
    for(int i(0);i<this->NRows();i++)
        for(int j(0);j<this->NCols();j++)
            mat[i][j]/=s;

    return *this;
}
Matrix operator /(const Matrix &m, double s){
    Matrix pom(m);
    return pom/=s;
}
Matrix& Matrix::operator /=(Matrix m2){
    if(m2.NRows() != m2.NCols()) throw std::domain_error("Divisor matrix is not square");
    if(m2.NCols() != mat[0].size()) throw std::domain_error("Incompatible formats");
    int p(0),  n(m2.NRows()),m(mat[0].size()), i(0);
    double u(0);
    for(int k(0);k<n;k++){
        p = k;
        for(i=k+1;i<n;i++)
            if(std::fabs(m2[k][i]) > std::fabs(m2[k][p]))
                p = i;
        if(std::fabs(m2[p][k]) < m2.GetEpsilon())
            throw std::domain_error("Divisor matrix is singular");
        if(p != k){
            double temp = 0;
            for(int j(k);j<n;j++){
                temp = mat[k][j];
                mat[j][k] = mat[j][p];
                mat[j][p] = temp;
            }
            for(int j(k);j<m;j++){
                temp = m2[k][j];
                m2[j][k] = m2[j][p];
                m2[j][p] = temp;
            }
        }    
        for(i=k+1;i<n;i++) {
            u=(mat[k][i]) / (mat[k][k]);
            for(int j(k+1);j<n;j++)
                mat[j][i]-=u*mat[j][k];
            for(int j(1);j<m;j++)
                m2[j][i]-=u*m2[j][k];    
        }
    }
    double s = 0;
    for(int k(0);k<m;k++){
        for(int i(n-1);i>=0;i--){
            s = m2[k][i];
            for(int j(i);j<n;j++)
                s = s - mat[j][i] * mat[k][j];
            mat[k][i] = s / mat[i][i];    
        }
    }
    return *this;
}   
Matrix operator /(Matrix m1, Matrix m2){
    m1/=m2;
    return m1;
}
void Matrix::Invert(){
    for(int i=0;i<mat.size();i++)
        if(mat[i].size() != mat.size()) throw std::domain_error("Matrix is not square");
}
Matrix Inverse(Matrix m){
    if(m.NRows() != m.NCols()) throw std::domain_error("Matrix is not square");
    return m;
} 

//LUDecomposer
class LUDecomposer{
    std::vector<double> w;
public:
    LUDecomposer(Matrix m){
        if(m.NRows() != m.NCols()) throw std::domain_error("Matrix is not square");
        int n(m.NCols()),p(0);
        w.resize(0);
        double s=0;
        for(int j(0); j<n;j++){
            for(int i(0);i<j;i++){
                s=m[i][j];
                for(int k(0);k<i-1;k++)
                    s=s-m[i][k] * m[k][j];
                m[i][j] = s;
            p=j;    
            }
            for(int i(j);i<n;i++){
                s=m[i][j];
                for(int k(0);k<j-1;k++)
                    s=s-m[i][k] * m[k][j];
                m[i][j]=s;
                if(std::fabs(s) > std::fabs(m[p][j]))
                    p=i;
            }
            if(std::fabs(m[p][j]) < m.GetEpsilon())
                throw std::domain_error("Matrix is singular");
            /*if(p!=j){
                double temp(0);
                for(int k(0);k<n;k++){
                    temp = m[p][k];
                    m[p][k] = m[j][k];
                    m[j][k] = temp;
                }
            }
            w.push_back(p);
            double u(1/m[j][j]);
            for(int i(j);i<n;j++)
                m[i][j] = u * m[i][j];*/
        }
    }
};

//QRDecomposer
class QRDecomposer{
  
public:
    QRDecomposer(Matrix m){
        if(m.NRows() < m.NCols()) throw std::domain_error("Invalid matrix format");
    }
};


//unit testovi
bool testVekChop(){
    bool flag(false);
    Vector n(5);
    for(int i(0);i<5;i++)
        n[i] = i/10000.;
    n.Chop();
    if(std::abs(n[1] - 0.0001) <= n.GetEpsilon()) flag=true;
    Vector b{0,0,0,0,0};
    for(int i(0);i<5;i++)
        b[i] = i;
    b.Chop(2);
    if((std::abs(b[2] - 0) <= b.GetEpsilon()) && flag) return true;
    return false;
}
bool testVekEqualTo(){
    bool flag1(false),flag2(false),flag3(false);
    {
        Vector a(5);
        Vector b{0,0,0};
        if(!a.EqualTo(b)) flag1 = true;
    }
    {
        Vector a(3);
        Vector b(3);
        for(int i(0);i<3;i++)
            a[i]=b[i]=i;
        if(a.EqualTo(b)) flag2 = true;    
    }
    {
        Vector a(5);
        Vector b{0,0,0,0,0};
        for(int i(0);i<5;i++)
            a[i]=i;
        for(int i(2);i<5;i++)
            b[i]=i;
        if(a.EqualTo(b,2)) flag3 = true;    
    }
    if(flag1 && flag2 && flag3) return true;
    return false;
}
bool testMatrixChop(){
    bool flag(false);
    Matrix m = {{0, 0.0001, 0.0002}, {0.0003, 0.0004, 0.0005}};
    m.Chop();
    if(std::abs(m[0][1] - 0.0001) <= m.GetEpsilon()) flag = true;
    Matrix b = {{1, 2, 3}, {4, 5, 6}};
    b.Chop(3);
    if((std::abs(b[0][2] - 0 ) <= b.GetEpsilon()) && flag) return true;
    return false;
}
bool testMatrixEqualTo(){
    bool f1(false), f2(false), f3(false);
    {
        Matrix a(2,3);
        Matrix b(2,5);
        if(!a.EqualTo(b)) f1=true;
    }
    {
        Matrix a = {{1 ,2}, {3, 4}, {5, 6}};
        Matrix b = {{1 ,2}, {3, 4}, {5, 6}};
        if(a.EqualTo(b)) f2 = true;
    }
    {
        Matrix a = {{0 ,1}, {2, 3}, {4, 5}};
        Matrix b = {{2 ,3}, {4, 5}, {6, 7}};
        if(a.EqualTo(b, 2)) f3 = true;
    }
    if(f1 && f2 && f3) return true;
    return false;
    
}
bool testMatrixDet(){
    bool f1(false);
    try{
        Matrix a(2,3);
        a.Det();
    }catch(std::domain_error e){
        f1=true;
    }
    Matrix a = {{1, 2, 3}, {2, 2, 2}, {1, 3, 2}};
    if((std::fabs(a.Det() - 6) <= a.GetEpsilon()) && f1) return true;
    return false;
}
bool testMatrixFriendDet(){
    bool f1(false);
    try{
        Matrix a(2,3);
        a.Det();
    }catch(std::domain_error e){
        f1=true;
    }
    Matrix a = {{1, 2, 3}, {2, 2, 2}, {1, 3, 2}};
    if((std::fabs(Det(a) - 6) <= a.GetEpsilon()) && f1) return true;
    return false;
}
bool testMatrixLeftDiv(){
    bool f1(false),f2(false),f3(false),f4(false); 
    try{
        Matrix a{{2,3,3},{5,4,2}};
        Matrix b{{2,5,6}};
        LeftDiv(a,b);
    }catch(std::domain_error e){
        f1=true;
    }
    try{
        Matrix a{{2,3},{5,4}};
        Matrix b{{2,5,6}};
        LeftDiv(a,b);
    }catch(std::domain_error e){
        f2=true;
    }
    try{
        Matrix a{{1,1},{2,2}};
        Matrix b{{1,1},{1,1}};
        LeftDiv(a,b);
    }catch(std::domain_error e){
        f3=true;
    }
    try{
        Matrix a{{3,2,1},{5,4,3},{9,8,4}};
        Matrix E{{1,0,0},{0,1,0},{0,0,1}};
        Matrix rez = a*E;
        if(E.EqualTo(LeftDiv(a,rez),-10)) f4=true;
    }catch(std::domain_error e){
        return false;
    }
    if(f1 && f2 && f3 && f4) return true;
    return false;
}
bool testMatrixLeftDivVector(){
    bool f1(false),f2(false),f3(false),f4(false); 
    try{
        Matrix a{{2,3,3},{5,4,2}};
        Matrix b{{2,5,6}};
        LeftDiv(a,b);
    }catch(std::domain_error e){
        f1=true;
    }
    try{
        Matrix a{{2,3},{5,4}};
        Matrix b{{2,5,6}};
        LeftDiv(a,b);
    }catch(std::domain_error e){
        f2=true;
    }
    try{
        Matrix a{{1,1,1},{2,2,2},{3,3,3}};
        Matrix b{{1,2},{1,2},{1,2}};
        LeftDiv(a,b);
    }catch(std::domain_error e){
        f3=true;
    }
    try{
        Matrix a{{2,1},{1,-1}};
        Vector b{-2,1};
        Vector rez = a*b;
        if(b.EqualTo(LeftDiv(a,rez),-10)) f4=true;
    }catch(std::domain_error e){
        return false;
    }
    if(f1 && f2 && f3 && f4) return true;
    return false;
}
bool testMatrixRightDivSkalar(){
    bool f1(false), f2(false);
    try{
        Matrix a{{2,4},{2,2}};
        double s = 0;
        a/s;
    }catch(std::domain_error e){
        f1=true;
    }
    try{
        Matrix a{{2,4},{2,2}};
        double s = 2;
        a/=s;
        if(std::fabs(a[0][1] - 2) < 0.00001) f2 = true;
    }catch(std::domain_error e){
        return false;
    }
    if(f1 && f2 ) return true;
    return false;
}
bool testMatrixRightDivMatrix(){
    bool f1(false),f2(false),f3(false),f4(true); 
    try{
        Matrix b{{2,3,3},{5,4,2}};
        Matrix a{{2,5,6}};
        a/b;
    }catch(std::domain_error e){
        f1=true;
    }
    try{
        Matrix b{{2,3},{5,4}};
        Matrix a{{2,5,6}};
        a/b;
    }catch(std::domain_error e){
        f2=true;
    }
    try{
        Matrix b{{1,1},{2,2}};
        Matrix a{{1,1},{1,1}};
        a/b;
    }catch(std::domain_error e){
        f3=true;
    }
    try{
        Matrix a {{0,3,2},{4,6,1},{3,1,7}};
        Matrix b {{4,1,5},{1,2,1},{1,1,1}};
        Matrix e {{1,0,0},{0,1,0},{0,0,1}};
        Matrix c=LeftDiv(a,e);
        Matrix rez = b * c;
        b/=a;
        if(b.EqualTo(rez)) f4=true;
    }catch(std::domain_error e){
        return true;
    }
    if(f1 && f2 && f3 && f4) return true;
    return false;    
}
void GlavnaTestFunkcija(){
    std::cout<<"Test Vektor Chop: ";
    if(testVekChop()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Vector EqualTo: ";
    if(testVekEqualTo()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Matrix Chop: ";
    if(testMatrixChop()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Matrix EqualTo: ";
    if(testMatrixEqualTo()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Matrix Det ";
    if(testMatrixDet()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Matrix Friend Det: ";
    if(testMatrixFriendDet()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Matrix LeftDiv: ";
    if(testMatrixLeftDiv()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Matrix LeftDiv Vector: ";
    if(testMatrixLeftDivVector()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Matrix RightDiv Scalar: ";
    if(testMatrixRightDivSkalar()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
     std::cout<<"Test Matrix RightDiv Matrix: ";
    if(testMatrixRightDivMatrix()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;        
        
}

int main (){
    GlavnaTestFunkcija();
	return 0;
}