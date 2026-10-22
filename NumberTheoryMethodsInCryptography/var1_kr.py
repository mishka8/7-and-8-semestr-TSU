''' python3 var1_kr.py 2599 2 3 5 7
'''
from math import isqrt


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


def pairs_dixon(n, limit=3000):
    for a in range(isqrt(n) + 1, isqrt(n) + 1 + limit):
        yield a % n, a * a % n          # b = a^2 mod n


def dixon_system(n, base, limit=3000):
    """Метод Диксона: вернуть список гладких пар (a, b, e) и матрицу над GF(2)."""
    rel = []          # (a, b, показатели)
    rows = []         # строки матрицы V по модулю 2 (списки 0/1)
    for a, b in pairs_dixon(n, limit):
        e = factor_over_base(b, base)
        if e is None:
            continue
        rel.append((a, b, e))
        vec = [x % 2 for x in e]
        rows.append(vec)
        print(f"  a = {a:6}  b = {b:8}  показатели {e}  mod 2 {vec}")
    return rel, rows


# ---------- линейная алгебра над GF(2) ----------
def null_space(rows):
    """rows[i] = целое-битовая маска строки V.
    Вернуть базис решений c: sum c_i*row_i = 0 над GF(2)."""
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


if __name__ == "__main__":
    # пример: n = 2599, база S = {2, 3, 5, 7}
    n = 2599
    base = [2, 3, 5, 7]
    MAX_DEPS = 10          # <-- ограничение на число выводимых зависимостей можно выводить больше

    print(f"n = {n}, база S = {base}, k = {len(base)}\n")
    print("Гладкие пары (метод Диксона):")
    rel, rows = dixon_system(n, base)

    print("\nМатрица над GF(2) (строки — пары, столбцы — элементы базы):")
    for r in rows:
        print("  ", r)

    # битовые маски строк для null_space
    masks = [sum((x % 2) << j for j, x in enumerate(e)) for (_, _, e) in rel]
    deps = null_space(masks)

    print(f"\nЛинейные зависимости над GF(2) (первые {MAX_DEPS} из {len(deps)}):")
    for d in deps[:MAX_DEPS]:
        I = [i for i in range(len(rel)) if (d >> i) & 1]
        print("  строки:", I, " -> a =", [rel[i][0] for i in I])
