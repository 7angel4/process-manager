SOME TEST CASES:

./allocate -f myTests/empty.txt -s SJF -m infinite -q 1

./allocate -f myTests/gap.txt -s SJF -m infinite -q 3

./allocate -f myTests/gap.txt -s RR -m infinite -q 3

./allocate -f myTests/gap.txt -s SJF -m best-fit -q 3

./allocate -f myTests/gap.txt -s RR -m best-fit -q 2

./allocate -s SJF -q 3 -m infinite -f myTests/ed.txt

./allocate -s SJF -q 3 -m best-fit -f myTests/ed.txt

./allocate -s SJF -q 3 -m best-fit -f myTests/alt.txt

./allocate -s RR -q 3 -m best-fit -f myTests/alt.txt

./allocate -s RR -q 3 -m best-fit -f myTests/same.txt

./allocate -s SJF -q 2 -m best-fit -f myTests/big.txt

./allocate -s RR -q 3 -m best-fit -f myTests/big.txt

./allocate -s RR -q 2 -m best-fit -f myTests/same_arrival_time.txt

./allocate -s SJF -q 2 -m infinite -f myTests/same_arrival_time.txt

./allocate -s SJF -q 2 -m best-fit -f myTests/same_arrival_time.txt

./allocate -q 1 -s RR -m best-fit -f myTests/wc.txt
