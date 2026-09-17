#ifndef BigNumbers_H
#define BigNumbers_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <set>
#include <limits> // Добавлено для numeric_limits

typedef unsigned short int BASE;
typedef unsigned int DBASE;
typedef unsigned long long int QBASE;
#define BASE_SIZE (sizeof(BASE) * 8)

// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ:
// Мы говорим компилятору "они существуют, но не здесь". 
// Память под них выделится в .cpp файле.
extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_int_distribution<BASE> dis;
extern std::uniform_int_distribution<int> sgn;

using namespace std;

class BN
{
    vector<BASE> digits;
    bool sign; // true = positive, false = negative
public:
    //обычно число из нулей
    BN(int lengths = 1, bool random = false);
    //конструктор копирования
    BN(const BN &);
    //размер вектоора
    int length();
    ~BN();
    //операторы сравнения и операции над числами
    BN & operator = (const BN &);
    bool operator == (const BN &) const;
    bool operator > (const BN &) const;
    bool operator < (const BN &) const;
    bool operator != (const BN &) const;
    bool operator >= (const BN &) const;
    bool operator <= (const BN &) const;
    BN operator + (const BN &);
    BN & operator += (const BN &);
    BN operator - (const BN &);
    BN & operator -= (const BN &);
    BN operator * (const BASE &);
    BN & operator *= (const BASE &);
    BN operator * (const BN &);
    BN & operator *= (const BN &);
    BN operator / (const BASE &);
    BN & operator /= (const BASE &);
    BN operator % (const BASE &);
    BN & operator %= (const BASE &);
    BN operator / (const BN &);
    BN & operator /= (const BN &);
    BN operator % (const BN &);
    BN & operator %= (const BN &);
    //инкримент декремент 
    BN operator ++(int);
    BN operator --(int);
    //быстрое возведение в квадрат
    BN qsqr();
    //степень
    BN operator ^ (const BN &);
    BN & operator ^= (const BN &);
    //алгоритм Барретта
    BN barmod(const BN &, const BN &, const BN &, const BN &, const BN &);
    //модульное возведение в степень 
    BN powmod(const BN &, const BN &);
    //тесты простоты
    bool fermaTest(const BN &);
    bool SolStrTest(const BN &);
    bool MilRabTest(const BN &);
    bool LucasTest(const BN &, set<BN>);
    BN Jacobi(const BN &);
    //генератор прсотых чисел и проверка с помощью теста миллера рабина
    BN strPrimeGen(const int &, const int &);
    //ввод и вывод шестнадцатиричных или десятичных строк
    void hexIn(const string &);
    void decIn(const string &);
    string hexOut();
    string decOut();
    
    //потоковый ввод и вывод
    friend istream & operator >> (istream &, BN &);
    friend ostream & operator << (ostream &, const BN &);
    
    //заполнение случайными словами
    void randomize(const int &);
    //случайное в диапозоне
    void randomizeRange(BN, BN);

    // по сути побитовые операции << >>
    BN move(int);
    //установка знака
    void signSet(const bool &);
    //получение знака
    bool signGet();
    //факторизация возвращает множество простых делителей 
    set<BN> factorize();
};

#endif
