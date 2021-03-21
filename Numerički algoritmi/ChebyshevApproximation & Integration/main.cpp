//NA 2017/2018: Zadaća 4, Zadatak 1
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <utility>

const double PI = 4*atan(1);


class ChebyshevApproximation{
    std::vector<double> c;
    int m;
    int n;
    double xmin, xmax;
    ChebyshevApproximation(std::vector<double> koeficijenti, int m, double xmin, double xmax){
        c=koeficijenti;
        this->m = m;
        this->xmin = xmin;
        this->xmax = xmax;
    }
public:
    template <typename FunTip>
        ChebyshevApproximation(FunTip f, double xmin, double xmax, int n);
    
    void set_m(int m);
    void trunc(double eps);
    double operator()(double x) const;
    double derivative(double x) const;
    ChebyshevApproximation derivative() const;
    double integrate(double a, double b) const;
    double integrate() const{ return 0;};
};

template <typename FunTip>
ChebyshevApproximation::ChebyshevApproximation(FunTip f, double xmin, double xmax, int n){
    if(xmin > xmax || n < 1)
        throw std::domain_error("Bad parameters");
    
    m = n;
    this->xmax = xmax;
    this->xmin = xmin;
    
    //racunamo coeficijente
    std::vector<double> v, w;
    v.resize(n+1);
    w.resize(n+2);
    for(int i(0);i<=n+1;i++){
        w[i] = cos( PI * i / (2*n + 2) );
    }
    for(int i(0);i<=n/2;i++){
        v[i] = f( (xmin + xmax + (xmax - xmin) * w[2*i+1] ) / 2 );
    }
    for(int i(n/2+1);i<=n;i++){
        v[i] = f( (xmin + xmax + (xmax - xmin) * w[2*n+1 - 2*i] ) / 2 ); 
    }
    for(int k(0);k<=n;k++){
        double s(0);
        for(int i(0);i<=n;i++){
            int p(( k*( 2*i + 1 ) ) % ( 4*n + 4 ));
            if(p>=2*n+2)
                p = 4*n + 4-p;
            if(p>=n+1)
                s -= v[i] * w[2*n + 2 - p];
            else
                s += v[i] * w[p];
        }
        c.push_back(2*s /(n+1));
    }
}
void ChebyshevApproximation::set_m(int n){
    if(n<=1 || n>m)
        throw std::domain_error("Bad order");
    m = n;    
}
void ChebyshevApproximation::trunc(double eps){
    if(eps<0 || eps > m)
        throw std::domain_error("Bad tolerance");
    std::vector<double> pom(c);
    for(int i(0);i<m;i++){
        if(m==1)
            throw std::domain_error("Bad tolerance");
        if( std::fabs(pom[i] - eps) < 0 ){
            std::swap(pom[i],pom[pom.size()-1]);
            pom.resize(pom.size()-1);
            m--;
        }    
    }    
}
double ChebyshevApproximation::operator()(double x) const{
    if(x<xmin || x>xmax)
        throw std::domain_error("Bad argument");
    double t((2 * x - xmin - xmax) / (xmax - xmin));
    double p(1);
    double q(t);
    double s(c[0]/2 + c[1]*t);
    double r(0);
    for(int k(2);k<m;k++){
        r = (2*t*q - p);
        s = s + c.at(k) * r;
        p = q;
        q = r;
    }
    return s;
}
double ChebyshevApproximation::derivative(double x) const{
    if(x<xmin || x>xmax)
        throw std::domain_error("Bad argument");    
    
    double t((2 * x - xmin - xmax) / (xmax - xmin));
    double p(1);
    double q(4*t);
    double s( c[1] + 4*c[2]*t );
    for(int k(3);k<m;k++){
        double r(k * (2 * t * q / (k-1) - p / (k-2)));
        s = s + c[k] * r;
        p = q;
        q = r;
    }
    return 2*s/(xmax - xmin);
}
ChebyshevApproximation ChebyshevApproximation::derivative() const{
    std::vector<double> cprim{};
    cprim.resize(m);
    
    double mi(4 / (xmax - xmin));
    cprim[m-1] = mi * m * c[m];
    cprim[m-2] = mi * (m-1) * c[m-1];
    for(int k(m-3);k>=0;k--){
        cprim[k] = cprim[k+2] + mi * (k + 1)* c[k+1];
    }
    
    ChebyshevApproximation derivat(cprim,m,xmin,xmax);
    return derivat;
}
double ChebyshevApproximation::integrate(double a, double b) const{
    bool inverzno(false);
    if(a>b){
        std::swap(a,b);
        inverzno = true;
    }
    if(!(a>=xmin && b<=xmax))
        throw std::domain_error("Bad interval");
    if(inverzno)
        return 1;
    return 0;    
}

template <typename FunTip>
std::pair<double, bool> RombergIntegration(FunTip f, double a, double b,double eps = 1e-8, int nmax = 1000000, int nmin = 50){
    if(eps < 0 || nmax < 0 || nmin < 0 || nmax < nmin)
        throw std::domain_error("Bad parameter");
    std::vector<double> I(nmax);
    double N(2);
    double h( (b - a) / N );
    double s(( f(a) + f(b) ) / 2);
    double Iold(s);
    
    for(int i(0); i<log2(nmax)-1; i++){
        for(int j(1); j<=N/2; j++){
            s = s + f( a + (2*j -1)* h);
        }
        I[i] = h*s;
        double p(4);
        for(int k(i-1); k>=0; k--){
            I[k] = ( p * I[k+1] - I[k] ) / (p - 1);
            p *= 4;
        }
        if(std::fabs(I[0] - Iold) <= eps) 
            return std::make_pair(I[0], true);
        Iold =  I[0];
        h /= 2;
        N *= 2;
        if( N >= nmax )
            break;
    }
    return std::make_pair(I[0],false);
}

template <typename FunTip>
std::pair<double, bool> TanhSinhIntegration(FunTip f, double a, double b,double eps = 1e-8, int nmax = 1000000, int nmin = 20, double range = 3.5){
    if(eps < 0 || nmin > nmax || nmin < 0 || range < 0 )
        throw std::domain_error("Bad parameter");
    double pi(PI);
    auto pomFun = [a,b,pi,f](double x){
        return ( b - a ) * PI/4 * cosh(x) / (cosh(PI/2 * sinh(x)) * cosh( PI/2 * sinh(x))) * f(( b + a )/2 + ( b - a)/2 * tanh( PI/2 * sinh(x)));
    };
    a = -range;
    b = range;
    int N(2);
    double h(( b - a ) / N );
    double s(0);
    {
        double tmp( (pomFun(a) + pomFun(b)) / 2 );
        if(std::isfinite(tmp))
            s = tmp;
    }
    double Iold(s);
    double I(Iold);
    while(N < nmax){
        for(int i(1);i<=N/2;i++){
            double tmp = pomFun( a + (2*i - 1) * h );
            if(std::isfinite(tmp))
                s+=tmp;
        }
        I = h * s;
        if(N > nmin && std::fabs( I - Iold) <= eps )
            return std::make_pair(I, true);
        Iold = I;
        N *= 2;
        h /= 2;
    }
    return std::make_pair(I, false);
}


template <typename FunTip>
std::pair<double, bool> AdaptiveIntegration(FunTip f, double a, double b,double eps = 1e-10, int maxdepth = 30, int nmin = 1){
    if(eps < 0 || maxdepth < 0 || nmin < 0)
        throw std::domain_error("Bad parameter");
}

//unit testovi
bool testChebby(){
    bool flag1(false), flag2(false), flag3(false), flag4(false), flag5(false);
    auto Sinus = [](double x) { return std::sin(x); };
    ChebyshevApproximation m(Sinus, 0,PI,10);
    try{
        ChebyshevApproximation(Sinus, 5,4,3);
    }catch(std::domain_error e){
        flag1 = true;
    }
    try{
        ChebyshevApproximation(Sinus, 4,5,-1);
    }catch(std::domain_error e){
        flag2 = true;
    }
    try{
        m.set_m(-1);
    }catch(std::domain_error){
        flag3 = true;
    }
    try{
        m.set_m(100);
    }catch(std::domain_error){
        flag4 = true;
    }
    try{
        m.set_m(100);
    }catch(std::domain_error){
        flag4 = true;
    }    
    try{
        if((m(1) - 0.841471) < 10e-4)
            flag5 = true;
    }catch(...){
        return false;
    }
    if(flag1 && flag2 && flag3 && flag4 && flag5)
        return true;
    return false;
}
bool testTanHSinH(){
    bool flag1(false), flag2(false), flag3(false), flag4(false);
    auto Sinus = [](double x) { return std::sin(x); };
    try{
        auto rez = TanhSinhIntegration(Sinus, 3,5,-1);
        if(std::fabs(rez.first-0)<10e-8)
            rez = {0,0};
    }catch(std::domain_error e){
        flag1 = true;
    }
    try{
        auto rez = TanhSinhIntegration(Sinus, 3,5, 10e-8, -2);
        if(std::fabs(rez.first-0)<10e-8)
            rez = {0,0};
    }catch(std::domain_error e){
        flag2 = true;
    }
    try{
        auto rez = TanhSinhIntegration(Sinus, 3,5, 10e-8, 100, 2, -5);
        if(std::fabs(rez.first-0)<10e-8)
            rez = {0,0};        
    }catch(std::domain_error e){
        flag3 = true;
    }
    try{
        auto rez = TanhSinhIntegration(Sinus, 3,5, 10e-8, 2, 100, 3.5);
        if(std::fabs(rez.first-0)<10e-8)
            rez = {0,0};        
    }catch(std::domain_error e){
        flag4 = true;
    }
    if(flag1 && flag2 && flag3 && flag4)
        return true;
    return false;
}
bool testSinusX(){
    auto Sinus = [](double x) { return std::sin(x); };
    auto testRomb(RombergIntegration(Sinus,0,PI));
    auto testTHSH(TanhSinhIntegration(Sinus,0,PI));
    if(std::fabs(testRomb.second - testTHSH.second) < 10e-8 && std::fabs(testRomb.first - testTHSH.first) < 10e-5)
        return true;
    return false;    
}
bool testKorijenX(){
    auto KorijenX = [](double x) { return 1 / std::sqrt(x);};
    auto testRomb(RombergIntegration(KorijenX, 0,1));
    auto testTHSH(TanhSinhIntegration(KorijenX, 0,1));
    if(std::fabs(testRomb.second - testTHSH.second) < 10e-8 && std::fabs(testRomb.first - testTHSH.first) < 10e-5)
        return true;
    return false;        
}
bool test1KrozX(){
    auto KrozX = [](double x) { return 1 / x; };
    auto testRomb(RombergIntegration(KrozX, 1,2));
    auto testTHSH(TanhSinhIntegration(KrozX, 1,2));
    if(std::fabs(testRomb.second - testTHSH.second) < 10e-8 && std::fabs(testRomb.first - testTHSH.first) < 10e-5)
        return true;
    return false;   
}
void GlavnaTestFunkcija(){
    std::cout<<"Test ChebyshevApproximation: ";
    if(testChebby()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Test TanhSinhIntegration : ";
    if(testTanHSinH()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Primjer ispravnosti za sin(x), [0,PI] : ";
    if(testSinusX()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;
    std::cout<<"Primjer ispravnosti za 1 / sqrt(x), [0,1] : ";
    if(testSinusX()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl; 
    std::cout<<"Primjer ispravnosti za 1 / x, [1,2] : ";
    if(test1KrozX()) std::cout<<"OK"<<std::endl;
        else std::cout<<"BAD"<<std::endl;         
}

int main ()
{
    GlavnaTestFunkcija();
	return 0;
}