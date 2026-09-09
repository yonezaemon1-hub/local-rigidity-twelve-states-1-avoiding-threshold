# Local Rigidity at Twelve States for the 2n-3 1-Avoiding Threshold with a Two-Fixed-Point Witness Family

Ryutaro Yonezu — Independent Researcher

Status: pre-DOI preprint release candidate / not peer reviewed.

## Main results

This repository accompanies the preprint

**Local Rigidity at Twelve States for the 2n-3 1-Avoiding Threshold with a Two-Fixed-Point Witness Family**.

The paper establishes two narrow results.

1. **Radius-three local classification at 12 states.** In the full labeled transition-table Hamming ball of radius three around the Ferens–Szykuła–Vorel automaton `A_12`, every binary strongly connected synchronizing automaton with 1-avoiding threshold at least `21 = 2*12-3` is isomorphic either to `A_12` or to the two-fixed-point witness `T_12`. No automaton in the ball has threshold at least 22.
2. **Exact analysis of the witness family `T_n`.** For every `n >= 6`, the explicitly defined family `T_n` is strongly connected and synchronizing and satisfies `at_1(T_n) = 2n-3`.

No priority claim is made for the transition family `T_n` itself. The novelty claim is restricted to the results proved and computationally certified here, especially the exhaustive radius-three local classification.

## Exhaustive computation at n=12

The radius-three search excludes the center `A_12` itself and enumerates every labeled transition table at Hamming distance 1, 2, or 3 from it.

- distance 1: 264 automata
- distance 2: 33,396 automata
- distance 3: 2,693,944 automata
- total non-center automata: 2,727,604

Among them, 1,932,462 are strongly connected and synchronizing. Exactly 10 non-center automata have 1-avoiding threshold at least 21:

- 8 are isomorphic to `A_12`
- 2 are isomorphic to `T_12`
- 0 belong to any other isomorphism class

Including the center `A_12`, the closed radius-three ball therefore contains 2,727,605 labeled automata, 1,932,463 strongly connected synchronizing automata, and 11 threshold-21 survivors. No automaton in the closed ball has 1-avoiding threshold at least 22.

## Independent computational checks

The finite theorem is checked in two independent exhaustive implementations:

- `paper13_local_rigidity_a12.cpp` — reference forward-subset implementation
- `paper13_local_rigidity_a12_fast.cpp` — reverse-preimage-gated implementation

The survivor set is then independently classified by

- `paper13_isomorphism_check.py`

The family formulas, synchronization constructions, and forward/reverse avoiding-distance calculations are audited by

- `paper13_family_audit.py`

Frozen expected outputs are included for every checker.

## One-command reproduction

Linux / GCC / Python 3:

```bash
chmod +x reproduce_all.sh
./reproduce_all.sh
```

Windows / PowerShell with `g++` and Python on `PATH`:

```powershell
.\reproduce_all.ps1
```

A successful run ends with:

```text
FULL_REPRODUCTION=PASS
```

The reference radius-three checker is exhaustive and may take substantially longer than the fast checker.

## Claim boundary

This repository does **not** claim:

- a proof of the global `n=12` extremal problem;
- a new numerical lower bound beyond `2n-3`;
- priority for the transition family `T_n` itself;
- a complete classification outside the radius-three neighborhood of `A_12`;
- peer review.

## Files

- `paper.tex` — manuscript source
- Final preprint PDF — to be added only after Paper/Software DOI reservation, final rebuild, and SHA freeze
- `paper13_family_audit.py` — family/synchronization/avoiding-distance audit
- `paper13_local_rigidity_a12.cpp` — exhaustive reference checker
- `paper13_local_rigidity_a12_fast.cpp` — exhaustive fast checker
- `paper13_isomorphism_check.py` — independent survivor classification
- `*_output.txt` — frozen expected outputs
- `FINAL_FACTCHECK.md` — final manuscript fact-check
- `PREPUBLICATION_AUDIT.md` — release audit
- `SHA256SUMS.txt` — integrity manifest

## Licenses

- Manuscript text and PDF: CC BY 4.0.
- Computational source code and reproducibility package: MIT License unless otherwise noted.
