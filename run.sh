set -e
g++ *.cpp
./a.out
# python3 plot.py outputMerge.txt
# python3 plot.py outputNoMerge.txt