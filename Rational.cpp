// IF YOU USE INT128_t, DO BUNDLE int128_t operator overload here.
// INF, NaN, -0 を認めない; 0 は 0/1 とみなす
// numerator に符号を持たせる
template<typename T>
class rational {
    T num, den;

    static constexpr T gcd(T a, T b) {
        T w = b;
        while (b != 0) {
            w = b;
            b = a % b;
            a = w;
        }
        return a;
    }
    void reduce() {
        T g = gcd(std::abs(num), den);
        num /= g, den /= g;
    }

   public:
    rational() : rational(0) {}

    // num / den(=1)
    rational(T _num, T _den = 1) : num(_num), den(_den) {
        assert(den != 0);
        if (den < 0) den = -den, num = -num;
        reduce();
    }

    rational operator+() const {return *this;}
    rational operator-() const {return rational(-num, den);}
    rational inv() const noexcept { return rational(den, num); }
    rational& operator+=(const rational& rhs) {
        T nnum = num * rhs.den + rhs.num * den;
        T nden = den * rhs.den;
        num = nnum, den = nden;
        reduce();
        return *this;
    }
    rational& operator-=(const rational& rhs) {
        return (*this += (-rhs));
    }
    rational& operator*=(const rational& rhs) {
        num *= rhs.num;
        den *= rhs.den;
        reduce();
        return *this;
    }
    rational& operator/=(const rational& rhs) {
        return (*this *= rhs.inv());
    }

    friend rational operator+(const rational& lhs, const rational& rhs) {
        return rational(lhs) += rhs;
    }
    friend rational operator-(const rational& lhs, const rational& rhs) {
        return rational(lhs) -= rhs;
    }
    friend rational operator*(const rational& lhs, const rational& rhs) {
        return rational(lhs) *= rhs;
    }
    friend rational operator/(const rational& lhs, const rational& rhs) {
        return rational(lhs) /= rhs;
    }
    friend bool operator<(const rational& lhs, const rational& rhs) {
        return lhs.num * rhs.den < rhs.num * lhs.den;
    }
    friend bool operator>(const rational& lhs, const rational& rhs) {
        return rhs < lhs;
    }
    friend bool operator<=(const rational& lhs, const rational& rhs) {
        return !(lhs > rhs);
    }
    friend bool operator>=(const rational& lhs, const rational& rhs) {
        return !(lhs < rhs);
    }
    friend bool operator==(const rational& lhs, const rational& rhs) {
        return lhs.num == rhs.num && lhs.den == rhs.den;
    }
    friend bool operator!=(const rational& lhs, const rational& rhs) {
        return !(lhs == rhs);
    }

    T denom() const noexcept { return den; }
    T numer() const noexcept { return num; }

    double to_double() const noexcept {
        return double(num) / double(den);
    }
    long double to_ldouble() const noexcept {
        return (long double)(num) / (long double)(den);
    }

    friend std::ostream& operator<<(std::ostream& os, const rational r) {
        return os << r.num << '/' << r.den;
    }
};
// using fraction = rational<long long>;
// using fraction = rational<int128_t>; // DO UNBUNDLE int128_t operator overload.