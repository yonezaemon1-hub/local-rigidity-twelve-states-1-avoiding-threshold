$ErrorActionPreference = 'Stop'

python paper13_family_audit.py | Out-File -Encoding ascii family_actual.txt
if ((Compare-Object (Get-Content paper13_family_audit_output.txt) (Get-Content family_actual.txt)).Count -ne 0) { throw 'family audit output mismatch' }

python paper13_isomorphism_check.py | Out-File -Encoding ascii isomorphism_actual.txt
if ((Compare-Object (Get-Content paper13_isomorphism_check_output.txt) (Get-Content isomorphism_actual.txt)).Count -ne 0) { throw 'isomorphism output mismatch' }

g++ -O2 -std=c++17 paper13_local_rigidity_a12_fast.cpp -o paper13_fast.exe
.\paper13_fast.exe | Out-File -Encoding ascii fast_actual.txt
if ((Compare-Object (Get-Content paper13_local_rigidity_a12_fast_output.txt) (Get-Content fast_actual.txt)).Count -ne 0) { throw 'fast checker output mismatch' }

g++ -O2 -std=c++17 paper13_local_rigidity_a12.cpp -o paper13_reference.exe
.\paper13_reference.exe | Out-File -Encoding ascii fullref_actual.txt
if ((Compare-Object (Get-Content paper13_local_rigidity_a12_fullref_output.txt) (Get-Content fullref_actual.txt)).Count -ne 0) { throw 'reference checker output mismatch' }

Get-Content SHA256SUMS.txt | ForEach-Object {
    if ($_ -match '^([0-9a-f]{64})\s+(.+)$') {
        $expected = $matches[1].ToUpperInvariant()
        $path = $matches[2]
        $actual = (Get-FileHash $path -Algorithm SHA256).Hash
        if ($actual -ne $expected) { throw "SHA256 mismatch: $path" }
    }
}

Write-Host 'FULL_REPRODUCTION=PASS'
