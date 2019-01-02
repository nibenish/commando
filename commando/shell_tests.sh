#!/bin/bash

printf "Loading tests... "
source shell_tests_data.sh
printf "%d tests loaded\n" "$T"

NTESTS=$T
NPASS=0

all_tests=$(seq $NTESTS)

# Check whether a single test is being run
single_test=$1
if ((single_test > 0 && single_test <= NTESTS)); then
    printf "Running single TEST %d\n" "$single_test"
    all_tests=$single_test
    NTESTS=1
else
    printf "Running %d tests\n" "$NTESTS"
fi

# printf "tests: %s\n" "$all_tests"
printf "\n"

for i in $all_tests; do
    printf "TEST %2d %-18s : " "$i" "${tnames[i]}"
    
    # Run the test
    printf "%s\n" "${input[i]}" | ./commando --echo >& actual.txt
    ./standardize actual.txt > actual.std
    printf "%s\n" "${output[i]}" > expect.std
    printf "%s\n" "${input[i]}" | valgrind ./commando --echo |& cat > valgrind.txt

    # Check for failure, print side-by-side diff if problems
    if ! cmp -s expect.std actual.std;
    then
        printf "FAIL: Output Incorrect\n"
        printf "================================\n"
        printf "INPUT:\n%s\n" "${input[i]}"
        printf "OUTPUT: EXPECT   vs   ACTUAL\n"
        diff -y expect.std actual.std
        # sdiff expect.txt actual.txt
        printf "================================\n"

    # Check various outputs from valgrind
    elif ! grep -q 'ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)' valgrind.txt ||
         ! grep -q 'in use at exit: 0 bytes in 0 blocks'  valgrind.txt ||
           grep -q 'definitely lost: 0 bytes in 0 blocks' valgrind.txt;
    then
        printf "FAIL: Valgrind detected problems\n"
        printf "================================\n"
        cat valgrind.txt
        printf "================================\n"
    else
        printf "OK\n"
        ((NPASS++))
    fi
done
printf "================================\n"
printf "Finished: %d / %d passed\n" "$NPASS" "$NTESTS"

