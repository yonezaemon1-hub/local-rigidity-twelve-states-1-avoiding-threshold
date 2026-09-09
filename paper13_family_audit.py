#!/usr/bin/env python3
from collections import deque

def T(n):
    # q1,...,qn are represented by 0,...,n-1.
    a = [None] * n
    b = [None] * n

    a[0] = 1
    a[1] = 0
    a[2] = 0
    for i in range(3, n - 1):
        a[i] = i + 1
    a[n - 1] = 3

    b[0] = 0
    for i in range(1, n - 2):
        b[i] = i + 1
    b[n - 2] = 1
    b[n - 1] = n - 1
    return a, b

def image_mask(mask, trans):
    out = 0
    while mask:
        lsb = mask & -mask
        i = lsb.bit_length() - 1
        out |= 1 << trans[i]
        mask -= lsb
    return out

def apply_word(mask, word, a, b):
    for ch in word:
        mask = image_mask(mask, a if ch == "a" else b)
    return mask

def avoiding_forward(n, a, b):
    full = (1 << n) - 1
    dist = [None] * n
    seen = {full}
    q = deque([(full, 0)])
    while q and any(x is None for x in dist):
        mask, d = q.popleft()
        for i in range(n):
            if dist[i] is None and not ((mask >> i) & 1):
                dist[i] = d
        for trans in (a, b):
            nxt = image_mask(mask, trans)
            if nxt not in seen:
                seen.add(nxt)
                q.append((nxt, d + 1))
    return dist

def avoiding_reverse(n, a, b):
    invs = []
    for trans in (a, b):
        inv = [0] * n
        for src, dst in enumerate(trans):
            inv[dst] |= 1 << src
        invs.append(inv)

    def preimage(mask, inv):
        out = 0
        while mask:
            lsb = mask & -mask
            j = lsb.bit_length() - 1
            out |= inv[j]
            mask -= lsb
        return out

    ans = []
    for state in range(n):
        start = 1 << state
        seen = {start}
        q = deque([(start, 0)])
        value = None
        while q:
            mask, d = q.popleft()
            if mask == 0:
                value = d
                break
            for inv in invs:
                nxt = preimage(mask, inv)
                if nxt not in seen:
                    seen.add(nxt)
                    q.append((nxt, d + 1))
        ans.append(value)
    return ans

def strongly_connected(n, a, b):
    g = [[a[i], b[i]] for i in range(n)]
    rg = [[] for _ in range(n)]
    for u in range(n):
        for v in g[u]:
            rg[v].append(u)

    def reach(adj):
        seen = {0}
        q = [0]
        for u in q:
            for v in adj[u]:
                if v not in seen:
                    seen.add(v)
                    q.append(v)
        return len(seen) == n

    return reach(g) and reach(rg)

def q1_avoiding_word(n):
    return "ab" + "a" * (n - 4) + "b" + "a" * (n - 5) + "bba"

def odd_reset_word(n):
    return "ab" * n + "b" + "a" * (n - 7) + "baba"

def even_reset_word(n):
    return (
        "ab" * n + "aab" + "ab" * n
        + "b" + "a" * (n - 11) + "b" + "aaa" + "baba"
    )

SMALL_EVEN_RESET = {
    6: "abaabbababbaba",
    8: "abaababbabababbaaaba",
    10: "ababaababaabababbabababa",
}

print("PAPER13_FAMILY_AUDIT")
print("family = T_n (two-fixed-point permutation family)")
print()

# Exact independent forward/reverse subset-BFS audit.
for n in range(5, 21):
    a, b = T(n)
    fwd = avoiding_forward(n, a, b)
    rev = avoiding_reverse(n, a, b)
    assert fwd == rev
    assert strongly_connected(n, a, b)
    expected = 2 * n - 3
    if n == 5:
        # Threshold identity holds, but T_5 is not synchronizing.
        assert max(fwd) == expected
    else:
        assert fwd == [2*n-3, n-2] + list(range(1, n-1))
    print(f"n={n:2d} at1={max(fwd):2d} expected={expected:2d} forward_reverse=PASS")

print()
# Explicit q1-avoiding word family.
for n in range(6, 201):
    a, b = T(n)
    full = (1 << n) - 1
    w = q1_avoiding_word(n)
    assert len(w) == 2 * n - 3
    assert (apply_word(full, w, a, b) & 1) == 0
print("q1_explicit_word_n6_n200 = PASS")

# Explicit reset constructions.
for n in range(7, 201, 2):
    a, b = T(n)
    full = (1 << n) - 1
    out = apply_word(full, odd_reset_word(n), a, b)
    assert out and (out & (out - 1)) == 0
print("odd_reset_formula_n7_n199 = PASS")

for n in range(12, 201, 2):
    a, b = T(n)
    full = (1 << n) - 1
    out = apply_word(full, even_reset_word(n), a, b)
    assert out and (out & (out - 1)) == 0
print("even_reset_formula_n12_n200 = PASS")

for n, w in SMALL_EVEN_RESET.items():
    a, b = T(n)
    full = (1 << n) - 1
    out = apply_word(full, w, a, b)
    assert out and (out & (out - 1)) == 0
print("small_even_reset_n6_n8_n10 = PASS")

print()
print("BOUNDARY_n5 = strongly_connected_but_not_synchronizing")
print("THEOREM_COMPUTATIONAL_AUDIT = PASS_FOR_STATED_TEST_RANGES")
