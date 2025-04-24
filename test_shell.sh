#!/bin/bash

# Color codes
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"

# Path to your compiled shell executable
SHELL_EXEC=../holbertonschool-simple_shell/hsh

# Check if your shell binary exists
if [ ! -x "$SHELL_EXEC" ]; then
    echo -e "${RED}Error: $SHELL_EXEC not found or not executable${NC}"
    exit 1
fi

# Create a unique clean test environment in /tmp
TEST_DIR=$(mktemp -d /tmp/shell_test_XXXXXX)
HSH_PATH="$TEST_DIR/hsh" # Store absolute path to shell executable
cp "$SHELL_EXEC" "$HSH_PATH"
cd "$TEST_DIR" || exit 1

#############################################
# Task 2: /bin/ls non-interactive
#############################################
echo "/bin/ls" | ./hsh > .shell_output.txt
/bin/ls > .expected_output.txt

if diff .shell_output.txt .expected_output.txt > /dev/null; then
    echo -e "${GREEN}Task 2: PASS${NC}"
else
    echo -e "${RED}Task 2: FAIL${NC}"
    diff .shell_output.txt .expected_output.txt
fi
#############################################
# Task 3: /bin/echo hello world
#############################################
echo "/bin/echo hello world" | ./hsh > shell_output.txt
/bin/echo hello world > .expected_output.txt

if diff shell_output.txt .expected_output.txt > /dev/null; then
    echo -e "${GREEN}Task 3: PASS${NC}"
else
    echo -e "${RED}Task 3: FAIL${NC}"
    diff shell_output.txt .expected_output.txt
fi

#############################################
# Task 4: PATH-resolved ls
#############################################

# Debug information
echo "==== DEBUG INFO FOR TASK 4 ===="
echo "Current directory:"
pwd
echo "Directory contents:"
ls -la
echo "PATH environment variable:"
echo $PATH
echo "Creating test files for ls command:"
touch test_file1 test_file2
echo "Directory contents after creating test files:"
ls -la

# Verify which ls is being used by the system
echo "Location of ls command:"
which ls

# Test command parsing with echo
echo "Testing command parsing:"
echo "echo COMMAND_PARSING_TEST" | ./hsh > echo_test.txt 2>echo_test_stderr.txt
echo "Echo test output:"
cat echo_test.txt
echo "Echo test stderr:"
cat echo_test_stderr.txt

# Explicit test of PATH resolution with full debug
echo "Testing PATH resolution directly:"
echo "Testing PATH: /bin/ls"
echo "/bin/ls" | ./hsh > direct_test.txt 2>direct_stderr.txt
echo "Direct path output:"
cat direct_test.txt
echo "Direct path stderr:"
cat direct_stderr.txt

# Compare system and shell behavior
echo "Comparing system and shell behavior for ls:"
echo "Command: which ls"
which ls
echo "Command: ls -la /bin/ls"
ls -la /bin/ls
echo "==== END DEBUG INFO ===="

# Create a completely separate testing approach for Task 4
echo "Creating isolated test environment for Task 4..."

# Create test directories with absolute paths
TASK4_TEST_DIR="$TEST_DIR/test_files_dir"
TASK4_RESULTS_DIR="$TEST_DIR/results_dir"
mkdir -p "$TASK4_TEST_DIR"
mkdir -p "$TASK4_RESULTS_DIR"

# Copy the shell executable to the test files directory
cp "$HSH_PATH" "$TASK4_TEST_DIR/hsh"

# Create test files in the test directory
touch "$TASK4_TEST_DIR/file1" "$TASK4_TEST_DIR/file2" "$TASK4_TEST_DIR/file3"

# Force filesystem synchronization
sync
sleep 1

# List directory to verify initial state
echo "Files in test directory:"
/bin/ls -a "$TASK4_TEST_DIR"

# Change to the test directory just for running the commands
cd "$TASK4_TEST_DIR"

# Run shell's ls command, writing output to the results directory
echo "ls" | ./hsh > "$TASK4_RESULTS_DIR/shell_output.txt" 2>/dev/null

# Run system's ls command, writing output to the results directory
/bin/ls > "$TASK4_RESULTS_DIR/expected_output.txt"

# Return to main test directory
cd "$TEST_DIR"

# Show contents for debugging
echo "Clean directory shell_output.txt content:"
cat "$TASK4_RESULTS_DIR/shell_output.txt"
echo "Clean directory expected_output.txt content:"
cat "$TASK4_RESULTS_DIR/expected_output.txt"

# Compare outputs directly from the results directory
if diff "$TASK4_RESULTS_DIR/shell_output.txt" "$TASK4_RESULTS_DIR/expected_output.txt" > /dev/null; then
    echo -e "${GREEN}Task 4: PASS${NC}"
else
    echo -e "${RED}Task 4: FAIL${NC}"
    diff "$TASK4_RESULTS_DIR/shell_output.txt" "$TASK4_RESULTS_DIR/expected_output.txt"
fi

# Make sure we're in the main test directory for all remaining tests
cd "$TEST_DIR"

#############################################
# Task 5: Built-in exit
#############################################
echo "exit" | "$HSH_PATH"
status=$?

if [ $status -eq 0 ]; then
    echo -e "${GREEN}Task 5: PASS${NC}"
else
    echo -e "${RED}Task 5: FAIL (exit status: $status)${NC}"
fi

# Make sure we're in the main test directory
cd "$TEST_DIR"

#############################################
# Task 6: Built-in env
#############################################
echo "env" | "$HSH_PATH" > shell_output.txt
env > expected_output.txt

grep -v "^_=" shell_output.txt > tmp_shell.txt
grep -v "^_=" expected_output.txt > tmp_expected.txt

if diff tmp_shell.txt tmp_expected.txt > /dev/null; then
    echo -e "${GREEN}Task 6: PASS${NC}"
else
    echo -e "${RED}Task 6: FAIL${NC}"
    diff tmp_shell.txt tmp_expected.txt
fi

# Make sure we're in the main test directory
cd "$TEST_DIR"

#############################################
# Task 7: Handle EOF (Ctrl+D)
#############################################
"$HSH_PATH" < /dev/null > shell_output.txt
status=$?

if [ $status -eq 0 ]; then
    echo -e "${GREEN}Task 7: PASS${NC}"
else
    echo -e "${RED}Task 7: FAIL (exit status: $status)${NC}"
fi

# Make sure we're in the main test directory
cd "$TEST_DIR"

#############################################
# Task 8: Invalid command error
#############################################
echo "badcommand" | "$HSH_PATH" > shell_output.txt 2>&1

if grep -q "not found" shell_output.txt; then
    echo -e "${GREEN}Task 8: PASS${NC}"
else
    echo -e "${RED}Task 8: FAIL (no error for invalid command)${NC}"
    cat shell_output.txt
fi

#############################################
# Cleanup
#############################################
cd /
rm -rf "$TEST_DIR"

