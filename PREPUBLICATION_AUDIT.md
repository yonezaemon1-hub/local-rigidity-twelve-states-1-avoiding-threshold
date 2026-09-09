# Prepublication audit — Paper 13

Date: 2026-09-09
Status: publication candidate / not peer reviewed

## Claim boundary

The manuscript claims a complete exhaustive classification only inside the closed labeled transition-table Hamming ball of radius three around the explicitly defined FSV automaton A_12. It does not claim a global classification at n=12, a new general upper or lower bound, or priority for the transition family T_n itself.

## Manuscript audit

The final v6.1 fact-check records:

- mathematical core: PASS;
- computational audits: PASS;
- FSV primary-source factual claims: PASS;
- claim-boundary framing: PASS;
- novelty audit: PASS for internal preprint/publication-preflight purposes;
- Vorel-thesis transition-table comparison: unresolved but non-blocking because the paper makes no priority claim for T_n itself.

Peer review has not been performed.

## Fresh computational reproduction

The release-candidate computational artifacts were rerun on 2026-09-09 before publication packaging.

1. `paper13_family_audit.py`: output exactly matches `paper13_family_audit_output.txt`.
2. `paper13_isomorphism_check.py`: output exactly matches `paper13_isomorphism_check_output.txt`.
3. `paper13_local_rigidity_a12_fast.cpp`: compiled with `g++ -O2 -std=c++17`; output exactly matches the frozen fast-checker log. Runtime in the audit environment was about 2 seconds.
4. `paper13_local_rigidity_a12.cpp`: compiled with `g++ -O2 -std=c++17`; output exactly matches the frozen full-reference log. Runtime in the audit environment was about 37.4 seconds.

The reference and fast implementations agree on the exhaustive counts and survivor set. Independent classification gives 8 non-center survivors in the A_12 isomorphism class, 2 in the T_12 class, and 0 other classes.

## Exact radius-three counts

Excluding the center A_12:

- Hamming distance 1: 264 total, 238 strongly connected + synchronizing, 0 with at_1 >= 21.
- Hamming distance 2: 33,396 total, 27,047 strongly connected + synchronizing, 2 with at_1 >= 21.
- Hamming distance 3: 2,693,944 total, 1,905,177 strongly connected + synchronizing, 8 with at_1 >= 21.
- Total: 2,727,604 total, 1,932,462 strongly connected + synchronizing, 10 with at_1 >= 21, 0 with at_1 >= 22.

Including center A_12:

- 2,727,605 labeled automata;
- 1,932,463 strongly connected + synchronizing automata;
- 11 automata with at_1 >= 21;
- 9 in the A_12 isomorphism class and 2 in the T_12 class;
- none with at_1 >= 22.

## Family audit

The computational family audit verifies the exact threshold 2n-3 for T_n over n=5..20, agrees between forward and reverse subset calculations, checks the explicit q1-avoiding word through n=200, checks the odd reset construction through n=199, the even reset construction through n=200, and the small even reset words at n=6,8,10. The theorem itself is analytic and stated for n>=6; n=5 is correctly identified as strongly connected but not synchronizing.

## Publication status

DOIs are not yet inserted in this pre-DOI release candidate. The final publication freeze must occur only after the Paper and Software DOIs have been reserved, inserted into manuscript and metadata, and the PDF has been rebuilt and re-hashed.
