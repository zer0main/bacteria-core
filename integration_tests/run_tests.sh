#!/bin/bash

set -xue

integration_test=$1

input_files=$(ls tests/*.yaml | grep -v '.expected.yaml$')

for f in $input_files; do
    name=$(basename "$f" .yaml)
    scripts=$(ls "tests/${name}.bact")
    observed="tests/${name}.observed.yaml"
    valgrind $integration_test "$f" "$scripts" "$observed"
done

observed_files=$(ls tests/*.observed.yaml)

for f in $observed_files; do
    name=$(basename "$f" .observed.yaml)
    expected="tests/${name}.expected.yaml"
    ./check_yaml.py --expected "$expected" --observed "$f"
done
