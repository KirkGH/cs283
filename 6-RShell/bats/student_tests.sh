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

@test "Test: check echo and grep" {
    run ./dsh <<EOF
echo "hello world" | grep "hello"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" = *"hello world"* ]]
}

@test "Test: check multiple piped commands" {
    run ./dsh <<EOF
ls | wc -l
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check three-command pipeline" {
    run ./dsh <<EOF
ls | grep . | wc -l
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check ls | grep test" {
    run ./dsh <<EOF
ls | grep test
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check whitespaces" {
    run ./dsh <<EOF
ls          |       grep test
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check cd with ls" {
    run ./dsh <<EOF
cd .. | ls
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check client exit" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check client cd" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
cd bats
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check client ls" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check client dragon" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
dragon
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check client multiple pipes" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
ls | wc -l
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check client echo" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
echo "hello world"
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check client whitespaces" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
ls    |    wc    -l
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check client three pipe commands" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
ls | grep . | wc -l
EOF
    [ "$status" -eq 0 ]
}

@test "Test: check server exit" {
    ./dsh -s -i 0.0.0.0 -p 7890
    run ./dsh -c -i 129.25.203.107 -p 7890 <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}