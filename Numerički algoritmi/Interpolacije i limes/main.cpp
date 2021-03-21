//NA 2017/2018: Zadaća 3, Zadatak 1
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <string>

class AbstractInterpolator{
private:
    mutable int i;
protected:
    std::vector<std::pair<double, double>> vek;
    int Locate(double x) const;
public:
    AbstractInterpolator(const std::vector<std::pair<double, double>> &data);
    virtual double operator()(double x) const =0;
};
AbstractInterpolator::AbstractInterpolator(const std::vector<std::pair<double, double>> &data){
    i=0;
    vek.resize(data.size());
    std::copy(data.begin(), data.end(), vek.begin());
    std::sort(vek.begin(), vek.end(), [](const std::pair<double, double> &x, const std::pair<double, double> &y) {return x.first < y.first;});
    for(int i(0);i<vek.size()-1; i++)
        if(std::fabs(vek[i].first - vek[i+1].first) < 0.0000001) throw std::domain_error("Invalid data set");
}
int AbstractInterpolator::Locate(double x) const {
    if(x <= vek[0].first) { i=0; return 0;}
    else if(x > vek[vek.size()-1].first) {i=vek.size()-2; return vek.size();}
    else if(vek[i].first < x && x < vek[i+1].first) { return i+1;}
    else if(i > 0 && vek[i-1].first < x && x < vek[i].first) {i--; return i+1;}
    else if(i < vek.size()-1 && vek[i+1].first < x && x < vek[i+2].first) {i++; return i+1;}
    else {
        std::pair<double, double> t(std::make_pair(x,0));
        auto it(std::lower_bound(vek.begin(), vek.end(), t));
        i = it - vek.begin();
    }
    return i;
}

//Linearna
class LinearInterpolator : public AbstractInterpolator{
private:
    
public:
    LinearInterpolator(const std::vector<std::pair<double, double>> &data) : AbstractInterpolator(data) {}
    double operator()(double x) const override;
};
double LinearInterpolator::operator()(double x) const {
    int i = Locate(x);
    if(i==vek.size()){
        int n(vek.size()-1);
        return vek[n].second + ( (vek[n-1].second - vek[n].second) / (vek[n-1].first - vek[n].first) ) * ( x - vek[n].first );
    }
    else if(i==0){
        return vek[0].second + ( (vek[1].second - vek[0].second) / (vek[1].first - vek[0].first) ) * ( x - vek[0].first );
    }
    else{
        i--;
        return vek[i].second + ( (vek[i+1].second - vek[i].second) / (vek[i+1].first - vek[i].first) ) * ( x - vek[i].first );
    }    
}

//Polinomial
class PolynomialInterpolator : public AbstractInterpolator{
    std::vector<std::vector<double>> vekKoef;  
public:    
    PolynomialInterpolator(const std::vector<std::pair<double, double>> &data);
    double operator()(double x) const override;
    void AddPoint(const std::pair<double, double> &p);
    std::vector<double> GetCoefficients() const;
};
PolynomialInterpolator::PolynomialInterpolator(const std::vector<std::pair<double, double>> &data) : AbstractInterpolator(data) {
    int n(vek.size());            
    vekKoef.resize(data.size());
    for(int i(0);i<vekKoef.size();i++)
        vekKoef[i].resize(data.size());
    for(int i(0);i<vekKoef.size();i++)
        for(int j(0);j<vekKoef.size();j++)
            if(j == 0) 
                vekKoef[i][j] = vek[i].second;
            else
                vekKoef[i][j] = 0;
    for(int j(1);j<n;j++)
        for(int i(n); i>j; i--)
            vekKoef[i-1][j] = ( vekKoef[i-1][j-1] - vekKoef[i-2][j-1] ) / ( vek[i-1].first - vek[i-j-1].first ); 
}
double PolynomialInterpolator::operator()(double x) const {
    double sums(0);
    double p(1);
    int j(0), n(vek.size());
    
    for(int i(n-1);i>=0;i--){
        p=1;
        for(j=0; j<i;j++)
            p = p * (x - vek[j].first);
        p = p * vekKoef[i][j];
        sums = sums + p;
    }
    return sums;
}
void PolynomialInterpolator::AddPoint(const std::pair<double, double> &p) {
    for(int i(0);i<vekKoef.size();i++)
        if(std::fabs(vek[i]. first - p.first) < 10e-8)
            throw std::domain_error("Invalid point");
    
    std::pair<double, double> novi(p);
    vek.push_back(novi);
    vekKoef.resize(vek.size());
    for(int i(0);i<vek.size();i++)
        vekKoef[i].resize(vek.size());
    int j(vek.size()-1);
    vekKoef[j][0] = novi.second;
    for(int i(1);i<vekKoef.size();i++)
        vekKoef[j][i] = ((vekKoef[j][i-1] - vekKoef[j-1][i-1]) / (vek[j].first - vek[j-i].first));
}
std::vector<double> PolynomialInterpolator::GetCoefficients() const {
    return std::vector<double>{};
}

//Dio po dio
class PiecewisePolynomialInterpolator : public AbstractInterpolator{ 
private:
    int red;
public:
    PiecewisePolynomialInterpolator(const std::vector<std::pair<double, double>> &data, int order);
    double operator()(double x) const override;
};
PiecewisePolynomialInterpolator::PiecewisePolynomialInterpolator(const std::vector<std::pair<double, double>> &data, int order) : AbstractInterpolator(data) {
    if(order < 1 || order>data.size())
        throw std::domain_error("Invalid order");
        red = order;
}
double PiecewisePolynomialInterpolator::operator()(double x) const {
    int i(Locate(x));
    int donjaG, gornjaG;
    
    if(i==0) i+=2;
    else if(i==1) i++;
    else if(i == vek.size()) i--;
    if(red % 2 != 0){
        donjaG = i - (red - 1) / 2 - 1;
        gornjaG = i + (red + 1) /2;
    }
    else {
        donjaG = i - red / 2;
        gornjaG = i + red / 2;
    }
    while(gornjaG > vek.size()){
        donjaG --;
        gornjaG--;
    }
    std::vector<std::pair<double, double>> pom;
    for(int i(donjaG); i<gornjaG; i++)
        pom.push_back({vek[i].first , vek[i].second});
    
    PolynomialInterpolator PolInt(pom);
    return PolInt(x);
}

//Splajn
class SplineInterpolator : public AbstractInterpolator{
private:
    std::vector<double> vekKoef;
    std::vector<double> vektorS;
    std::vector<double> vektorQ;
public:    
    SplineInterpolator(const std::vector<std::pair<double, double>> &data);
    double operator()(double x) const override;
};
SplineInterpolator::SplineInterpolator(const std::vector<std::pair<double, double>> &data) : AbstractInterpolator(data){
    int n = vek.size();
    std::vector<double> r(n);
    std::vector<double> alpha(n);
    std::vector<double> s(n);
    std::vector<double> q(n);
    
    r[0]=0; r[n-1]=0;
    for(int i(1); i<n-1;i++){
        alpha[i] = 2 * (vek[i+1].first - vek[i-1].first);
        r[i] = 3 * ( (vek[i+1].second - vek[i].second) / (vek[i+1].first - vek[i].first) - (vek[i].second - vek[i-1].second) / (vek[i].first - vek[i-1].first));
    }
    for(int i(1);i<n-2;i++){
        double mi((vek[i+1].first - vek[i].first) / alpha[i]);
        alpha[i+1] = alpha[i+1] - mi * (vek[i+1].first - vek[i].first);
        r[i+1] = r[i+1] - mi * r[i];
    }
    r[n-2] = r[n-2] / alpha[n-2];
    for(int i(n-3);i>0;i--)
        r[i] = ( r[i] - ( vek[i+1].first - vek[i].first ) * r[i+1] ) / alpha[i];
        
    vekKoef = r;
    
    for(int i(0);i<n-1;i++){
        double delta(vek[i+1].first - vek[i].first);
        s[i] = ( r[i+1] - r[i] ) / ( 3 * delta );
        q[i] = ( vek[i+1].second - vek[i].second ) / delta - delta * (r[i+1] + 2 * r[i]) / 3;
    }
    vektorS = s;
    vektorQ = q;
}
double SplineInterpolator::operator ()(double x) const{
    int i(Locate(x));
    if(i==0) i+=1;
    else if(i == vek.size()) i-=1;
    double t = x - vek[i-1].first;
    return vek[i-1].second + t * (vektorQ[i-1] + t * (vekKoef[i-1] + vektorS[i-1] * t));
}

//Limes
template<typename FunTip>
double Limit(FunTip f, double x0, double h = 0, double eps = 1e-8, double nmax = 20){
    if(std::fabs(h - 0) < 0.00001) h = 0.001 * std::max(1., std::abs(x0));
    if(eps <= 0 || nmax < 3 || nmax > 30) throw std::domain_error("Invalid parameters");
    if(isinf(x0)) h=0;
    
    double y_old = std::numeric_limits<double>::infinity();
    std::vector<double> y{};
    y.resize(nmax);
    for(int i(0);i<nmax;i++){
        y[i] = f(x0 + h);
        double p(2);
        for(int k(i-2);k>=0;k--){
            y[k] = (p * y[k+1] - y[k]) / ( p - 1 );
            p = 2 * p;
        }
        if(std::fabs(y[0] - y_old) < eps) 
            return y[0];
        y_old = y[1];
        h = h / 2;
    }
    throw std::logic_error("Accuracy goal is not achieved");
}

//Baricentricna
class BarycentricInterpolator : public AbstractInterpolator{
    int d;
public:
    BarycentricInterpolator(const std::vector<std::pair<double, double>> &data, int order);
    double operator()(double x) const override;
    std::vector<double> GetWeights() const;
};
BarycentricInterpolator::BarycentricInterpolator(const std::vector<std::pair<double, double>> &data, int order) : AbstractInterpolator(data){
    if(!(order>=0 && order<=vek.size())) throw std::domain_error("Invalid order");
    d=order;
}
std::vector<double> BarycentricInterpolator::GetWeights() const{
    int n(vek.size());
    std::vector<double> rezultat;
    
    for(int i(1);i<=n;i++){
        long double w(0);
        for(int k(std::max(1, i-d)); k<=std::min(i, n-d); k++){
            long double p(1);
            for(int j(k); j<=k+d; j++){
                if(j-1 == i-1) continue;
                p=p / (vek[i-1].first - vek[j-1].first);
            }    
            if(k % 2 == 0)
                p = -p;
            w+=p;   
        }
        rezultat.push_back(w);
    }
    return rezultat;
}
double BarycentricInterpolator::operator()(double x) const{
    double p(0);
    double q(0);
    std::vector<double> w(GetWeights());
    int n(vek.size());
    
    for(int i(0); i<n; i++){
        if(std::fabs(x - vek[i].first) < 10e-6)
            return vek[i].second;
        double u(w[i] / (x - vek[i].first));
        p+=u * vek[i].second;
        q+=u;
    }
    return p/q;
}


//unit testovi
bool testInterpolacija(int odabir){
    std::vector<std::pair<double, double>> vek({{2,5}, {8,4}, {3,3}, {5,2}, {3,6}});
    bool flag1(false), flag2(false);
    //Da li baca izuzetak
    try{
        if(odabir==1)
            LinearInterpolator li(vek);
        else if(odabir == 2)
            PolynomialInterpolator pol(vek);
        else if(odabir == 3)
            PiecewisePolynomialInterpolator dio(vek,2);
        else if(odabir == 4)
            SplineInterpolator splajn(vek);
        else if(odabir == 5)    
            BarycentricInterpolator bar(vek,2);
    }catch(std::domain_error izu){
        flag1 = true;
        vek.resize(vek.size()-1);
    }
    //Da li je ispravno sortiran
    std::vector<double> vek1{};
    {
        if(odabir==1){
            LinearInterpolator li(vek);
            for(int i(0);i<vek.size();i++)
                vek1.push_back(li(i));
        }
        else if(odabir == 2){
            PolynomialInterpolator li(vek);
            for(int i(0);i<vek.size();i++)
                vek1.push_back(li(i));
        }
        else if(odabir == 3){
            PiecewisePolynomialInterpolator li(vek,2);
            for(int i(0);i<vek.size();i++)
                vek1.push_back(li(i));            
        }
        else if(odabir == 4){
            SplineInterpolator li(vek);
            for(int i(0);i<vek.size();i++)
                vek1.push_back(li(i));            
        }
        else if(odabir == 5){
            BarycentricInterpolator li(vek,2);
            for(int i(0);i<vek.size();i++)
                vek1.push_back(li(i));            
        }    
    }
    std::vector<double> vek2{};
    {
        std::sort(vek.begin(), vek.end(), [](std::pair<double, double> a, std::pair<double, double> b) {return a.first < b.first;});
        if(odabir==1){
            LinearInterpolator li(vek);
            for(int i(0);i<vek.size();i++)
                vek2.push_back(li(i));
        }
        else if(odabir == 2){
            PolynomialInterpolator li(vek);
            for(int i(0);i<vek.size();i++)
                vek2.push_back(li(i));
        }
        else if(odabir == 3){
            PiecewisePolynomialInterpolator li(vek,2);
            for(int i(0);i<vek.size();i++)
                vek2.push_back(li(i));            
        }
        else if(odabir == 4){
            SplineInterpolator li(vek);
            for(int i(0);i<vek.size();i++)
                vek2.push_back(li(i));            
        }
        else if(odabir == 5){
            BarycentricInterpolator li(vek,2);
            for(int i(0);i<vek.size();i++)
                vek2.push_back(li(i));            
        }        
    }
    int i(0);
    for(;i<vek.size();i++)
        if(!(std::fabs(vek1[i]-vek2[i]) < 10e-8))
            break;
    if(i==vek.size()) flag2 = true;   
    if(flag1 && flag2) return true;
    return false;
}
bool testIzuzetciReda(int odabir){
    bool flag1(false), flag2(false), flag3(false);
    std::vector<std::pair<double, double>> vek({{2,5}, {8,4}, {3,3}, {5,2}});    
    {
        int r(-2);
        try{
            if(odabir==1)
                PiecewisePolynomialInterpolator li(vek, r);
            else if(odabir==2)
                BarycentricInterpolator li(vek, r);
        }catch(std::domain_error izu){
            flag1 = true;
        }
    }
    {
        int r(2);
        try{
            if(odabir==1){
                PiecewisePolynomialInterpolator li(vek, r);
                flag2 = true;
            }    
            else if(odabir==2){
                BarycentricInterpolator li(vek, r);
                flag2 = true;
            }    
        }catch(std::domain_error izu){
            flag2 = false; 
        }
    }
    {
        int r(10);
        try{
            if(odabir==1)
                PiecewisePolynomialInterpolator li(vek, r);
            else if(odabir==2)
                BarycentricInterpolator li(vek, r);
        }catch(std::domain_error izu){
            flag3 = true; 
        }        
    }
    if(flag1 && flag2 && flag3) return true;
        return false;
}
bool testLimes(int odabir){
    if(odabir == 1){
        if(std::fabs(Limit([](double x) {return std::sin(x)/x;},0) - 1) < 10e-8) return true;
            return false;
    }        
    else if(odabir == 2){
        if(std::fabs(Limit([](double x) {return (std::exp(x) - 1) / x;},0) - 1) < 10e-8) return true;
            return false;
    } 
    else if(odabir == 3){
        if(std::fabs(Limit([](double x) {return std::log(1 + x) / x;}, 0) - 1) < 10e-8) return true;
            return false;
    }
    else if(odabir == 4){
        bool flag1(false), flag2(false);
        try{
            Limit([](double x) {return std::sin(x)/x;},0, 10e-5, -2);
        }catch(std::domain_error){
            flag1 = true;
        }catch(...){
            flag1 = false;
        }
        
        try{
            Limit([](double x) {return std::sin(x)/x;},0, 2,2, 35);
        }catch(std::domain_error){
            flag2 = true;
        }catch(...){
            flag2 = false;
        }
        if(flag1 && flag2) return true;
            return false;
    }
    else if(odabir == 5){
        try{
            double inf = std::numeric_limits<double>::infinity();
            if(std::fabs(Limit([](double x) { return atan(x); }, -inf) + 1.5708) < 10e-5)
                return true;
            return false;    
        }catch(...){
            return false;
        }
    }
    return false;
}
bool testAddPoint(){
    std::vector<std::pair<double, double>> vek({{2,5}, {8,4}, {3,3}, {5,2}});
    try{
        PolynomialInterpolator pol(vek);
        pol.AddPoint(std::make_pair(3.,5.));
    }catch(std::domain_error){
        return true;
    }catch(...){
        return false;
    }
    return false;
}
bool testGetWeights(){
    BarycentricInterpolator bi2({{7,3},{3,2},{5,2},{4,5}}, 0);
    std::vector<double> wi = bi2.GetWeights();
    int k(1);
    for(int i(0);i<wi.size();i++){
        if(!(std::fabs(wi[i] - k) < 10e-8))
            return false;
        k = -k;    
    }
    return true;
}

void GlavnaTestFunkcija(){
    std::cout<<"Test interpolacija..."<<std::endl;
    std::cout<<"Test Linearna interpolacija: ";
    if(testInterpolacija(1)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Polinomijalna interpolacija: ";
    if(testInterpolacija(2)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;    
    std::cout<<"Test Dio po dio interpolacija: ";
    if(testInterpolacija(3)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Splajn interpolacija: ";
    if(testInterpolacija(4)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Baricentricna interpolacija: ";
    if(testInterpolacija(5)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Get Weights: ";
    if(testGetWeights()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;        
    
    //dodatni testovi
    std::cout<<"\nTest izuzetaka..."<<std::endl;
    std::cout<<"Test dio po dio interpolacije na red: ";
    if(testIzuzetciReda(1)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Baricentricne interpolacije na red: ";
    if(testIzuzetciReda(2)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test Polinomial AddPoint na istu tacku: ";
    if(testAddPoint()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;        
        
    //test Limesa
    std::cout<<"\nTest Limesa..."<<std::endl;
    std::cout<<"Test sin(x) / x, kad x->0: ";
    if(testLimes(1)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test (exp(x) - 1) / x, kad x->0: ";
    if(testLimes(2)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test ln(1 + x) / x, kad x->0: ";
    if(testLimes(3)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test izuzetaka na pogresne parametre: ";
    if(testLimes(4)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test beskonacnosti: ";
    if(testLimes(5)) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
        
    std::cout<<"\nEnd of testing..."<<std::endl;    
}

int main(){
    GlavnaTestFunkcija();
    return 0;
}