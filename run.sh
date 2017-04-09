# runs the simulation and then deletes it
make
./swap -f testFirst1.txt -a first -m 1000 -q 7 > myoutFirst1.txt
./swap -f input.txt -a first -m 1000 -q 7 > myout.txt
./swap -f testWorst1.txt -a worst -m 1000 -q 7 > myoutWorst1.txt
./swap -f testBest1.txt -a best -m 1000 -q 7 > myoutBest1.txt
echo "Running diff first 1:"
diff myoutFirst1.txt outTestFirst1.txt
echo "Running diff first default:"
diff myout.txt output.txt
echo "Running diff worst 1:"
diff myoutWorst1.txt outTestWorst1.txt
echo "Running diff best 1:"
diff myoutBest1.txt outTestBest1.txt
make clobber
