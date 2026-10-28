#!/bin/bash

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
mapfile -t hashes < "$script_dir/../hashes.txt"

width=128
height=128
k=4

for hash in "${hashes[@]}" ;
do
    "$script_dir/../structure-in-noise" -x "$1" -y "$2" -w "$width" -h "$height" -k "$k" "$hash"
done

"$script_dir/../structure-in-noise" -x "$1" -y "$2" -w "$width" -h "$height" -k "$k" -f /dev/random
