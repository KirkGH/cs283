#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suite in this file

@test "Test: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check exit command terminates shell" {
    run ./dsh <<EOF                
exit
EOF
    [ "$status" -ne 0 ]
}

@test "Test: check cd works" {
    startDir=$(pwd)
    mkdir -p tmp/valid-dir
    run ./dsh <<EOF
cd tmp/valid-dir
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" = *"/tmp/valid-dir"* ]]
}

@test "Test: check quote with space in between" {
    run ./dsh <<EOF
echo "hello world"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" = *"hello world"* ]]
}

@test "Test: check ls command" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
    [[ "$output" = *"$(ls)"* ]]
}

@test "Test: check empty input gives warning" {
    run ./dsh <<EOF
     
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check dragon command output" {
    run ./dsh <<EOF
dragon
EOF
    [ "$status" -eq 0 ]
}
