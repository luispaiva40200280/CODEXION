#!/bin/bash
###############################################################################
# run_tests.sh — strict validation suite for ./codexion
#
# Categories (25 tests total):
#   1. Invalid Arguments        (5 tests) -> program must exit non-zero
#   2. Valid Args & Simulation  (5 tests) -> program must exit 0
#   3. Memory Leaks             (10 tests) -> valgrind --leak-check=full
#   4. Data Races / Deadlocks   (5 tests) -> valgrind --tool=helgrind
#
# Only "[OK] <name>" (green) or "[KO] <name>" (red) is ever printed per test.
###############################################################################

BIN="./codexion"
SUPP="./helgrind.supp"

# Generous per-test wall-clock ceilings so a real deadlock/hang can never
# freeze the whole suite. Native runs are cheap; valgrind/helgrind runs are
# 10-50x slower than native, hence the larger ceilings for those tiers.
NATIVE_TIMEOUT=10
VALGRIND_TIMEOUT=400
HELGRIND_TIMEOUT=600

# A sentinel exit code used ONLY to detect whether valgrind/helgrind itself
# flagged an error (memory error / data race). This is distinct from the
# exit code of the program being wrapped, which is why we don't rely on the
# plain $? of the wrapped invocation for these tiers.
VG_ERR_CODE=99

# Persistent output directory for valgrind/helgrind logs (kept after the run
# so failing tests can be inspected). Cleared and recreated on each run.
TMP_DIR="TEST_OUTPUT"
rm -rf "$TMP_DIR"
mkdir -p "$TMP_DIR"

GREEN="\033[0;32m"
RED="\033[0;31m"
WHITE="\033[1:37M"
NC="\033[0m"

PASS_COUNT=0
FAIL_COUNT=0

# Prints the exact command about to be executed, dimmed, on its own line.
show_cmd() {
    echo -e "${WHITE}\$ $*${NC}"
}

ok() {
    echo -e "${GREEN}[OK]${NC} $1"
    PASS_COUNT=$((PASS_COUNT + 1))
}

ko() {
    echo -e "${RED}[KO]${NC} $1"
    FAIL_COUNT=$((FAIL_COUNT + 1))
}

if [ ! -x "$BIN" ]; then
    ko "codexion binary not found or not executable at $BIN"
    exit 1
fi

###############################################################################
# Category 1 — Invalid Arguments (expect non-zero exit code)
###############################################################################

test_invalid() {
    local name="$1"
    shift
    show_cmd timeout "$NATIVE_TIMEOUT" "$BIN" "$@"
    timeout "$NATIVE_TIMEOUT" "$BIN" "$@" >/dev/null 2>&1
    local rc=$?
    if [ "$rc" -ne 0 ]; then
        ok "$name"
    else
        ko "$name"
    fi
}

test_invalid "invalid_missing_arguments"        3 3000 100 100
test_invalid "invalid_negative_number"          -1 3000 100 100 100 3 50 fifo
test_invalid "invalid_non_integer_string"       3 3000 abc 100 100 3 50 fifo
test_invalid "invalid_zero_coders"              0 3000 100 100 100 3 50 fifo
test_invalid "invalid_wrong_scheduler_name"     3 3000 100 100 100 3 50 roundrobin

###############################################################################
# Category 2 — Valid Arguments & Simulation (expect exit code 0)
###############################################################################

test_valid() {
    local name="$1"
    shift
    show_cmd timeout "$NATIVE_TIMEOUT" "$BIN" "$@"
    timeout "$NATIVE_TIMEOUT" "$BIN" "$@" >/dev/null 2>&1
    local rc=$?
    if [ "$rc" -eq 0 ]; then
        ok "$name"
    else
        ko "$name"
    fi
}

test_valid "valid_standard_completion_fifo"     3 3000 100 100 100 3 50 fifo
test_valid "valid_standard_completion_edf"      4 3000 150 100 100 3 50 edf
test_valid "valid_single_coder_completion"      1 3000 100 100 100 3 50 fifo
test_valid "valid_forced_burnout_single_coder"  1 300 100 100 100 5 50 edf
test_valid "valid_moderate_multi_coder_run"     5 5000 200 150 150 2 100 fifo

###############################################################################
# Category 3 — Memory Leaks (valgrind --leak-check=full)
###############################################################################

test_valgrind_leak() {
    local name="$1"
    shift
    local logfile="$TMP_DIR/valgrind_${name}.log"

    show_cmd timeout "$VALGRIND_TIMEOUT" valgrind \
        --leak-check=full --show-leak-kinds=all \
        --error-exitcode="$VG_ERR_CODE" --log-file="$logfile" \
        "$BIN" "$@"

    timeout "$VALGRIND_TIMEOUT" valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --error-exitcode="$VG_ERR_CODE" \
        --log-file="$logfile" \
        "$BIN" "$@" >/dev/null 2>&1
    local rc=$?

    if [ "$rc" -eq 0 ] && grep -q "All heap blocks were freed -- no leaks are possible" "$logfile" 2>/dev/null; then
        ok "$name"
    else
        ko "$name"
    fi
}

test_valgrind_leak "mem_valid_small_fifo"           2 3000 100 100 100 2 50 fifo
test_valgrind_leak "mem_valid_small_edf"            3 3000 100 100 100 2 50 edf
test_valgrind_leak "mem_valid_single_coder"         1 3000 100 100 100 2 50 fifo
test_valgrind_leak "mem_forced_burnout_early_exit"  1 300 100 100 100 5 50 fifo
test_valgrind_leak "mem_heavy_starvation_multi"     5 200 150 150 150 5 100 edf
test_valgrind_leak "mem_invalid_missing_args"       3 3000 100 100
test_valgrind_leak "mem_invalid_negative_number"    -2 3000 100 100 100 3 50 fifo
test_valgrind_leak "mem_invalid_wrong_scheduler"    3 3000 100 100 100 3 50 bogus
test_valgrind_leak "mem_invalid_zero_coders"        0 3000 100 100 100 3 50 edf
test_valgrind_leak "mem_valid_larger_run_edf"       4 4000 100 100 100 3 50 edf

###############################################################################
# Category 4 — Data Races & Deadlocks (valgrind --tool=helgrind)
###############################################################################

test_helgrind() {
    local name="$1"
    shift
    local logfile="$TMP_DIR/helgrind_${name}.log"

    show_cmd timeout "$HELGRIND_TIMEOUT" valgrind \
        --tool=helgrind --suppressions="$SUPP" \
        --error-exitcode="$VG_ERR_CODE" --log-file="$logfile" \
        "$BIN" "$@"

    timeout "$HELGRIND_TIMEOUT" valgrind \
        --tool=helgrind \
        --suppressions="$SUPP" \
        --error-exitcode="$VG_ERR_CODE" \
        --log-file="$logfile" \
        "$BIN" "$@" >/dev/null 2>&1

    if grep -q "ERROR SUMMARY: 0 errors from 0 contexts" "$logfile" 2>/dev/null; then
        ok "$name"
    else
        ko "$name"
    fi
}

test_helgrind "race_standard_three_coders_fifo"   3 3000 100 100 100 2 50 fifo
test_helgrind "race_standard_four_coders_edf"     4 3000 150 100 100 2 50 edf
test_helgrind "race_high_contention_fifo"         5 3000 100 100 100 2 300 fifo
test_helgrind "race_high_contention_edf"          6 4000 150 100 100 2 200 edf
test_helgrind "race_single_coder_edge_case"       1 3000 100 100 100 2 50 edf

###############################################################################
# Summary
###############################################################################

TOTAL=$((PASS_COUNT + FAIL_COUNT))
echo "----------------------------------------"
echo -e "${GREEN}${PASS_COUNT} passed${NC}, ${RED}${FAIL_COUNT} failed${NC} (of ${TOTAL} tests)"

if [ "$FAIL_COUNT" -ne 0 ]; then
    exit 1
fi
exit 0