#include <codecvt>
#include <stdexcept>
#include <string>
#include "BigNumbers.h"

using namespace std;
//метод пробных делителей
//факторизация - разложение натурального числа на простые сомножители
//методы факторизации два вида
//1) общего назначения - для факторизации любого числа
//2) специального назначения - для факторизации числа спец вида
//(числа, имеющие маленькие делители, большие делители, свободные от квадратов)

BN Uint64MakeBN(QBASE val)//метод для создания BN из беззнакового числа
{
    BN res;
    res.decIn(to_string(val));

    return res;
}


vector<BN> trial_divisor_method(const BN& num)
{
    //вспомогательные числа
    BN zero = Uint64MakeBN(0);
    BN one = Uint64MakeBN(1);
    BN two = Uint64MakeBN(2);
    BN six = Uint64MakeBN(6);

    //копия числа
    BN num_copy = num;
    num_copy.signSet(true);//меняем знак у копии числа

    if(num_copy <= one)
    {
        throw invalid_argument("trial_divisor_method - число должно быть не равно 1 или 0");
    }

    BN rounds = Uint64MakeBN(10);
    bool check_prime = num_copy.MilRabTest(rounds);

    //результат
    vector<BN> res;

    if(check_prime)
    {
        res.push_back(num);
        return res;
    }
    
    //этим избавимся от всех двоек
    while(num_copy % two == zero)
    {
        res.push_back(two);
        num_copy = num_copy / two;
    }

    if (num_copy == one)
    {
        return res;
    }
    
    //по формуле из этого получим все простые числа
    vector<BN> d;
    d.push_back(Uint64MakeBN(3));
    d.push_back(Uint64MakeBN(5));
    d.push_back(Uint64MakeBN(7));

    //индекс текущего пробного делителя
    int k = 0;//номер пробного делителя
    int t = 0;//номер делителя - по сути нам не нужна
    while (num_copy > one)
    {
        BN q = num_copy / d[k];//частное 
        BN r = num_copy % d[k];//остаток
        if(r == zero)//случай когда остаток равен нулю кладем делитель
        {
            t++;
            res.push_back(d[k]);
            num_copy = q;
            continue;
        }
        if(q > d[k])//когда частное больше делителя надо увеличеть делитель по формуле
        {
            k++;
            if(k >= int(d.size()))
            {
                BN next_k = d[k-2] + six;//d[i] = d[i-2] + 6 по формуле
                d.push_back(next_k);
            }
        }
        else
        {
            //последний случай num_copy будет простым
            res.push_back(num_copy);
            return res;
        }

    }

    return res;
}

void printFactorz(const vector<BN>& factorz)
{
    vector<BN> uniqFactorz;
    vector<int> counts;

    for(int i = 0; i < factorz.size(); i++)
    {
        bool found = false;
        for(int j = 0; j < uniqFactorz.size(); j++)
        {
            if(uniqFactorz[j] == factorz[i])
            {
                counts[j]++;
                found = true; 
                break;
            }
        }
        if(!found)
        {
            uniqFactorz.push_back(factorz[i]);
            counts.push_back(1);
        }
    }
    for(int i = 0; i < uniqFactorz.size(); i++)
    {
        cout << uniqFactorz[i];
        if(counts[i] > 1)
            cout << "^" << counts[i];
        if(i + 1 < uniqFactorz.size()) 
            cout << " * ";
    }
}

void full_trial_divisor_method(vector<BN> test)
{
    BN rounds = Uint64MakeBN(10);


    for(int i = 0; i < test.size(); i++)
    {
        BN n = test[i];

        vector<BN> factorz = trial_divisor_method(n);

        if(factorz.size() == 1)
            cout << "число " << n << " - простое" << endl;
        else
        {
            cout << "разложение " << n << " = ";
            printFactorz(factorz);
            cout << endl;
        }
    }
}





int main()
{
    // 435461 - 13 * 19 * 41 * 43
    // 3483688 - 2^3 * 13 * 19 * 41 * 43
    // 37806808 - 2^3 * 13 * 19^3 * 53
    // //99971 - простое
    // 1063073438058073088 - 2^10 * 941 * 6217 * 7919 * 22409
    vector<BN> numbers;

    numbers.push_back(Uint64MakeBN(435461));
    numbers.push_back(Uint64MakeBN(3483688));
    numbers.push_back(Uint64MakeBN(37806808));
    numbers.push_back(Uint64MakeBN(99971));
    numbers.push_back(Uint64MakeBN(1063073438058073088));

    BN num_test;
    num_test.decIn("302302022023193913");
    numbers.push_back(num_test);

    num_test.decIn("3471234723723487");
    numbers.push_back(num_test);

    full_trial_divisor_method(numbers);
    
    return 0;
}
