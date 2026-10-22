''' python3 var2_kr.py 1177 -1 2 3 7 '''
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


def pairs_qs(n, limit=3000):
    """Пары (a, b) для квадратичного решета: a = x + m, b = a^2 - n."""
    m = isqrt(n)
    yield m, m * m - n                  # x = 0
    for x in range(1, limit):
        for xx in (x, -x):
            yield (xx + m) % n, (xx + m) ** 2 - n


def qs_system(n, base, limit=3000):
    """Метод квадратичного решета: вернуть гладкие пары (a, b, e) и матрицу над GF(2)."""
    rel = []          # (a, b, показатели)
    rows = []         # строки матрицы V по модулю 2 (списки 0/1)
    for a, b in pairs_qs(n, limit):
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
    # пример: n = 1177, база S = {-1, 2, 3, 7}
    n = 1177
    base = [-1, 2, 3, 7]
    MAX_DEPS = 10          # ограничение на число выводимых зависимостей

    print(f"n = {n}, база S = {base}, k = {len(base)}\n")
    print("Гладкие пары (метод квадратичного решета):")
    rel, rows = qs_system(n, base)

    # битовые маски строк для null_space — матрицу в консоль не печатаем
    masks = [sum((x % 2) << j for j, x in enumerate(e)) for (_, _, e) in rel]
    deps = null_space(masks)

    print(f"\nЛинейные зависимости над GF(2) (первые {MAX_DEPS} из {len(deps)}):")
    for d in deps[:MAX_DEPS]:
        I = [i for i in range(len(rel)) if (d >> i) & 1]
        print("  строки:", I, " -> a =", [rel[i][0] for i in I])
