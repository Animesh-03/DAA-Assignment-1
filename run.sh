set -e
g++ *.cpp
./a.out $1
#python3 plot.py outputMerge.txt
#python3 plot.py outputNoMerge.txt