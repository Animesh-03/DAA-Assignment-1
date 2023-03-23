import sys
import matplotlib 
import os 

c = list(10*x for x in range(1,20))
dict = {}
for i in c:
    os.system("python3 generate.py "+str(i))
    os.system("./a.out 0")
    with open("times.txt") as f:
        a = float(f.readline())
        b = float(f.readline())
        dict[i] = {a,b,a+b}
print(dict)