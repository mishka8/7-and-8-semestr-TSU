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

void first_lab()
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

    num_test.decIn("34717");
    numbers.push_back(num_test);


    full_trial_divisor_method(numbers);
}

/////////////////////////third lab
BN newtonRoot(const BN& num, unsigned int degree)//корень n-ой степени с помощью метода ньютона
{
    BN zero = Uint64MakeBN(0);
    BN one = Uint64MakeBN(1);
    BN two = Uint64MakeBN(2);

    if (num <= zero) return zero;
    if (num == one) return one;
    if (degree == 0) return one;
    if (degree == 1) return num;

    BN num_copy = num;
    BN prew = num;
    BN n1 = Uint64MakeBN(degree - 1);
    BN n  = Uint64MakeBN(degree);

    while (true)
    {
        BN prewPow = prew ^ n1;// x_i^(k-1)
        BN tmp = num_copy / prewPow;// N / x_i^(k-1)
        BN next = (n1 * prew + tmp) / n;// ((k-1)*x_i + N/x_i^(k-1)) / k

        if (next == prew) return next;
        if (next > prew) return prew;

        prew = next;
    }
}

BN findDToOlweyMethod(BN n)//находим д с чего начинать будем взяли нижнюю границу
{
    BN num_copy = n;
    BN two = Uint64MakeBN(2);
    BN one = Uint64MakeBN(1);
    return two * newtonRoot(num_copy, 3) + one;
}


//метод факторизации спец назначения т.к. находит только большие делители 
//идея метода состоит в угадывании след значения на основе прошлых и корректировке в некоторых случаях
//всего выполняется два раза деление вначале
//если делители достаточно велики то количество операций будет небольшим
//n = d[k] * q[k] + r[k]
//тогда последовательность неполных частных q будет образовывать или весли себя почти как ариф прогрессия
//а r[k] имееет квадратичное поведение
//значит мы можем предсказывать след значения
BN olwey_method(const BN& num, BN d)
{
    BN zero = Uint64MakeBN(0);
    BN one = Uint64MakeBN(1);
    BN two = Uint64MakeBN(2);
    BN four = Uint64MakeBN(4);
    
    BN num_copy = num;

    BN p = Uint64MakeBN(1);

    if (num_copy % two == zero || num <= one)
    {
        cout << "Нужно не четное число, больше единицы" << endl;
        return p;
    }

    BN t_rab = Uint64MakeBN(8);
    if (num_copy.MilRabTest(t_rab))
    {
        cout << "Простое" << endl;
        return p;
    }

    BN s = newtonRoot(num_copy, 2);

    if (d < (two * newtonRoot(num_copy, 3) + one) || d > s || d % two == zero)
    {
        cout << "не подходит по условиям, должно быть нечетным, больше чем s, подходить под формулу" << endl;
        return p;
    }

    BN r = num_copy % d;
    BN r_prev = num_copy % (d - two);
    BN q = four * ((num_copy / (d - two)) - (num_copy / d));

    while (d <= s)
    {
        d += two;
        
        if (d > s) 
        {
            cout << "число - " << num << " не имеет делителей в диапозоне - [ " << d - two << ", " << s << " ]" << endl;
            break;
        }

        BN r_new = two * r - r_prev + q;
        r_prev = r; 
        r = r_new;

        if (r < zero) 
        { 
            r += d; 
            q += four; 
        }

        while (r >= d) 
        { 
            r -= d;
            q -= four; 
        }

        if (r == zero) 
        { 
            p = d; 
            break;
        }
    }

    return p;
}

void full_olwey_method(vector<BN> test)
{   
    BN one = Uint64MakeBN(1);
    for(int i = 0; i < test.size(); i++)
    {
        BN n = test[i];

        BN d = findDToOlweyMethod(n);
        BN res = olwey_method(n, d);

        if (res == one)
        {
            continue;
        }
        else 
        {
            cout << "число " << n << " - найден делитель " << res << endl;
        }
    }
}

void second_lab()
{
    vector<BN> numbers;

    numbers.push_back(Uint64MakeBN(252109));//143
    numbers.push_back(Uint64MakeBN(128333));//нет корня 
    numbers.push_back(Uint64MakeBN(820621));//нет корня
    numbers.push_back(Uint64MakeBN(7737509));//923
    numbers.push_back(Uint64MakeBN(67435217));//7549
    numbers.push_back(Uint64MakeBN(29202331261007));//1078639
    numbers.push_back(Uint64MakeBN(4594246750342037099));//3997417

    BN num_test;
    num_test.decIn("140774569922349479203");//11650417
    numbers.push_back(num_test);

    // num_test.decIn("3471234723723487");
    // numbers.push_back(num_test);

    // num_test.decIn("34717");
    // numbers.push_back(num_test);

    full_olwey_method(numbers);
}

//поиск ближайших к корню n делителей числа n   
//идея в представлении факторизируемового числа в виде разности двух квадраьтов
//алгоритм состоит в опробовании всех x от n^1/2 до (n + 1) / 2
//вычесляем третий шаг без возведения в квадрат 
//метод ферма эффективен когда делители числа близки к n^1/2
//для чисел с маленькими делителями он работает медленно 

vector<BN> method_ferma(const BN& n)
{
    BN zero = Uint64MakeBN(0);
    BN one = Uint64MakeBN(1);
    BN two = Uint64MakeBN(2);
    BN num = n;
    vector<BN> d(2);

    if(num % 2 == 0 || n <= one)//ошибочные случаи
    {
        cout << "num not correct" << endl;
        d[0] = 1; d[1] = 1;
        return d;
    }

    BN t = Uint64MakeBN(5);
    if(num.MilRabTest(t))//првоерка на пустоту 
    {
        cout << "prost" << endl;
        d[0] = 1; d[1] = n;
        return d;
    }

    BN x = newtonRoot(n, 2);
    if (x.qsqr() == n)//полный квадрат a = b = n^1/2 = x
    {
        d[0] = x;
        d[1] = x;
        return d;
    }
    
    BN end_while = (num + one) / two;//условие выхода 
    BN z = x.qsqr() - num;//начальное z

    while(x <= end_while)
    {
        x += one;//каждый раз прибавляем единицу

        if(x == end_while)
        {
            cout << "x - prostoe" << endl;
            d[0] = 1; d[1] = num;
            return d;
        }

        z = z + two * x - one;
        BN y = newtonRoot(z, 2);

        if(y.qsqr() == z)
        {
            d[0] = x + y; d[1] = x - y;
            return d;
        }
    }

    d[0] = 1; d[1] = num;
    return d;       
}

void full_method_ferma(BN num)
{
    vector<BN> delitel = method_ferma(num);
    cout << "num - " << num << endl;
    for(int i = 0; i < delitel.size(); i++)
    {
        cout << "del - " << delitel[i] << endl;
    }
}

void third_lab()
{
    BN test_num;
    //test_num.decIn("88169891");
    test_num.decIn("256590620791");
    full_method_ferma(test_num);
}
////////////////////////
//полард - вероятностный типа лас вегас иногда не возвращает результат
//метод основывается на свойствах отображений конечного множества в себя
//метод специального назначения - хорошо ищет маленькие делители 
//вероятность успеха тем выше, чем меньше d
//критерия выбора функции нет но эмперическим путем установлено 
//f(x^2 + c)
//вычисляется до тех пор пока не произойдет совпадение a == b(в файле xi == x2i)
//совпадение будет точно так как переодичность x

//вспомогательная (x^2 + c) % n (c - оптимизация чтобы возвращатьсяя во второй пункт)
BN f_help_method(BN x, BN c, BN n)
{
    BN res;

    res = (x.qsqr() + c) % n;
    return res;
}

BN method_P0_polard(const BN& n)
{
    BN num = n;
    BN two = Uint64MakeBN(2);
    BN d = Uint64MakeBN(1);
    if(num <= 1)
    {
        cout << "num not correct" << endl;
        return d;
    }
    if(num % 2 == 0)
        return two;

    BN test = Uint64MakeBN(5);
    if(num.MilRabTest(test))
    {
        cout << "prostoe" << endl;
        return d;
    }

    BN one = Uint64MakeBN(1), zero = Uint64MakeBN(0);
    BN c = one, lim_for_c = Uint64MakeBN(20);//c - будет переодическим так как +=1 каждый раз до лимита

    while(c <= lim_for_c)//из замечаний для улучшения метода 
    {
        BN a, b;
        //сделаем do while так как в первом пункте a = b = two и у нас в цикле проверка если a == b -> выход
        //не вычисляем во внешнем цикле чтобы не нагружать память
        a = two;
        b = two;
        do
        {
            a = f_help_method(a, c, num);
            b = f_help_method(f_help_method(b, c, num), c, num);
            //cout << a << " " << b << endl;

            if(a > b)//формальная реализация |a - b| просто смотрим что больше
            {
                d = num.gcd(a - b);//gcd это НОД по алгоритму евклида в BigNumbers описано как работает
            }
            if(b > a)
            {
                d = num.gcd(b - a);
            }

            if(d != num && d != one) { return d; }
        } while(a != b);
        c += 1;        
    }
    cout << "net delitelei" << endl;
    return d;
}

void fourth_lab_P0()
{
    BN num_test;
    num_test = Uint64MakeBN(19825);
    cout << "num - " << num_test << " delitel - "<< method_P0_polard(num_test) << endl;

    BN num_test_1;
    num_test_1 = Uint64MakeBN(1232854618375623997);
    cout << "num - " << num_test_1 << " delitel - "<< method_P0_polard(num_test_1) << endl;

}

//как и p0-полард так же метод спец назначения и так же является вероятностным
//метод очень быстро находит простые делители малой и средней величины
//n - B - гладкое, если все его простые делители не превосходят B
//по сути можно реализовывать с помощью функции factorize() она возвращает множество простых делителилей
//но у меня сделано подругому
//метод поларда позволяет исходя из границы гладкости B найти делитель d такой что 
//d-1 - B-гладкое
//так как у меня реализован корень с помощью метода ньютона  я испоьлзую B  (=/<=) n^(1/2)(корень)
//для противостояния метода все простые делители p факторизируемого числа должны быть такими,
//чтобы p-1 не являлось B-гладким
//метод поларда - аналог метода пробных делителей
//только мы ищем перебором не делители числа n, а делители d-1
//на практике используют используют до применения сильных методов факторизации,
//чтобы отделить небольшие простые делители 

BN method_P1_polard(const BN& n)
{
    BN zero = Uint64MakeBN(0), one = Uint64MakeBN(1), two = Uint64MakeBN(2);
    BN num = n;
    
    if(num <= one)
    {
        cout << "num not correct" << endl;
        return one;
    }
    if(num % two == zero)
        return two;

    BN t = Uint64MakeBN(8);
    if(num.MilRabTest(t))
    {
        cout << "prostoe" << endl;
        return one;
    }
    //берем 10 попыток так как отказы имеют разную природу 
    //и в п 3.3 отказ не критичен и мы можем вернуться в пункт 2 и взять дургое a
    int maxTry = 10;

    BN B = newtonRoot(num, 2);//выбираем из условия
    for (int attempt = 0; attempt < maxTry; ++attempt)
    {
        BN a, d;
        a.randomizeRange(two, num - two);

        d = a.gcd(num);

        //сразу проверяем делитель если не 1 то выходим
        if (d > one && d < num)
            return d;

        for (BN q = two; q <= B; q += one)//перебираем все простые q до B
        {
            //проверяем q что оно простое
            if (q != two && !q.MilRabTest(t)) { continue; }

            // BN e = B.log(q);
            BN e = num.log(q);

            if (e == zero) { continue; }

            BN qe = q ^ e;

            //возведение в степень по модулю a^(q^e) mod n
            a = a.powmod(qe, num);
            if(a == one) { break; }

            //считаем d по формуле
            d = (a - one).gcd(num);

            //если d меньше числа и больше единицы - значит нашли делитель
            if (d > one && d < num)
                return d;

            //если d == num значит нет делителей и a выбрано плохое а 
            //выходим и меняем a
            //if (d == num) { break; }
        }
    }
    //сюда попадаем после 10 попыток и возвращаем 1 как отказ
    cout << "not find" << endl;
    return one;
}

void fourth_lab_P1()
{
    BN test1; test1 = Uint64MakeBN(19825);
    BN test2; test2 = Uint64MakeBN(1232854618375623997);
    BN res1 = method_P1_polard(test1);
    BN res2 = method_P1_polard(test2);

    cout << "num - " << test1 << " delitel - "<< res1 << endl;
    cout << "num - " << test2 << " delitel - "<< res2 << endl;
}

//дискретный логарифм
struct LogRes {
    bool found = false;//найден ли логарифм
    BN x; // решение g^x ≡ a (mod p)
    BN h; // корень из n + 1
    BN b; // b = g^h mod p - вычисление большого шага
};

//првоерка что g - образующий Zp 
// Критерий: g^(p-1) = 1 и для каждого простого q | (p-1) верно
// g^((p-1)/q) != 1 (mod p).
// Если g не образующий, дискретный логарифм определён не для всех a.
bool Generator(const BN& g, const BN& p)
{
    BN one = Uint64MakeBN(1);
    BN zero = Uint64MakeBN(0);

    //g должно лежать в 1 до p-1
    if(g == zero || g >= p) { return false; }

    BN g_copy = g;
    BN p_copy = p;
    BN n = p_copy - one;//порядок группы Zp

    //обязательное условие что g^(p-1) = 1 
    if(g_copy.powmod(n, p) != one) { return false; }

    //расскладываем n-1 на множители с помощью функции 
    set<BN> primes = n.factorize();
    
    //проверяем для каждого простого q что равно 1
    for(const BN& q : primes)
    {
        if(g_copy.powmod(n / q, p) == one) { return false; }

    }

    return true;
}

// Основная функция: метод Гельфонда (baby-step giant-step)
// Вход: p (простое), g (образующий), a (элемент Z*_p)
// Выход: структура LogRes с полями found, x, h, b
LogRes gelfond(const BN& p, const BN& g, const BN& a, bool showTable = true)
{
    //проверяем входные данные
    LogRes res;
    res.h = Uint64MakeBN(0);
    res.b = Uint64MakeBN(0);
    res.x = Uint64MakeBN(0);

    BN zero = Uint64MakeBN(0);
    BN one = Uint64MakeBN(1);
    BN two = Uint64MakeBN(2);

    BN t = Uint64MakeBN(10);
    BN p_copy = p;
    if(!p_copy.MilRabTest(t)) 
    {
        cout << "P - должно быть простым" << endl;
        return res;
    }

    BN g_copy = g;
    if(g_copy == zero || g_copy >= p_copy)
    {
        cout << "g находится вне [1, p - 1]" << endl;
        return res;
    }

    BN a_copy = a;
    if(a_copy == zero || a_copy >= p_copy)
    {
        cout << "a находится вне [1, p - 1]" << endl;
        return res;
    }

    //проверяем что g это образующий группы если нет то выход
    if(!Generator(g_copy, p_copy))
    {
        cout << "g не образующий Zp^" << endl;
        return res;
    }

    //шаг 1 - порядок группы и h

    BN n = p_copy - one;//p - 1
    BN h = newtonRoot(n, 2);//корень второй степени из n 

    //корректируем h до нижней границы 
    //и там уже либо увеличиваем либо уменьшаем h
    while((h + one).qsqr() <= n) { h += one; }
    while(h.qsqr() > n) { h -= one; }

    //после корректировки увеличиваем на 1 чтобы гарантировать что h^2 > n
    h += one;

    //шаг 2 - b = g^h mod p
    BN b = g_copy.powmod(h, p_copy);

    //шаг 3 - вычисление больших шагов 
    //будем хранить список u для каждого значения b^u
    //BN - ключ - значение b^u mod p
    //vector<BN> - список всех u при которых у нас это значение 
    map<BN, vector<BN>> giant;

    BN current = one;
    for(BN u = one; u <= h; u += one)
    {
        current = (current * b) % p;// current = b^u
        giant[current].push_back(u);//запоминаем u для b^u
    }

    res.h = h;//большой шаг
    res.b = b;//вычисление шага 

    //тут вычисляем малые шаги 
    current = one;
    for(BN v = one; v <= h; v += one)
    {
        current = (current * g_copy) % p_copy;//current = g^v
        BN small = (a_copy * current) % p_copy;//small = a * g^v

        //ищем ключ который равен малому шагу 
        //тут идёт бинарный поиск
        auto it = giant.find(small);
        //сюда попадаем если нашли ключ и он не последний элемент
        if(it != giant.end())
        {
            // Нашли совпадение b^u = a*g^v. Может быть несколько u
            for(BN& u : it->second)
            {
                // x = (h*u - v) mod n по алгоритму 
                BN hu = ((h % n) * (u % n)) % n;
                BN x = (hu + n - (v % n)) % n;

                //обязательная проверка g^x mod p должно совпасть с а 
                //условие b^u = a * g^v
                if(g_copy.powmod(x, p) == a)
                {
                    res.found = true;
                    res.x = x;
                    return res;
                }
            }
        }
    }

    return res;
}

//тут просто вывод - обертка функции    
struct enterData 
{
    BN p;
    BN g;
    BN a;
    BN expect;
    string description;
};

vector<enterData> getData()
{
    vector<enterData> data;
    
    // Лямбда-функция для удобного добавления
    auto add = [&](QBASE p_val, QBASE g_val, QBASE a_val, QBASE exp_val, const string& desc) {
        data.push_back({
            Uint64MakeBN(p_val), Uint64MakeBN(g_val),
            Uint64MakeBN(a_val), Uint64MakeBN(exp_val), desc
        });
    };

    add(599,   22,   418, 185,  "Задание 1: log_22(418) в Z*_599");
    add(599,   22,   183, 321,  "Задание 1: log_22(183) в Z*_599");
    add(599,   70,   21,  221,  "Задание 2: log_70(21) в Z*_599");
    add(599,   70,   269, 436,  "Задание 2: log_70(269) в Z*_599");
    add(1571,  52,   647, 368,  "Задание 3: log_52(647) в Z*_1571");
    add(1571,  52,   190, 1023, "Задание 3: log_52(190) в Z*_1571");
    add(1571,  1003, 912, 146,  "Задание 4: log_1003(912) в Z*_1571");
    add(1571,  1003, 25,  86,   "Задание 4: log_1003(25) в Z*_1571");
    add(10069, 77,   625, 156,  "Задание 5: log_77(625) в Z*_10069");
    add(10069, 77,   7128,835,  "Задание 5: log_77(7128) в Z*_10069");
    add(10069, 8900, 9458,1024, "Задание 6: log_8900(9458) в Z*_10069");
    add(10069, 8900, 7330,2356, "Задание 6: log_8900(7330) в Z*_10069");
    //add(10069, 8900, 7330,2356, "Задание 6: log_8900(7330) в Z*_10069"); для тестов 

    return data;
}

void solveTask(const enterData& t, bool showTables) {
    cout << "\n--------------------------------------------------\n";
    cout << t.description << "\n";
    cout << "  p = " << t.p << "\n";
    cout << "  g = " << t.g << "\n";
    cout << "  a = " << t.a << "\n";

    LogRes res = gelfond(t.p, t.g, t.a, showTables);

    if (!res.found) {
        cout << "  Решение не найдено.\n";
        return;
    }

    cout << "  Найдено x = " << res.x << "\n";
    cout << "  h = " << res.h << ", b = " << res.b << "\n";
    cout << "  Ожидалось: " << t.expect << " -> "
         << (res.x == t.expect ? "OK" : "НЕ СОВПАЛО") << "\n";
}

// Прогон всех задач
void runAllTasks(bool showTables) {
    auto tasks = getData();
    int ok = 0;
    for (size_t i = 0; i < tasks.size(); ++i) {
        solveTask(tasks[i], showTables);
        LogRes res = gelfond(tasks[i].p, tasks[i].g, tasks[i].a, false);
        if (res.found && res.x == tasks[i].expect) ok++;
    }
    cout << "\n==================================================\n";
    cout << "Итог: " << ok << " из " << tasks.size() << " задач решены верно.\n";
}

// Меню
void fifth_gelfond() {
    auto tasks = getData();

    while (true) {
        cout << "\n==============================================\n";
        cout << "       ЛАБОРАТОРНАЯ: МЕТОД ГЕЛЬФОНДА\n";
        cout << "==============================================\n";
        cout << "1. Выполнить все 12 примеров\n";
        cout << "0. Выход\n";

        int choice;
        if (!(cin >> choice)) break;

        if (choice == 0) {
            cout << "Завершение.\n";
            break;
        }
        if (choice == 1) {
            runAllTasks(false);
        }
    }
}

struct PollardRhoResult
{
    bool found;
    BN x;
};

// Обратный элемент для взаимно простых value и modulus.
static BN pollardRhoInverse(BN value, BN modulus)
{
    BN zero = Uint64MakeBN(0);
    BN remainder = modulus;
    BN coefficient = zero;
    BN nextCoefficient = Uint64MakeBN(1);

    // Расширенный алгоритм Евклида с коэффициентами по модулю modulus.
    while (value != zero)
    {
        BN quotient = remainder / value;
        BN nextRemainder = remainder % value;
        BN product = (quotient * nextCoefficient) % modulus;
        BN updatedCoefficient = (coefficient + modulus - product) % modulus;
        remainder = value;
        value = nextRemainder;
        coefficient = nextCoefficient;
        nextCoefficient = updatedCoefficient;
    }

    return coefficient;
}

// ρ-метод Полларда для g^x = a (mod p).
// Условия задачи: p простое, g порождает группу Z*_p.
PollardRhoResult pollardRho(const BN& p, BN g, BN a)
{
    const BN zero = Uint64MakeBN(0);
    const BN one = Uint64MakeBN(1);
    const BN two = Uint64MakeBN(2);
    const PollardRhoResult failure{false, zero};

    if (p <= one || g < one || g >= p || a < one || a >= p)
        return failure;
    if (a == one)
        return {true, zero};
    if (a == g)
        return {true, one};

    BN order = p;
    order -= one;
    const BN maxExponent = order - one;

    // На каждом шаге сохраняется x = g^u * a^v (mod p).
    struct State
    {
        BN x;
        BN u;
        BN v;
    };

    const auto step = [&](State& state)
    {
        BN group = state.x % static_cast<BASE>(3);
        if (group == one)
        {
            state.x = (state.x * a) % p;
            state.v = (state.v + one) % order;
        }
        else if (group == two)
        {
            state.x = state.x.qsqr() % p;
            state.u = (state.u + state.u) % order;
            state.v = (state.v + state.v) % order;
        }
        else
        {
            state.x = (state.x * g) % p;
            state.u = (state.u + one) % order;
        }
    };

    constexpr int maxAttempts = 16;
    for (int attempt = 0; attempt < maxAttempts; ++attempt)
    {
        State slow{one, zero, zero};
        if (attempt > 0)
        {
            slow.u.randomizeRange(zero, maxExponent);
            slow.v.randomizeRange(zero, maxExponent);
            slow.x = (g.powmod(slow.u, p) * a.powmod(slow.v, p)) % p;
        }

        State fast = slow;
        do
        {
            step(slow);
            step(fast);
            step(fast);
        } while (slow.x != fast.x);

        // После совпадения решаем r * x = b (mod order).
        BN r = (slow.v + order - fast.v) % order;
        if (r == zero)
            continue;

        BN b = (fast.u + order - slow.u) % order;
        BN d = r.gcd(order);
        if (b % d != zero)
            continue;

        BN spacing = order / d;
        BN inverse = pollardRhoInverse(r / d, spacing);
        BN candidate = ((b / d) * inverse) % spacing;

        // Кандидаты имеют вид x0 + k * spacing, 0 <= k < d.
        BN power = g.powmod(candidate, p);
        BN multiplier = g.powmod(spacing, p);
        for (BN k = zero; k < d; k += one)
        {
            if (power == a)
                return {true, candidate};
            candidate += spacing;
            power = (power * multiplier) % p;
        }
    }

    return failure;
}

void sixth_lab_PollardRho()
{
    const auto tasks = getData();
    size_t passed = 0;

    for (const enterData& task : tasks)
    {
        const PollardRhoResult result = pollardRho(task.p, task.g, task.a);
        cout << task.description << " | ожидается x = " << task.expect;

        if (!result.found)
        {
            cout << " | решение не найдено\n";
            continue;
        }

        const bool matches = result.x == task.expect;
        cout << " | получено x = " << result.x << (matches ? " | OK\n" : " | ОШИБКА\n");
        if (matches)
            ++passed;
    }

    cout << "Итог: " << passed << " из " << tasks.size() << " тестов пройдено.\n";
}

int main()
{
    //first_lab();

    //second_lab();

    // third_lab();

    //fourth_lab_P0();

    //fourth_lab_P1();

    //fifth_gelfond();

    sixth_lab_PollardRho();
    
    return 0;
}
