"""
Помощник для контрольной: методы Диксона, квадратичного решета, цепных дробей.
Запуск:  python3 factor_helper.py dixon 2599 2 3 5 7
         python3 factor_helper.py qs    1177 -1 2 3 7
         python3 factor_helper.py cf    2641 -1 2 3 5 7
Дальше идёт: метод, n, факторная база (числа через пробел; -1 допустимо).
"""
import sys
from math import isqrt, gcd
from itertools import product


def factor_over_base(b, base):
    """Разложить b по базе. Вернуть список показателей или None, если не гладкое."""
    e = [0] * len(base)
    if b == 0:
        return None
    if b < 0:
        if -1 not in base:
            return None
        e[base.index(-1)] = 1
        b = -b
    for j, p in enumerate(base):
        if p == -1:
            continue
        while b % p == 0:
            b //= p
            e[j] += 1
    return e if b == 1 else None


# ---------- генераторы пар (a_i, b_i) ----------
def pairs_dixon(n, limit=3000):
    for a in range(isqrt(n) + 1, isqrt(n) + 1 + limit):
        yield a % n, a * a % n          # b = a^2 mod n, реальное значение


def pairs_qs(n, limit=3000):
    m = isqrt(n)
    yield m, m * m - n                  # x = 0
    for x in range(1, limit):
        for xx in (x, -x):
            yield (xx + m) % n, (xx + m) ** 2 - n


def pairs_cf(n, limit=60):
    a0 = isqrt(n)
    m, d, a = 0, 1, a0
    Pm, P = 1, a0
    for i in range(limit):
        m = a * d - m
        d = (n - m * m) // d            # d_{i+1}
        a = (a0 + m) // d
        yield P % n, (-1) ** (i + 1) * d   # b_i = P_i^2 - n Q_i^2
        Pm, P = P, a * P + Pm


# ---------- линейная алгебра над GF(2) ----------
def null_space(rows):
    """rows[i] = целое-битовая маска строки V. Вернуть базис решений c: sum c_i*row_i = 0."""
    t = len(rows)
    work = [(rows[i], 1 << i) for i in range(t)]   # (вектор, какие исходные строки в нём)
    basis, deps = {}, []
    for vec, comb in work:
        while vec:
            hb = vec.bit_length() - 1
            if hb in basis:
                bv, bc = basis[hb]
                vec ^= bv
                comb ^= bc
            else:
                basis[hb] = (vec, comb)
                break
        if vec == 0:
            deps.append(comb)
    return deps


def main():
    method, n = sys.argv[1], int(sys.argv[2])
    base = [int(x) for x in sys.argv[3:]]
    k = len(base)
    gen = {"dixon": pairs_dixon, "qs": pairs_qs, "cf": pairs_cf}[method](n)

    rel = []          # (a, b, показатели)
    print(f"n = {n}, база S = {base}, k = {k}\n")
    print("Гладкие пары:")
    for a, b in gen:
        e = factor_over_base(b, base)
        if e is None:
            continue
        rel.append((a, b, e))
        vec = [x % 2 for x in e]
        print(f"  a = {a:6}  b = {b:8}  показатели {e}  mod 2 {vec}")
        if len(rel) < k + 1:
            continue                    # нужно t = k+1 пар
        # матрица V как битовые маски
        rows = [sum((x % 2) << j for j, x in enumerate(r[2])) for r in rel]
        deps = null_space(rows)
        # перебираем все ненулевые комбинации базисных решений
        for coef in product([0, 1], repeat=len(deps)):
            if not any(coef):
                continue
            c = 0
            for ci, dv in zip(coef, deps):
                if ci:
                    c ^= dv
            I = [i for i in range(len(rel)) if (c >> i) & 1]
            x, bprod = 1, 1
            for i in I:
                x = x * rel[i][0] % n
                bprod *= rel[i][1]
            if bprod < 0:
                continue
            y = isqrt(bprod)
            if y * y != bprod:
                continue
            ok = (x - y) % n != 0 and (x + y) % n != 0
            print(f"\nI = {[rel[i][0] for i in I]}: x = {x}, y = {y}, "
                  f"{'нетривиально' if ok else 'x = ±y, пропускаем'}")
            if ok:
                d = gcd(x + y, n)
                print(f"НОД(x+y, n) = {d},  n = {d} * {n // d}")
                return
    print("делитель не найден, расширьте базу")


if __name__ == "__main__":
    main()
