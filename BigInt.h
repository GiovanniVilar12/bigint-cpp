#ifndef BigInt_h
#define BigInt_h


#include <iostream>
#include <cstdint>
#include <string>

class BigInt{

public:
    BigInt();
    BigInt(long long int);
    BigInt(const BigInt& P);
    BigInt(BigInt&& temp) noexcept;
    BigInt(const std::string& S);

    ~BigInt();

    BigInt& operator = (const BigInt& P);
    BigInt& operator = (BigInt&& Temp) noexcept;

    bool isNeg() const;
    int size() const;
    bool isZero() const;
    int operator[](int i) const;

    // Conversão
    long long int toInt() const;

    friend std::istream& operator>>(std::istream& I, BigInt& P);
    friend std::ostream& operator<<(std::ostream& O, const BigInt& P);

    BigInt operator+() const;
    BigInt operator-() const;
    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);


    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    BigInt operator*(const BigInt&) const;
    BigInt operator/(const BigInt&) const;
    BigInt operator%(const BigInt&) const;
    BigInt operator<<(int) const;
    BigInt operator>>(int) const;


    friend BigInt abs(const BigInt&);
    void division(const BigInt& D, BigInt& Q, BigInt& R) const;





private:
    bool neg;
    int nDig;
    int8_t* d;

    void correct();
    void increment();
    void decrement();





};
 BigInt operator!(const BigInt& B);

#endif
