# Paper 13 final fact-check — Draft v6.1 — 2026-09-09

Status: internal publication preflight; not peer review.

## FSV A5 point — DIRECT PRIMARY-SOURCE PASS

Ferens, Szykuła, and Vorel, *Lower Bounds on Avoiding Thresholds* (MFCS 2021), Section 2.1 states that there are particular synchronizing automata with 1-avoiding threshold `2n-2` and explicitly says that for automaton `A_5` the value is `8`. Since `2*5-2=8`, Draft v6's sentence "their paper notes that A_5 has threshold 2n-2=8" is accurate.

The same paper's Theorem 4 states `at_1(A_n)=2n-3` only for `n>=7`, and immediately after the proof says that the lower bound for cases `n<=6` was confirmed experimentally. Draft v6.1 therefore correctly warns against reading that sentence as an extension of Theorem 4 to every smaller `A_n`.

## Minor citation-order correction

Changed `\\cite{Vorel2018,Volkov2022}` to `\\cite{Volkov2022,Vorel2018}` so the rendered citation is `[3,6]` instead of `[6,3]`. No mathematical or substantive claim changed.

## Visual regression

Draft v6.1 remains 8 pages. Render comparison against v6 changes only page 1 (draft label v6 -> v6.1) and page 7 (citation order). Pages 2-6 and 8 are pixel-identical at the comparison resolution.

## Verdict

- Mathematical core: PASS
- Computational audits: PASS
- FSV factual claims used in the v6 priority-independent framing: PASS
- Claim-boundary framing: PASS
- Novelty audit: PASS for internal preprint/publication-preflight purposes, with no claim of peer-reviewed priority guarantee
- Vorel thesis transition-table direct comparison: unresolved but non-blocking because no priority claim is made for `T_n` itself and the principal novelty claim is the radius-three local classification
