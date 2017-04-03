# runs the simulation and then deletes it
make
./swap -f input.txt -a first -m 1000 -q 5
echo
echo "Deleting program ..."
make clobber
