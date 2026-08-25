#!/usr/bin/env bash

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
WHITE='\033[1;37m'
YELLOW='\033[0;33m'
NC='\033[0m'

BIN="./codexion"
TIMEOUT=10
PASS_COUNT=0
FAIL_COUNT=0

if [[ ! -x "$BIN" ]]; then
    echo -e "${RED}Error: Binary '$BIN' not found or not executable.${NC}"
    exit 1
fi

evaluate_concurrency() {
    local test_name="$1"
    local expected="$2"
    shift 2

    local num_coders="$1"
    local time_to_compile="$3"
    local raw_output
    local starts=()
    local regex='^[[:space:]]*([0-9]+)[[:space:]]+[0-9]+.*is compiling'

    echo -e "${WHITE}$ $BIN $*${NC}"

    # Capture execution output under timeout
    raw_output=$(timeout "$TIMEOUT" "$BIN" "$@" 2>&1)
    local exit_code=$?

    if (( exit_code == 124 )); then
        echo -e "${RED}[KO]${NC} $test_name (Timed out after ${TIMEOUT}s)"
        ((FAIL_COUNT++))
        return
    fi

    # Parse timestamps using native Bash regex
    while IFS= read -r line; do
        if [[ "$line" =~ $regex ]]; then
            starts+=("${BASH_REMATCH[1]}")
        fi
    done <<< "$raw_output"

    local total_events=${#starts[@]}
    local max_concurrent=0

    # Sliding-window concurrency calculation
    for ((i = 0; i < total_events; i++)); do
        local current_window=0
        local window_start="${starts[i]}"
        local window_limit=$((window_start + time_to_compile))

        for ((j = i; j < total_events; j++)); do
            if (( starts[j] < window_limit )); then
                ((current_window++))
            else
                break
            fi
        done

        if (( current_window > max_concurrent )); then
            max_concurrent=$current_window
        fi
    done

    # Validation
    if (( max_concurrent >= expected )); then
        echo -e "${GREEN}[OK]${NC} $test_name (Peak Concurrency: $max_concurrent / Expected: $expected)"
        ((PASS_COUNT++))
    else
        echo -e "${RED}[KO]${NC} $test_name (Peak Concurrency: $max_concurrent / Expected: $expected)"
        ((FAIL_COUNT++))
    fi
}

echo -e "\n${YELLOW}=== CODEXION CONCURRENCY & PARALLELISM SUITE ===${NC}"

# Test 1: Even Baseline (6 coders -> Max concurrent: 3)
evaluate_concurrency "conc_even_baseline" 3 6 3000 200 10 10 3 10 fifo

# Test 2: Odd Offset (7 coders -> Max concurrent: 3)
evaluate_concurrency "conc_odd_offset" 3 7 3000 200 10 10 3 10 fifo

# Test 3: Massive Scale (20 coders -> Max concurrent: 10)
evaluate_concurrency "conc_massive_scale" 10 20 5000 150 10 10 2 10 fifo

# Test 4: Zero Cooldown Burst (8 coders -> Max concurrent: 4)
evaluate_concurrency "conc_zero_cooldown_burst" 4 8 4000 200 10 10 3 0 fifo

# Test 5: EDF Priority Contention (5 coders -> Max concurrent: 2)
evaluate_concurrency "conc_edf_starvation_stress" 2 5 3000 200 10 10 4 10 edf

# Test 6: EDF Priority Contention (30 coders -> Max concurrent: 15)
evaluate_concurrency "conc_edf_starvation_stress" 15 30 3000 20 10 10 4 10 edf

# Test 6: EDF Priority Contention 200 coders -> Max concurrent: 100)
evaluate_concurrency "conc_edf_starvation_stress" 100 200 300 20 10 10 4 10 edf

echo "----------------------------------------------------"
echo -e "${GREEN}$PASS_COUNT passed${NC}, ${RED}$FAIL_COUNT failed${NC}"

if (( FAIL_COUNT > 0 )); then
    exit 1
fi
exit 0