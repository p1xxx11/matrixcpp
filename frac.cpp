#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

std::vector<unsigned int> razl(int x) {
    x = abs(x);
    std::vector<unsigned int> result;
    if (x < 2) return result;

    for (int p = 2; p <= x / p; p += (p == 2 ? 1 : 2)) {
        while (x % p == 0) {
            result.push_back(static_cast<unsigned int>(p));
            x /= p;
        }
    }

    if (x > 1) result.push_back(static_cast<unsigned int>(x));
    std::sort(result.begin(), result.end());
    return result;
}

class Frac{
    private:
    int a_;
    unsigned int b_;
    public:
    Frac() : a_(0), b_(1) {}
    Frac(int a, unsigned int b) : a_(a), b_(b) {}
    template <typename T, typename K>
    Frac(T a, K b);
    Frac(const Frac& fr) : a_(fr.a()), b_(fr.b()) {}

    int a() const {return a_;}
    unsigned int b() const {return b_;}
    int& a(int x) {return a_=x;}
    unsigned int& b(unsigned x) {return b_=x;} 
    Frac& operator+=(const Frac& fr);
    Frac& operator-=(const Frac& fr);
    Frac& operator*=(const Frac& fr);
    // Frac& operator*=(const int x)
    Frac& operator/=(const Frac& fr);
    int sign() const;

    Frac& canoniz();
    Frac& cast();
};

template <typename T, typename K>
Frac::Frac(T a, K b){
    if (b < 0){
        a = -a; b = -b;
    }
    float a_ost = a - round(a);
    float b_ost = b - round(b);
    while (a_ost != 0 || b_ost != 0){
        a*=10; b*=10;
        a_ost = a - round(a);
        b_ost = b - round(b);
    }
    a_ = a;
    b_ = b;
}

std::ostream& operator<<(std::ostream& os, const Frac& a) {
    os << a.a() << "/" << a.b() << "\n";
    return os;
}

int Frac::sign() const{
    if (this->a() >= 0) return 1;
    return -1;
}


Frac& Frac::canoniz(){
    std::vector<unsigned int> ar = razl(this->a());
    std::vector<unsigned int> br = razl(this->b());

    std::vector<unsigned int> dl = {};

    std::set_intersection(ar.begin(), ar.end(),
    br.begin(), br.end(),
    std::back_inserter(dl)
    );

    if (!(dl.empty())){
        for (int i : dl){
            this->a((this->a())/i);
            this->b((this->b())/i);
            // std::cout << i << "\n" << *this;
        }
    }
    return *this;
}


Frac& Frac::operator*=(const Frac& fr){
    a_ *= fr.a();
    b_ *= fr.b();
    this->canoniz();
    return *this;
}

Frac operator*(const Frac& a, const Frac& b){
    Frac f;
    f.a(a.a()*b.a());
    f.b(a.b()*b.b());
    f.canoniz();
    return f;
}

Frac& Frac::operator/=(const Frac& fr){
    a_ *= fr.b()*fr.sign();
    b_ *= abs(fr.a());
    this->canoniz();
    return *this;
}

Frac operator/(const Frac& a, const Frac& b){
    Frac f;
    f.a(a.a()*b.b()*b.sign());
    f.b(a.b()*abs(b.a()));
    f.canoniz();
    return f;
}

Frac& Frac::operator+=(const Frac& fr){
    a_ = a_*fr.b() + b_*fr.a();
    b_ = b_*fr.b();
    this->canoniz();
    return *this;
}

Frac operator+(const Frac&a, const Frac&b){
    Frac f;
    f.a(a.a()*b.b() +a.b()*b.a());
    f.b(a.b()*b.b());
    f.canoniz();
    return f;
}

Frac& Frac::operator-=(const Frac& fr){
    (*this)*=Frac(-1, 1);
    (*this)+=fr;
    (*this)*=Frac(-1, 1);
    return *this;
}

Frac operator-(const Frac& a, const Frac& b){
    Frac f;
    f = a + (b*Frac(-1, 1));
    return f;
}



int main(){
    Frac fr(2, 5);
    Frac fr2(4, 3);
    // fr.a(1);
    std::cout << (fr/fr2);

}