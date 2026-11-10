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

run_error_case() {
    local name="$1"
    local input="$2"
    local expected="$3"
    local output

    output="$(printf '%b' "$input" | "$BIN" 2>&1 || true)"

    if grep -Fq "$expected" <<< "$output"; then
        echo "PASS: $name"
        pass=$((pass + 1))
    else
        echo "FAIL: $name"
        echo "$output"
        fail=$((fail + 1))
    fi
}
run_commutativity_case() {
    local name="$1"
    local field="$2"
    local a="$3"
    local b="$4"
    local output_ab
    local output_ba
    local result_ab
    local result_ba

    output_ab="$(printf '%s\n%s %s\n' "$field" "$a" "$b" | "$BIN")"
    output_ba="$(printf '%s\n%s %s\n' "$field" "$b" "$a" | "$BIN")"

    result_ab="$(sed -n 's/^A \* B = //p' <<< "$output_ab")"
    result_ba="$(sed -n 's/^A \* B = //p' <<< "$output_ba")"

    if [[ -n "$result_ab" && "$result_ab" == "$result_ba" ]]; then
        echo "PASS: $name"
        pass=$((pass + 1))
    else
        echo "FAIL: $name"
        echo "A * B = $result_ab"
        echo "B * A = $result_ba"
        fail=$((fail + 1))
    fi
}
run_distributivity_case() {
    local name="$1"
    local field="$2"
    local a="$3"
    local b="$4"
    local c="$5"
    local b_xor_c
    local output_lhs
    local output_ab
    local output_ac
    local result_lhs
    local result_ab
    local result_ac
    local ab_value
    local ac_value
    local expected_rhs

    b_xor_c="$(printf '%X' "$((16#$b ^ 16#$c))")"

    output_lhs="$(printf '%s\n%s %s\n' "$field" "$a" "$b_xor_c" | "$BIN")"
    output_ab="$(printf '%s\n%s %s\n' "$field" "$a" "$b" | "$BIN")"
    output_ac="$(printf '%s\n%s %s\n' "$field" "$a" "$c" | "$BIN")"

    result_lhs="$(sed -n 's/^A \* B = //p' <<< "$output_lhs")"
    result_ab="$(sed -n 's/^A \* B = //p' <<< "$output_ab")"
    result_ac="$(sed -n 's/^A \* B = //p' <<< "$output_ac")"

    if [[ -z "$result_lhs" || -z "$result_ab" || -z "$result_ac" ]]; then
        echo "FAIL: $name"
        fail=$((fail + 1))
        return
    fi

    ab_value=$((16#${result_ab#0x}))
    ac_value=$((16#${result_ac#0x}))

    printf -v expected_rhs '0x%X' "$((ab_value ^ ac_value))"

    if [[ "$result_lhs" == "$expected_rhs" ]]; then
        echo "PASS: $name"
        pass=$((pass + 1))
    else
        echo "FAIL: $name"
        echo "a * (b XOR c) = $result_lhs"
        echo "(a * b) XOR (a * c) = $expected_rhs"
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
run_case "Lowercase hexadecimal input" 1 "a f" "0xC"

echo
echo "Testing GF(2^8)..."

run_case "Zero multiplication" 2 "0 57" "0x0"
run_case "Multiplication by one" 2 "1 AE" "0xAE"
run_case "Reduction modulo 0x11B" 2 "57 83" "0xC1"
run_case "Inverse relationship" 2 "53 CA" "0x1"
run_case "Maximum operands" 2 "FF FF" "0x13"

echo
echo "Testing commutativity..."

# GF(2^4)
run_commutativity_case "GF(2^4): zero and maximum" 1 0 F
run_commutativity_case "GF(2^4): one and maximum" 1 1 F
run_commutativity_case "GF(2^4): 3 and 7" 1 3 7
run_commutativity_case "GF(2^4): 5 and A" 1 5 A
run_commutativity_case "GF(2^4): maximum operands" 1 F F

# GF(2^8)
run_commutativity_case "GF(2^8): zero and maximum" 2 0 FF
run_commutativity_case "GF(2^8): one and maximum" 2 1 FF
run_commutativity_case "GF(2^8): 57 and 83" 2 57 83
run_commutativity_case "GF(2^8): 53 and CA" 2 53 CA
run_commutativity_case "GF(2^8): maximum operands" 2 FF FF

echo
echo "Testing distributivity..."

# GF(2^4)
run_distributivity_case "GF(2^4): 3, 5, A" 1 3 5 A
run_distributivity_case "GF(2^4): 7, 8, 4" 1 7 8 4
run_distributivity_case "GF(2^4): F, 3, C" 1 F 3 C
run_distributivity_case "GF(2^4): 1, A, 5" 1 1 A 5
run_distributivity_case "GF(2^4): zero and maximum" 1 0 F 7

# GF(2^8)
run_distributivity_case "GF(2^8): 57, 83, CA" 2 57 83 CA
run_distributivity_case "GF(2^8): 53, CA, FF" 2 53 CA FF
run_distributivity_case "GF(2^8): 1, A5, 5B" 2 1 A5 5B
run_distributivity_case "GF(2^8): FF, 80, 7F" 2 FF 80 7F
run_distributivity_case "GF(2^8): zero and maximum" 2 0 FF 57
echo
echo "Testing invalid input..."

run_error_case "Reject out-of-range GF(2^4) operand" \
    "1\n10 2\n" "Error"

run_error_case "Reject out-of-range GF(2^8) operand" \
    "2\n100 2\n" "Error"

run_error_case "Reject invalid field choice" \
    "3\n" "Error: invalid field choice."

run_error_case "Reject non-hexadecimal input" \
    "1\nG 2\n" "Error: invalid input."

run_error_case "Reject missing operand" \
    "1\n7\n" "Error: invalid input."
run_error_case "Reject negative operand" \
    "1\n-1 5\n" "Error: invalid input."

run_error_case "Reject excessively large hexadecimal input" \
    "2\nFFFFFFFFFFFFFFFFFFFFFFFF 2\n" "Error: invalid input."

run_error_case "Reject extra trailing operand" \
    "1\n7 D ABC\n" "Error: invalid input."

run_error_case "Reject invalid hexadecimal character" \
    "1\nG 2\n" "Error: invalid input."


echo
echo "=============================="
echo "Passed: $pass"
echo "Failed: $fail"
echo "Total:  $((pass + fail))"
echo "=============================="

if [[ "$fail" -ne 0 ]]; then
    exit 1
fi
