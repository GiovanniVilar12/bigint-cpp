#include <iostream>
#include <string>
#include <cmath>
#include <limits>

#include "BigInt.h"

using namespace std;


BigInt::BigInt() {

 neg = false;
 nDig = 1;
 d = new int8_t[1];
  d[0] = 0;
}



BigInt::~BigInt() {
  delete[] d;
}

BigInt::BigInt(long long int N) {
    neg = (N < 0);

    if (N == 0) {
        nDig = 1;
        d = new int8_t[1];
        d[0] = 0;
        return;
    }


    double abs_N = std::fabs(static_cast<double>(N));
    nDig = 1 + static_cast<int>(std::log10(abs_N));
    d = new int8_t[nDig];


    long long int temp_N = N;
    if (neg) {

        if (N == std::numeric_limits<long long int>::min()) {
             // Tratamento para evitar -LLONG_MIN
             for (int i = 0; i < nDig; ++i) {
                d[i] = -(temp_N % 10);
                temp_N /= 10;
            }
        } else {
            temp_N = -N;
            for (int i = 0; i < nDig; ++i) {
                d[i] = temp_N % 10;
                temp_N /= 10;
            }
        }
    } else {

        for (int i = 0; i < nDig; ++i) {
            d[i] = temp_N % 10;
            temp_N /= 10;
        }
    }
}


BigInt::BigInt(const BigInt& P) {

neg = P.neg;
nDig = P.nDig;
d = new int8_t[nDig];

  for (int i = 0; i < nDig; ++i)
    d[i] = P.d[i];

}


BigInt::BigInt(BigInt&& Temp) noexcept {
    neg = Temp.neg;
    nDig = Temp.nDig;
    d = Temp.d;

 //  deixa o objeto temp em estado validp
    Temp.neg = false;
    Temp.nDig = 0;
    Temp.d = nullptr;
}

BigInt& BigInt::operator=(const BigInt& P) {

    if (this == &P) {

    return *this;
    }

    delete[] d;

    neg = P.neg;
    nDig = P.nDig;
    d = new int8_t[nDig];

  for (int i = 0; i < nDig; ++i)
    d[i] = P.d[i];


  return *this;
}


BigInt& BigInt::operator=(BigInt&& Temp) noexcept {

    if (this == &Temp) {

    return *this;
  }

    delete[] d;

    neg = Temp.neg;
    nDig = Temp.nDig;
    d = Temp.d;


Temp.neg = false;
Temp.nDig = 0;
Temp.d = nullptr;

return *this;
}


bool BigInt::isNeg() const {
    return neg;
}

int BigInt::size() const {
    return nDig;
}

bool BigInt::isZero() const {
    return (nDig == 1 && d[0] == 0);
}

int BigInt::operator[](int i) const {
    if (i >= 0 && i < nDig) {
    return d[i];
  }
  return 0;
}

// *******************************************************
// TESTE 02
// *******************************************************

void BigInt::correct() {

    if (nDig <= 0 || d == nullptr) {

        neg = false;
        nDig = 1;
        delete[] d;
        d = new int8_t[1];
        d[0] = 0;
        return;
    }

    // elimina zeros mais significativos
    int newSize = nDig;
    while (newSize > 1 && d[newSize - 1] == 0) {
        newSize--;
    }

    if (newSize != nDig) {
        int8_t* temp = new int8_t[newSize];
        for (int i = 0; i < newSize; ++i) {
            temp[i] = d[i];
        }
        delete[] d;
        d = temp;
        nDig = newSize;
    }

    // corrige número de 1 dígito com sinal errado
    if (nDig == 1 && d[0] == 0) {
        neg = false;
    }
}

BigInt::BigInt(const std::string& S) {
    neg = false;
    nDig = 1;
    d = new int8_t[1];
    d[0] = 0;

    int start_index = 0;

    // validação da string
    if (S.empty()) {
        std::cerr << "Erro" << std::endl;
        return;
    }
    if (S[0] == '+' || S[0] == '-') {
        if (S.size() == 1) {
            std::cerr << "Erro" << std::endl;
            return;
        }
        neg = (S[0] == '-');
        start_index = 1;
    }
    for (size_t i = start_index; i < S.size(); ++i) {
        if (!isdigit(S[i])) {
            std::cerr << "Erro" << std::endl;
            return;
        }
    }

    // Aloca a memória e converte os dígitos
    nDig = S.size() - start_index;
    delete[] d;
    d = new int8_t[nDig];

    for (int i = 0; i < nDig; ++i) {
        d[i] = S[S.size() - 1 - i] - '0';
    }


    correct();
}
//****************************************************
//  TESTE 03
// ***************************************************



long long int BigInt::toInt() const {
  long long int val = 0;


  for (int i = nDig - 1; i >= 0; --i) {

    if (val > (std::numeric_limits<long long int>::max() - d[i]) / 10) {
      std::cerr << "Erro" << std::endl;
      return 0;
    }

    val = val * 10 + d[i];
  }


  if (neg) {
    return -val;
  }

  return val;
}

std::ostream& operator<<(std::ostream& os, const BigInt& P) {

  if (P.isNeg()) {
    os << '-';
  }


  for (int i = P.size() - 1; i >= 0; --i) {

    os << P[i];
  }

  return os;
}





std::istream& operator>>(std::istream& I, BigInt& P) {

  P = BigInt();


  std::istream::sentry s(I);
  if (s) {
    char c;


    c = I.peek();


    if (c == '+' || c == '-') {
      I.get();
      P.neg = (c == '-');
      c = I.peek();
    }

    int digit_count = 0;
    while (std::isdigit(c)) {
      I.get(c); // lê o caractere


      int8_t* tempD = new int8_t[P.nDig + 1];

      for (int i = 0; i < P.nDig; ++i) {
        tempD[i + 1] = P.d[i];
      }


      tempD[0] = static_cast<int8_t>(c - '0');

      delete[] P.d;
      P.d = tempD;
      P.nDig++;

      c = I.peek();
      digit_count++;
    }


    if (digit_count == 0) {
      I.setstate(std::ios::failbit);
    }
  }


  P.correct();

  return I;
}

//******************************************************
// TESTE 04
//******************************************************


BigInt abs(const BigInt& X){

    BigInt result = X;
    result.neg = false;

    return result;
}

bool BigInt::operator==(const BigInt& B) const{

    if(neg!= B.neg){
        if(isZero() && B.isZero()){
            return true;
        }
        return false;
    }
    if(nDig != B.nDig){
        return false;
    }
    for(int i=0;i<nDig;i++){
        if(d[i] != B.d[i])
            return false;
    }

    return true;

}

bool BigInt::operator!=(const BigInt& B) const {
    return !(*this == B);
}

bool BigInt::operator<(const BigInt& B) const {

    if(neg != B.neg)
        return neg;

    if(neg){
         return  B < *this;
    }
    else{
        if(nDig != B.nDig){
            return nDig < B.nDig;
        }
        for(int i= nDig - 1; i>=0;--i){
            if(d[i] != B.d[i]){
                return d[i] < B.d[i];
            }
        }
        return false;
    }
}

bool BigInt::operator<=(const BigInt& B) const {
    return (*this < B) || (*this == B);
}

bool BigInt::operator>(const BigInt& B) const {
    return B < *this;
}

bool BigInt::operator>=(const BigInt& B) const {
    return !(*this < B);
}

//**************************************************
// TESTE 05
//**************************************************


void BigInt::increment() {
    int k = 0;
    d[k]++;


    while (k < nDig - 1 && d[k] > 9) {
        d[k] = 0;
        k++;
        d[k]++;
    }


    if (k == nDig - 1 && d[k] > 9) {

        int8_t* temp = new int8_t[nDig + 1];


        temp[nDig] = 1;
        d[k] = d[k] - 10;


        for (int i = 0; i < nDig; ++i) {
            temp[i] = d[i];
        }

        delete[] d;
        d = temp;
        nDig++;
    }
}

void BigInt::decrement() {
    int k = 0;
    d[k]--;

    while (k < nDig - 1 && d[k] < 0) {
        d[k] = 9;
        k++;
        d[k]--;
    }

    if (k == nDig - 1 && d[k] <= 0) {
        correct();
    }
}

BigInt& BigInt::operator++() {
    if (isNeg()) {
        decrement();
    } else {
        increment();
    }
    return *this;
}

BigInt& BigInt::operator--() {
    if (isNeg()) {
        increment();
    } else {
        decrement();
    }
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    ++(*this);
    return temp;
}

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --(*this);
    return temp;
}



//*********************************************
// TESTE 06
//*********************************************


BigInt BigInt::operator+() const{
    return *this;
}


BigInt BigInt::operator-() const{
    BigInt result = *this;
    if(!isZero()){
        result.neg = !result.neg;
    }
    return result;
}


BigInt BigInt::operator+(const BigInt& B) const{

    if(neg == B.neg){
        BigInt result;
        result.neg = neg;


        int max_size = (nDig > B.nDig) ? nDig : B.nDig;
        result.nDig = 1 + max_size;

        delete[] result.d;
        result.d = new int8_t[result.nDig];

        int carry = 0;
        for(int i = 0; i < result.nDig; ++i) {
            int sum = (*this)[i] + B[i] + carry;
            result.d[i] = sum % 10;
            carry = sum / 10;
        }

        result.correct();
        return result;
    }


    else {

        if (abs(*this) >= abs(B)) {
            BigInt result;
            result.neg = neg;
            result.nDig = nDig;
            delete[] result.d;
            result.d = new int8_t[result.nDig];

            int borrow = 0;
            for (int i = 0; i < result.nDig; ++i) {
                int diff = (*this)[i] - B[i] - borrow;
                if (diff < 0) {
                    diff += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                result.d[i] = diff;
            }
            result.correct();
            return result;
        }

        else {
            return B + *this;
        }
    }
}


BigInt BigInt::operator-(const BigInt& B) const{
    return *this + (-B);
}


//*****************************************
// TESTE 07
//*****************************************


BigInt BigInt::operator*(const BigInt& B) const {

    if (isZero() || B.isZero()) {
        return BigInt(0);
    }


    BigInt result;
    result.neg = (neg != B.neg);
    result.nDig = nDig + B.nDig;
    delete[] result.d;
    result.d = new int8_t[result.nDig]{0};

    for (int i = 0; i < nDig; ++i) {
        if (d[i] != 0) {
            for (int j = 0; j < B.nDig; ++j) {
                if (B.d[j] != 0) {
                    int k = i + j;
                    result.d[k] += d[i] * B.d[j];

                    // vai um
                    while (result.d[k] > 9) {
                        result.d[k+1] += result.d[k] / 10;
                        result.d[k] %= 10;
                        k++;
                    }
                }
            }
        }
    }

    result.correct();
    return result;
}


BigInt operator!(const BigInt& B) {

    if (B.isNeg()) {
        std::cerr << "Erro" << std::endl;
        return BigInt(0);
    }


    BigInt result = 1;

    for (BigInt i = 2; i <= B; ++i) {
        result = result * i;
    }

    return result;
}

//*********************************
// TESTE 08
// ********************************

BigInt BigInt::operator<<(int N) const {

    if (N <= 0 || isZero()) {
        return *this;
    }

    BigInt result;
    result.neg = neg;
    result.nDig = nDig + N;
    delete[] result.d;
    result.d = new int8_t[result.nDig]{0};

    // Copia os dígitos originais para as novas posições.
    for (int i = 0; i < nDig; ++i) {
        result.d[i + N] = d[i];
    }

    result.correct();
    return result;
}

BigInt BigInt::operator>>(int N) const {

    if (N <= 0 || isZero()) {
        return *this;
    }

    if (N >= nDig) {
        return BigInt(0);
    }

    BigInt result;
    result.neg = neg;
    result.nDig = nDig - N;
    delete[] result.d;
    result.d = new int8_t[result.nDig];


    for (int i = 0; i < result.nDig; ++i) {
        result.d[i] = d[i + N];
    }

    result.correct();
    return result;
}

void BigInt::division(const BigInt& D, BigInt& Q, BigInt& R) const {

    if (D.isZero()) {
        std::cerr << "Erro" << std::endl;
        Q = BigInt(0);
        R = BigInt(0);
        return;
    }
    if (isZero()) {
        Q = BigInt(0);
        R = BigInt(0);
        return;
    }

    if (abs(*this) < abs(D)) {
        Q = BigInt(0);
        R = *this;
        return;
    }


    Q = BigInt(0);
    R = BigInt(0);


    for (int i = nDig - 1; i >= 0; --i) {

        R = R << 1;
        R.d[0] = d[i];

        int div = 0;
        // faz subtracoes sucessivas para encontrar o proximo digito do quociente.
        while (R >= abs(D)) {
            R = R - abs(D);
            div++;
        }

        // Desloca o quociente para a esquerda e adiciona o novo digito.
        Q = Q << 1;
        Q.d[0] = div;
    }

    // Define os sinais do quociente e do resto.
    Q.neg = (neg != D.neg);
    if (!R.isZero()) {
        R.neg = neg;
    }

    Q.correct();
    R.correct();
}

BigInt BigInt::operator/(const BigInt& B) const {
    BigInt Q, R;
    this->division(B, Q, R);
    return Q;
}

BigInt BigInt::operator%(const BigInt& B) const {
    BigInt Q, R;
    this->division(B, Q, R);
    return R;
}

