#include "BigNumbers.h"

random_device rd;
mt19937 gen(static_cast<unsigned>(
chrono::high_resolution_clock::now().time_since_epoch().count()));
uniform_int_distribution<BASE> dis(0, std::numeric_limits<BASE>::max());
uniform_int_distribution<int> sgn(0, 1);

void BN::signSet (const bool & newSign)
{
    sign = newSign;
}
bool BN::signGet()
{
    return sign;
}

int BN::length()
{
    return digits.size();
}

BN::BN(int lengths, bool random)
{
    if (random)
    {
        digits.resize(lengths, 0);
        if (lengths == 1) digits.at(0) = dis(gen);
        else
        {
            while (digits.at(lengths-1) == 0) digits.at(lengths-1) = dis(gen); 
            for (int i = lengths - 2; i >= 0; i--) digits.at(i) = dis(gen);
        }
        int srand = sgn(gen);
        if (srand) {sign = true;} else {sign = false;}    
    }
    else
    {
        digits.resize(lengths, 0);
        sign = true;
    }
}

BN::~BN() {}

BN::BN(const BN & number)
{
    digits = number.digits;
    sign = number.sign;
}

BN BN::operator ++(int)
{
    BN one;
    one.hexIn("1");
    *this += one;
    return *this;
}

BN BN::operator --(int)
{
    BN one;
    one.hexIn("1");
    *this -= one;
    return *this;
}

BN & BN::operator = (const BN & number)
{
    if (this != &number)
    {
        digits = number.digits;
        sign = number.sign;
    }
    return *this;
}

bool BN::operator == (const BN & number) const
{
    if (digits.size() != number.digits.size()) return false;
    if (sign != number.sign) return false;
    for (int i = digits.size() - 1; i >= 0; i--)
    {
        if (digits.at(i) != number.digits.at(i)) return false;
    }
    return true;
}

bool BN::operator != (const BN & number) const
{
    if (digits.size() != number.digits.size()) return true;
    if (sign != number.sign) return true;
    for (int i = digits.size() - 1; i >= 0; i--)
    {
        if (digits.at(i) != number.digits.at(i)) return true;
    }
    return false;
}

bool BN::operator > (const BN & number) const
{
    if (sign && !(number.sign)) return true;
    if (!(sign) && number.sign) return false;
    if (!(sign) && !(number.sign))
    {
        if (digits.size() < number.digits.size()) return true;
        if (digits.size() > number.digits.size()) return false;
        for (int i = digits.size() - 1; i >= 0; i--)
        {
            if (digits.at(i) > number.digits.at(i)) return false;
            if (digits.at(i) < number.digits.at(i)) return true;
        }
    }
    if (digits.size() < number.digits.size()) return false;
    if (digits.size() > number.digits.size()) return true;
    for (int i = digits.size() - 1; i >= 0; i--)
    {
        if (digits.at(i) > number.digits.at(i)) return true;
        if (digits.at(i) < number.digits.at(i)) return false;
    }
    return false;
}

bool BN::operator < (const BN & number) const
{
    if (sign && !(number.sign)) return false;
    if (!(sign) && number.sign) return true;
    if (!(sign) && !(number.sign))
    {
        if (digits.size() < number.digits.size()) return false;
        if (digits.size() > number.digits.size()) return true;
        for (int i = digits.size() - 1; i >= 0; i--)
        {
            if (digits.at(i) > number.digits.at(i)) return true;
            if (digits.at(i) < number.digits.at(i)) return false;
        }
    }
    if (digits.size() < number.digits.size()) return true;
    if (digits.size() > number.digits.size()) return false;
    for (int i = digits.size() - 1; i >= 0; i--)
    {
        if (digits.at(i) > number.digits.at(i)) return false;
        if (digits.at(i) < number.digits.at(i)) return true;
    }
    return false;
}

bool BN::operator >= (const BN & number) const
{
    if (sign && !(number.sign)) return true;
    if (!(sign) && number.sign) return false;
    if (!(sign) && !(number.sign))
    {
        if (digits.size() < number.digits.size()) return true;
        if (digits.size() > number.digits.size()) return false;
        for (int i = digits.size() - 1; i >= 0; i--)
        {
            if (digits.at(i) > number.digits.at(i)) return false;
            if (digits.at(i) < number.digits.at(i)) return true;
        }
    }
    if (digits.size() < number.digits.size()) return false;
    if (digits.size() > number.digits.size()) return true;
    for (int i = digits.size() - 1; i >= 0; i--)
    {
        if (digits.at(i) > number.digits.at(i)) return true;
        if (digits.at(i) < number.digits.at(i)) return false;
    }
    return true;
}

bool BN::operator <= (const BN & number) const
{
    if (sign && !(number.sign)) return false;
    if (!(sign) && number.sign) return true;
    if (!(sign) && !(number.sign))
    {
        if (digits.size() < number.digits.size()) return false;
        if (digits.size() > number.digits.size()) return true;
        for (int i = digits.size() - 1; i >= 0; i--)
        {
            if (digits.at(i) > number.digits.at(i)) return true;
            if (digits.at(i) < number.digits.at(i)) return false;
        }
    }
    if (digits.size() < number.digits.size()) return true;
    if (digits.size() > number.digits.size()) return false;
    for (int i = digits.size() - 1; i >= 0; i--)
    {
        if (digits.at(i) > number.digits.at(i)) return false;
        if (digits.at(i) < number.digits.at(i)) return true;
    }
    return true;
}

BN BN::operator + (const BN & number)
{
    
    BN zero;
    if (sign && !(number.sign)) // a + (-b) = a - b
    {
        BN a, b;
        a.digits = digits;
        b.digits = number.digits;
        if (a == b) return zero;
        return (a - b);
    }
    if (!(sign) && number.sign) // -a + b = b - a
    {
        BN a, b;
        a.digits = digits;
        b.digits = number.digits;
        if (a == b) return zero;
        return (b - a);
    }
    if (!(sign) && !(number.sign)) // -a + (-b) = -(a+b)
    {
        BN a, b, c;
        a.digits = digits;
        b.digits = number.digits;
        c = a + b;
        c.sign = false;
        return c;
    }
    int len1 = digits.size();
    int len2 = number.digits.size();
    int max, min;
    if (len1 < len2)
    {
        max = len2;
        min = len1;
    }
    else
    {
        max = len1;
        min = len2;
    }
    BN w(max+1);
    DBASE tmp; int j = 0, k = 0;
    for (;j < min; j++)
    {
        tmp = (DBASE)digits.at(j) + (DBASE)number.digits.at(j) + (DBASE)k;
        w.digits.at(j) = (BASE) tmp;
        k = (BASE)(tmp >> BASE_SIZE);
    }
    for (;j < len1; j++)
    {
        tmp = (DBASE)digits.at(j) + (DBASE)k;
        w.digits.at(j) = (BASE) tmp;
        k = (BASE)(tmp >> BASE_SIZE);
    }
    for (; j < len2; j++)
    {
        tmp = (DBASE)number.digits.at(j) + (DBASE)k;
        w.digits.at(j) = (BASE) tmp;
        k = (BASE)(tmp >> BASE_SIZE);
    }
    w.digits.at(j) = k;
    while (w.digits.size() > 1 && w.digits.back() == 0) w.digits.pop_back();
    if (w.digits == zero.digits) w.sign = true;  
    return w;
}

BN & BN::operator += (const BN & number)
{
    *this = *this + number;
    return *this;
}

BN BN::operator - (const BN & number)
{
    BN zero;
    if (*this == number) return zero;
    if (sign && !(number.sign)) // a - (-b) = a + b
    {
        BN a, b;
        a.digits = digits;
        b.digits = number.digits;
        return (a + b);
    }
    if (!(sign) && number.sign) // -a - b = -(a+b)
    {
        BN a, b, c;
        a.digits = digits;
        b.digits = number.digits;
        c = a + b;
        c.sign = false;
        return c;
    }
    if (!(sign) && !(number.sign)) // -a - (-b) = b - a 
    {
        BN a, b;
        a.digits = digits;
        b.digits = number.digits;
        return b - a;
    }
    if (*this < number) // a < b, a - b = -(b - a)
    {
        BN a, b, c;
        a.digits = digits;
        b.digits = number.digits;
        c = b - a;
        c.sign = false;
        return c;
    }
    int len1 = digits.size();
    int len2 = number.digits.size();
    BN w(len1);
    int j = 0;
    DBASE tmp, k = 0;
    for (; j < len2; j++)
    {
        tmp = (DBASE)((DBASE)1 << BASE_SIZE) | (DBASE)digits.at(j);
        tmp = (DBASE)tmp - (DBASE)number.digits.at(j) - (DBASE)k;
        w.digits.at(j) = (BASE)tmp;
        k = (DBASE)(!((DBASE)tmp >> BASE_SIZE));
    }
    for (; j < len1; j++)
    {
        tmp = (DBASE)((DBASE)1 << BASE_SIZE) | (DBASE)digits.at(j);
        tmp -= (DBASE)k;
        w.digits.at(j) = (BASE)tmp;
        k = (DBASE)(!((DBASE)tmp >> BASE_SIZE));
    }
    while (w.digits.size() > 1 && w.digits.back() == 0) w.digits.pop_back();
    return w;
}

BN & BN::operator -= (const BN & number)
{
    *this = *this - number;
    return *this;
}

BN BN::operator * (const BASE & digit)
{
    int len = digits.size();
    BN w(len+1);
    int j = 0; DBASE tmp; BASE k = 0;
    for (;j < len; j++)
    {
        tmp = (DBASE)digits.at(j) * (DBASE)digit + (DBASE)k;
        w.digits.at(j) = (BASE) tmp;
        k = (BASE) (tmp >> BASE_SIZE);
    }
    w.digits.at(j) = k;
    while (w.digits.size() > 1 && w.digits.back() == 0) w.digits.pop_back();
    return w;
}

BN & BN::operator *= (const BASE & digit)
{
    *this = *this * digit;
    return *this;
}


BN BN::operator * (const BN & number)
{
    BN op1 = *this, op2 = number, zero;
    op1.sign = true; op2.sign = true;
    if (op1 == zero || op2 == zero) return zero;
    int len1 = op1.digits.size();
    int len2 = op2.digits.size();
    BN w(len1+len2);
    int i, j = 0; DBASE tmp, k;
    for (;j < len2; j++)
    {
        if (op2.digits.at(j) == 0) continue;
        k = 0;
        for (int i = 0; i < len1; i++)
        {
            tmp = (DBASE)op1.digits.at(i) * (DBASE)op2.digits.at(j) + (DBASE)w.digits.at(i+j) + (DBASE)k;
            w.digits.at(i+j) = (BASE) tmp;
            k = (BASE)(tmp >> BASE_SIZE);
        }
        w.digits.at(len1+j) = k;
    }
    while (w.digits.size() > 1 && w.digits.back() == 0) w.digits.pop_back();
    if (sign && !(number.sign)) w.sign = false;
    if (!(sign) && number.sign) w.sign = false;
    return w;
}

BN & BN::operator *= (const BN & number)
{
    *this = *this * number;
    return *this;
}

BN BN::operator / (const BASE & digit)
{
    if (!digit)
    {
        cout << "Error: cannot divide by zero";
        exit(1);
    }
    int len = digits.size();
    BASE r = 0; DBASE tmp;
    BN q(len);
    for (int j = len-1; j >= 0; j--)
    {
        tmp = (DBASE)((DBASE)r << BASE_SIZE) + (DBASE)digits.at(j);
        q.digits.at(j) = (BASE) (tmp / (DBASE)digit);
        r = (BASE) (tmp % (DBASE)digit);
    }
    while (q.digits.size() > 1 && q.digits.back() == 0) q.digits.pop_back();
    return q;
}

BN BN::operator % (const BASE & digit)
{
    if (!digit)
    {
        cout << "Error: cannot divide by zero";
        exit(1);
    }
    int len = digits.size();
    BASE r = 0; DBASE tmp;
    BN q(len);
    for (int j = len-1; j >= 0; j--)
    {
        tmp = (DBASE)((DBASE)r << BASE_SIZE) + (DBASE)digits.at(j);
        q.digits.at(j) = (BASE)(tmp / (DBASE)digit);
        r = (BASE)(tmp % digit);
    }
    while (q.digits.size() > 1 && q.digits.back() == 0) q.digits.pop_back();
    BN rest(1);
    rest.digits.at(0) = r;
    return rest;
}

BN & BN::operator %= (const BASE & digit)
{
    *this = *this % digit;
    return *this;
}

BN & BN::operator /= (const BASE & digit)
{
    *this = *this / digit;
    return *this;
}

BN BN::move(int spaces)
{
    BN result;
    if (spaces > 0)
    {
        result.digits.reserve(digits.size() + spaces);
        result.digits.resize(spaces, 0);
        result.digits.insert(result.digits.end(), digits.begin(), digits.end());
    }
    else
    {
        result.digits.reserve(digits.size());
        result.digits = digits;
    }
    return result;
}

BN BN::operator / (const BN & number)
{
    BN zero; 
    BN dividend(*this);
    BN divider(number);
    dividend.sign = true; divider.sign = true;

    int len1 = digits.size();
    int len2 = number.digits.size();
    
    if (zero == divider)
    {
        cout << "Error: cannot divide by zero";
        exit(1);
    }
    if (zero == dividend) return zero;
    if (len2 == 1)
    {
        BN c = dividend / number.digits.at(0);
        if (sign && !(number.sign)) c.sign = false;
        if (!(sign) && number.sign) c.sign = false;
        if (!(sign) && !(number.sign)) c.sign = true;
        return c;
    }
    if (dividend == divider)
    {
        zero.hexIn("1");
        return zero;
    }
    if (dividend < divider) return zero;
    
    int diff = len1 - len2;
    DBASE b = (DBASE)1 << BASE_SIZE;
    BASE d = (BASE)(b / ((DBASE)(number.digits.back()) + (DBASE)1));


    dividend = dividend * d;
    if (dividend.digits.size() == len1) dividend.digits.push_back(0);

    divider = divider * d;

    BN quotient(diff+1);
    for (int i = diff; i >= 0; i--)
    {
        if (i + len2 >= dividend.digits.size())
        {
            while (i + len2 >= dividend.digits.size()) dividend.digits.push_back(0);
        }

        DBASE qtemp = ((DBASE(dividend.digits.at(i + len2)) << BASE_SIZE) + DBASE(dividend.digits.at(i + len2 - 1))) / (DBASE(divider.digits.at(len2 - 1)));
        DBASE rtemp = ((DBASE(dividend.digits.at(i + len2)) << BASE_SIZE) + DBASE(dividend.digits.at(i + len2 - 1))) % (DBASE(divider.digits.at(len2 - 1)));

        if (qtemp == b || (DBASE(qtemp) * DBASE(divider.digits.at(len2 - 2))) > ((DBASE(rtemp) << BASE_SIZE) + DBASE(dividend.digits.at(i + len2 - 2))))
        {
            qtemp--;
            rtemp = DBASE(rtemp) + DBASE(divider.digits.at(len2 - 1));
        }
        if (rtemp < b)
        {
            if (qtemp == b || (DBASE(qtemp) * DBASE(divider.digits.at(len2 - 2))) > ((DBASE(rtemp) << BASE_SIZE) + DBASE(dividend.digits.at(i + len2 - 2))))
            {
                qtemp--;
                rtemp += divider.digits.at(number.digits.size() - 1);
            }
        }

        BN substractor = divider * (BASE)qtemp;
        substractor = substractor.move(i);

        if (dividend < substractor) {
            qtemp--;
            substractor = divider * (BASE)qtemp;
            substractor = substractor.move(i);
        }

        dividend -= substractor;

        quotient.digits.at(i) = (BASE)qtemp;
    }
    while (quotient.digits.back() == 0 && quotient.digits.size() > 1) quotient.digits.pop_back();
    if (sign && !(number.sign)) quotient.sign = false;
    if (!(sign) && number.sign) quotient.sign = false;
    return quotient;
}

BN BN::operator % (const BN & number)
{
    BN zero; 
    BN dividend(*this);
    BN divider(number);

    int len1 = digits.size();
    int len2 = number.digits.size();
    if (zero == divider)
    {
        cout << "Error: cannot divide by zero";
        exit(1);
    }
    if (len2 == 1)
    {
        BN c;
        c = dividend % number.digits.at(0);
        if (c == zero) return zero;
        if (sign && !(number.sign)) c.sign = false;
        if (!(sign) && number.sign) c.sign = false;
        if (!(sign) && !(number.sign)) c.sign = true;
        return c;
    }
    if (dividend < divider) return dividend;
    if (dividend == divider)
    {
        return zero;
    }
    
    int diff = len1 - len2;
    DBASE b = (DBASE)1 << BASE_SIZE;
    BASE d = (BASE)(b / ((DBASE)(number.digits.back()) + (DBASE)1));


    dividend = dividend * d;
    if (dividend.digits.size() == len1) dividend.digits.push_back(0);

    divider = divider * d;

    for (int i = diff; i >= 0; i--)
    {
        if (i + len2 >= dividend.digits.size())
        {
            while (i + len2 >= dividend.digits.size()) dividend.digits.push_back(0);
        }

        DBASE qtemp = ((DBASE(dividend.digits.at(i + len2)) << BASE_SIZE) + DBASE(dividend.digits.at(i + len2 - 1))) / (DBASE(divider.digits.at(len2 - 1)));
        DBASE rtemp = ((DBASE(dividend.digits.at(i + len2)) << BASE_SIZE) + DBASE(dividend.digits.at(i + len2 - 1))) % (DBASE(divider.digits.at(len2 - 1)));

        if (qtemp == b || (DBASE(qtemp) * DBASE(divider.digits.at(len2 - 2))) > ((DBASE(rtemp) << BASE_SIZE) + DBASE(dividend.digits.at(i + len2 - 2))))
        {
            qtemp--;
            rtemp = DBASE(rtemp) + DBASE(divider.digits.at(len2 - 1));
        }
        if (rtemp < b)
        {
            if (qtemp == b || (DBASE(qtemp) * DBASE(divider.digits.at(len2 - 2))) > ((DBASE(rtemp) << BASE_SIZE) + DBASE(dividend.digits.at(i + len2 - 2))))
            {
                qtemp--;
                rtemp += divider.digits.at(number.digits.size() - 1);
            }
        }

        BN substractor = divider * (BASE)qtemp;
        substractor = substractor.move(i);

        if (dividend < substractor) {
            qtemp--;
            substractor = divider * (BASE)qtemp;
            substractor = substractor.move(i);
        }

        dividend -= substractor;
    }
    dividend /= d;
    if (dividend == zero) return zero; 
    if (sign && !(number.sign)) dividend.sign = false;
    if (!(sign) && number.sign) dividend.sign = false;
    return dividend;
}

BN & BN::operator /= (const BN & number)
{
    *this = *this / number;
    return *this;
}

BN & BN::operator %= (const BN & number)
{
    *this = *this % number;
    return *this;
}

void BN::hexIn(const string & number)
{
    int len = (number.length() - 1)/(BASE_SIZE >> 2) + 1, limit = 0;
    BN newBN(len, 0);
    *this = newBN;
    int j = 0;
    int k = 0;
    BASE carrier = 0;
    char symbol;
    if (number[0] == '-') limit = 1;
    for (int i = number.length() - 1; i >= limit; i--)
    {
        symbol = number[i];
        if (('0' <= symbol) && (symbol <= '9')) carrier = symbol - '0';
        else if (('a' <= symbol) && (symbol <= 'f')) carrier = symbol - 'a' + 10;
        else if (('A' <= symbol) && (symbol <= 'F')) carrier = symbol - 'A' + 10;
        else
        {
            cout << "Error: " << symbol << " is incorrect hex digit";
            exit(1);
        }
        digits.at(j) |= carrier << k;
        k += 4;
        if (k >= BASE_SIZE) {k = 0; j++;};
    }
    while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    if (limit) sign = false;
}

void BN::decIn(const string & number)
{
    int len = number.length(), limit = 0;
    BN result(1);
    BN digit(1);
    if (number[0] == '-') limit = 1;
    for (int j = limit; j < len; j++)
    {
        if (('0' > number[j]) || (number[j] > '9'))
        {
            cout << "Error: " << number[j] << " is not a decimal digit";
            exit(1);
        }
        digit.digits.at(0) = (BASE)(number[j] - '0');
        result = result * (BASE)10 + digit;
    }
    while (result.digits.size() > 1 && result.digits.back() == 0) result.digits.pop_back();
    if (limit) result.sign = false;
    *this = result;
}

string BN::hexOut()
{
    string s;
    int k = BASE_SIZE - 4;
    int tmp = 0;
    for (int j = digits.size()-1; j >= 0;)
    {
        tmp = (digits.at(j) >> k) & (0xf); // 0xf = 1111, так выделяем четвёрку символов
	    if ((0 <= tmp) && (tmp <= 9)) s += (char)(tmp + '0');
    	else if ((10 <= tmp) && (tmp <= 15)) s += (char)(tmp - 10 + 'a');
	    k -= 4;
    	if (k < 0)
        {
            j--;
            k = BASE_SIZE-4;
        }
    }
    while (s[0] == '0' && s.length() > 1) s.erase(s.begin());
    if (!sign) s = "-" + s;
    return s;
}

istream & operator >> (istream & BNin, BN & number)
{
    string decnum;
    BNin >> decnum;
    int len = decnum.length(), limit = 0;
    BN result(1);
    BN digit(1);
    if (decnum[0] == '-') limit = 1; 
    for (int j = limit; j < len; j++)
    {
        if (('0' > decnum[j]) || (decnum[j] > '9'))
        {
            cout << "Error: " << decnum[j] << " is not a decimal digit";
            exit(1);
        }
        digit.digits.at(0) = (BASE)(decnum[j] - '0');
        result = result * (BASE)10 + digit;
    }
    while (result.digits.size() > 1 && result.digits.back() == 0) result.digits.pop_back();
    number = result;
    if (limit) number.sign = false;
    return BNin;
}
ostream & operator << (ostream & BNout, const BN & number)
{
    string result;
    BN num = number;
    num.sign = true;             
    BN zero(1);
    BN digit(1);

    if (num == zero)
    {
        BNout << "0";
        return BNout;
    }

    while (num != zero)
    {
        digit = num % (BASE)10;
        result = (char)((digit.digits.at(0)) + '0') + result;
        num = num / (BASE)10;
    }

    if (!(number.sign)) result = "-" + result;   
    BNout << result;
    return BNout;
}

string BN::decOut()
{
    string result;
    BASE tmp;
    BN num = *this; num.sign = true;
    BN digit(1);
    BN zero(1);
    char symbol;
    if (num == zero)
    {
        result += "0";
        return result;
    }
    while (num != zero)
    {
        digit = num % (BASE)10;
        result = (char)((digit.digits.at(0)) + '0') + result;
        num = num / (BASE)10;
    }
    if (!(this->sign)) result = "-" + result;
    return result;
}

BN BN::qsqr()
{
    int n = digits.size();
    BN result(2 * n + 1);
    DBASE tmp;
    QBASE qtmp;
    BASE c, u, v, carry;
    for (int i = 0; i < n; i++)
    {
        tmp = (DBASE)result.digits.at(2 * i) + (DBASE)digits.at(i) * (DBASE)digits.at(i);
        result.digits.at(2*i) = (BASE)tmp;
        u = (BASE)(tmp >> BASE_SIZE);
        c = 0;

        for (int j = i + 1; j < n; j++)
        {
            qtmp = (QBASE)result.digits.at(i+j) + (((QBASE)digits.at(i) * (QBASE)digits.at(j)) << 1) + (((QBASE)c << BASE_SIZE) + (QBASE)u);
            result.digits.at(i+j) = (BASE)qtmp;
            c = (BASE)(qtmp >> BASE_SIZE*2);
            u = (BASE)(qtmp >> BASE_SIZE);
        }

        tmp = (DBASE)result.digits.at(i+n) + (DBASE)u;
        result.digits.at(i+n) = (BASE)tmp;
        carry = (BASE)(tmp >> BASE_SIZE);

        tmp = (DBASE)result.digits.at(i+n+1) + (DBASE)c + (DBASE)carry;
        result.digits.at(i+n+1) = (BASE)tmp;
        carry = (BASE)(tmp >> BASE_SIZE);

        for (int k = i + n; carry; k++)
        {
            tmp = (DBASE)result.digits.at(k) + (DBASE)carry;
            result.digits.at(k) = (BASE)tmp;
            carry = (BASE)(tmp >> BASE_SIZE);
        }
    }
    while (result.digits.size() > 1 && result.digits.back() == 0) result.digits.pop_back();
    return result;
}

BN BN::operator ^ (const BN & number)
{
    BASE mask = 1, leading_zeros = 0, digit;
    BN result, acc = *this, num = *this, zero, one, two;
    one.hexIn("1");
    two.hexIn("2");

    if (number == zero || *this == one) return one;
    if (*this == zero) return *this;
    if (number == two) return acc.qsqr();

    int size = number.digits.size();
    if (number.digits.at(0) & mask)
    {
        result = *this;
    }
    else
    {
        result = one;
    }

    mask = (BASE)1 << (BASE_SIZE - 1);
    digit = number.digits.at(size-1);
    while (!(digit & mask))
    {
        leading_zeros++;
        mask = mask >> 1;
    }
    
    mask = 1;
    digit = number.digits.at(0);
    if (size == 1)
    {
        for (BASE j = 1; j < BASE_SIZE - leading_zeros; j++)
        {
            mask = mask << 1;
            acc = acc.qsqr();
            if (digit & mask) result *= acc;
        }
        return result;
    }

    for (BASE j = 1; j < BASE_SIZE; j++)
    {
        mask = mask << 1;
        acc = acc.qsqr();
        if (digit & mask) result *= acc;
    }

    for (int i = 1; i < size-1; i++)
    {
        mask = 1;
        digit = number.digits.at(i);
        for (BASE j = 0; j < BASE_SIZE; j++)
        {
            acc = acc.qsqr();
            if (digit & mask) result *= acc;
            mask = mask << 1;
        }
    }

    mask = 1;
    digit = number.digits.at(size-1);
    for (BASE j = 0; j < BASE_SIZE - leading_zeros; j++)
    {
        acc = acc.qsqr();
        if (digit & mask) result *= acc;
        mask = mask << 1;
    } 
    if (!(sign) && (num % two != zero)) result.sign = false;
    return result;
}

BN & BN::operator ^= (const BN & number)
{
    *this = *this ^ number;
    return *this;
}

BN BN::barmod(const BN & m, const BN & z, const BN & kminus, const BN & kplus, const BN & bkplus)
{
    BN bp = bkplus;
    int k = m.digits.size(), n = digits.size();
    if (n > 2*k)
    {
        cout << "Error: Barrett's method requires k <= 2*n condition to be satisfied" << endl;
        return *this;
    }
    BN zero, one, q, r1, r2, r;
    one.hexIn("1");
    
    q = *this;
    for (BN i = kminus; i > zero; i -= one)
{
    if (!q.digits.empty())
        q.digits.erase(q.digits.begin());
    else
        break;
}
    q *= z;
for (BN i = kplus; i > zero; i -= one)
{
    if (!q.digits.empty())
        q.digits.erase(q.digits.begin());
    else
        break;
}
    r1 = *this;
    if (bp == r1) r1 = zero;
    if (bp < r1)
    {
        if (r1.digits.size() > k + 1)
        {
            size_t remove_count = r1.digits.size() - k - 1;
            for (size_t i = 0; i < remove_count; i++)
            {
                if (r1.digits.empty()) break;
                r1.digits.pop_back();
            }
        }
        else
        {
            r1 = zero;
        }
    }
    r2 = q * m;
    if (bp == r2) r2 = zero;
    if (bp < r2)
    {
        if (r2.digits.size() > k + 1)
        {
            size_t remove_count = r2.digits.size() - k - 1;
            for (size_t i = 0; i < remove_count; i++)
            {
                if (r2.digits.empty()) break;
                r2.digits.pop_back();
            }
        }
        else
        {
            r2 = zero;
        }
    }
    if (r1 >= r2)
    {
        r = r1 - r2;
    }
    else
    {
        r = bp + r1 - r2;
    }
    while (r >= m) r -= m;
    return r;
}

BN BN::powmod(const BN & exponent, const BN & mod)
{
    BN result, zero;
    BN base = *this % mod;
    BN exp = exponent;
    result.hexIn("1");
    
    while (exp != zero)
    {
        if (exp.digits[0] & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp = exp / (BASE)2;
    }
    return result;
}

bool BN::fermaTest(const BN & t)
{
    BN number = *this;
    BN zero, one, two, three;
    one.decIn("1");
    two.decIn("2");
    three.decIn("3");
    if (number <= three) return true;
    if ((number % two) == zero) return false;
    BN a, r;
    BN min = two;
    BN max = number - two;
    BN power = number - one;
    for (BN i = zero; i < t; i += one)
    {
        a.randomizeRange(min, max);
        r = a.powmod(power, number);
        if (r != one) return false;
    }
    return true;
}

BN BN::Jacobi(const BN & n1)
{
    BN a = *this;
    BN zero;
    if (a == zero) return zero;
    BN one; one.hexIn("1");
    if (a == one) return one;
    BN s, k = zero, a1 = a, two, three, four, seven, eight, n = n1;
    two.hexIn("2");
    three.hexIn("3");
    four.hexIn("4");
    seven.hexIn("7");
    eight.hexIn("8");
    while(a1 % two == zero) {k++; a1 /= two;}
    if (k % two == zero) s = one;
    else if (n % eight == one || n % eight == seven) s = one;
    else s.hexIn("-1");
    if (n % four == three && a1 % four == three) s.signSet(!(s.signGet()));
    if (a1 == one) return s;
    else
    {
        const BN ca1 = a1;
        BN mn = n % a1;
        return (s * mn.Jacobi(ca1));
    }
}

bool BN::SolStrTest(const BN & t)
{
    BN number = *this;
    BN zero, one, two, three;
    one.decIn("1");
    two.decIn("2");
    three.decIn("3");
    if (number <= three) return true;
    if ((number % two) == zero) return false;
    BN a, r;
    BN min = two;
    BN max = number - two;
    BN power = (number - one) / 2;
    BN falseRes1 = one, falseRes2 = number - one;
    for (BN i = zero; i < t; i += one)
    {
        a.randomizeRange(min, max);
        r = a.powmod(power, number);
        if (r != falseRes1 && r != falseRes2) return false;
        BN s = a.Jacobi(number);
        if (s < zero) s += number;
        if (r != s) return false; 
    }
    return true;
}

bool BN::MilRabTest(const BN & tt)
{
    BN number = *this, t = tt;
    BN zero, one, two, three;
    one.decIn("1");
    two.decIn("2");
    three.decIn("3");
    if (number <= three) return true;
    if ((number % two) == zero) return false;
    BN s, r = number - one, b, y, i, j;
    while ((r % 2) == zero)
    {
        s++;
        r /= two;
    }
    BN min = two;
    BN max = number - two;
    BN falseRes1 = one, falseRes2 = number - one;
    for (BN i = zero; i < t; i += one)
    {
        b.randomizeRange(min, max);
        y = b.powmod(r, number);
        if (y != falseRes1 && y != falseRes2)
        {
            j = one;
            while (j < s && y != falseRes2)
            {
                y = y.powmod(two, number);
                if (y == one) return false;
                j++;
            }
            if (y != falseRes2) return false;
        }
    }
    return true;
}
 

set<BN> BN::factorize()
{
    BN number = *this, d, sqr, zero;
    set<BN> factors;
    d.decIn("2");
    while (d * d <= number)
    {
        while (number % d == zero)
        {
            factors.insert(d);
            number /= d;
        }
        d++;
    }
    if (number > 1) factors.insert(number);
    return factors;
}

bool BN::LucasTest(const BN & t, set<BN> fs)
{
    BN number = *this;
    BN zero, one, two, three;
    one.decIn("1");
    two.decIn("2");
    three.decIn("3");
    if (number <= three) return true;
    if ((number % two) == zero) return false;
    BN nmo = number - one, a, min = two, max = number - two, power, falseRes1 = one, falseRes2 = number - one;
    set<BN> factors = fs;
    bool flag;
    for (BN i = zero; i < t; i += one)
    {
        a.randomizeRange(min, max);
        power = nmo;
        flag = true;
        if (a.powmod(power, number) != one) return false;
        for (BN p : factors)
        {
            power = nmo / p;
            if (a.powmod(power, number) == one) flag = false;
        }
        if (flag) return true;
    }
    return false;
}

BN BN::strPrimeGen(const int & size, const int & tt)
{
    BN s, t, r, p, p0, i0, j0, g, one, two;
    int ttt = tt;
    g.decIn(to_string(ttt));
    one.hexIn("1");
    two.hexIn("2");
    bool isSPrime = false, isTPrime = false, isRPrime = false, isPPrime = false;
    while (!isSPrime)
    {
        s.randomize(size);
        if (s.MilRabTest(g)) isSPrime = true;
    }
    while (!isTPrime)
    {
        t.randomize(size);
        if (t.MilRabTest(g)) isTPrime = true;
    }
    i0.randomize(size);
    while (!isRPrime)
    {
        r = two * i0 * t + one;
        if (r.MilRabTest(g)) isRPrime = true; 
        i0++;
    }
    BN power = r - two;
    p0 = two * s.powmod(power, r) * s - one;
    j0.randomize(size);
    while (!isPPrime)
    {
        p = two * j0 * r * s + p0;
        if (p.MilRabTest(g)) isPPrime = true;
        j0++;
    }
    *this = p;
    return *this;
}

void BN::randomizeRange(BN min, BN max)
{
    if (!(min.sign) && !(max.sign))
    {
        std::uniform_int_distribution<int> lenRange(max.digits.size(), min.digits.size());
        int randLength = lenRange(gen);
        digits.resize(randLength, 0);
        do {
            for (int i = randLength - 1; i >= 0; i--) digits.at(i) = dis(gen);
        } while (*this < min || max < *this);
        sign = false;
    }
    else if (!(min.sign) && max.sign)
    {
        BN bmax = max - min;
        std::uniform_int_distribution<int> lenRange(1, bmax.digits.size());
        int randLength = lenRange(gen);
        digits.resize(randLength, 0);
        do {
            for (int i = randLength - 1; i >= 0; i--) digits.at(i) = dis(gen);
        } while (bmax < *this);
        *this = *this + min;
    }
    else if (min.sign && max.sign)
    {
        std::uniform_int_distribution<int> lenRange(min.digits.size(), max.digits.size());
        int randLength = lenRange(gen);
        digits.resize(randLength, 0);
        do {
            for (int i = randLength - 1; i >= 0; i--) digits.at(i) = dis(gen);
        } while (*this < min || max < *this);
    }
    else
    {
        cout << "Error: wrong range borders at randomizing\n";
        exit(1);
    }
}

void BN::randomize(const int & length)
{
    if (length == 1)
    {
        digits.resize(length, 0);
        digits.at(0) = dis(gen);
    }
    else
    {
        std::uniform_int_distribution<int> lenRange(1, length);
        int randLength = lenRange(gen);
        //std::uniform_int_distribution<int> sgnRange(0, 1);
        //int sgn = sgnRange(gen);
        digits.resize(randLength, 0);
        for (int i = randLength - 1; i >= 0; i--) digits.at(i) = dis(gen);
        //if (sgn) {sign = true;} else {sign = false;}
        while (digits.size() > 1 && digits.back() == 0) digits.pop_back();
    }
}

