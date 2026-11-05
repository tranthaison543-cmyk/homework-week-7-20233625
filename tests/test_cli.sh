#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/gf2mul.exe"

cd "$ROOT_DIR"

echo "Building program..."

gcc -std=c11 -Wall -Wextra -Wpedantic -Werror \
    src/main.c -o "$BIN"

pass=0
fail=0

run_case() {
    local name="$1"
    local field="$2"
    local operands="$3"
    local expected="$4"
    local output

    output="$(printf '%s\n%s\n' "$field" "$operands" | "$BIN")"

    if grep -Fq "A * B = $expected" <<< "$output"; then
        echo "PASS: $name"
        pass=$((pass + 1))
    else
        echo "FAIL: $name"
        echo "$output"
        fail=$((fail + 1))
    fi
}

echo
echo "Testing GF(2^4)..."

run_case "Zero multiplication" 1 "0 5" "0x0"
run_case "Multiplication by one" 1 "1 7" "0x7"
run_case "Reduction modulo 0x13" 1 "7 8" "0xD"
run_case "Polynomial multiplication" 1 "3 3" "0x5"
run_case "Maximum operands" 1 "F F" "0xA"

echo
echo "Testing GF(2^8)..."

run_case "Zero multiplication" 2 "0 57" "0x0"
run_case "Multiplication by one" 2 "1 AE" "0xAE"
run_case "Reduction modulo 0x11B" 2 "57 83" "0xC1"
run_case "Inverse relationship" 2 "53 CA" "0x1"
run_case "Maximum operands" 2 "FF FF" "0x13"

echo
echo "Testing invalid input..."

output="$(printf '1\n10 2\n' | "$BIN" 2>&1 || true)"

if grep -Fq "Error" <<< "$output"; then
    echo "PASS: Reject out-of-range operand"
    pass=$((pass + 1))
else
    echo "FAIL: Reject out-of-range operand"
    echo "$output"
    fail=$((fail + 1))
fi

echo
echo "=============================="
echo "Passed: $pass"
echo "Failed: $fail"
echo "=============================="

if [[ "$fail" -ne 0 ]]; then
    exit 1
fi
