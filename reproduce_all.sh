#!/usr/bin/env bash
set -euo pipefail

CXX=${CXX:-g++}
PYTHON=${PYTHON:-python3}

$PYTHON paper13_family_audit.py > family_actual.txt
diff -u paper13_family_audit_output.txt family_actual.txt

$PYTHON paper13_isomorphism_check.py > isomorphism_actual.txt
diff -u paper13_isomorphism_check_output.txt isomorphism_actual.txt

$CXX -O2 -std=c++17 paper13_local_rigidity_a12_fast.cpp -o paper13_fast
./paper13_fast > fast_actual.txt
diff -u paper13_local_rigidity_a12_fast_output.txt fast_actual.txt

$CXX -O2 -std=c++17 paper13_local_rigidity_a12.cpp -o paper13_reference
./paper13_reference > fullref_actual.txt
diff -u paper13_local_rigidity_a12_fullref_output.txt fullref_actual.txt

sha256sum -c SHA256SUMS.txt

echo FULL_REPRODUCTION=PASS
