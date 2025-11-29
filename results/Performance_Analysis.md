g++ -g -O0 *.cc -o a.out
valgrind --tool=callgrind ./a.out
kcachegrind callgrind.out.*