import matplotlib.pyplot as plt
import subprocess 
import pandas as pd
import os
from polygenerator import (
    random_polygon,
)

c = list(range(20,400,50))
def show(dict):
    print("N : Decomp time,Merge Time,Total")
    for t in c:
        print(str(t)+" : "+str(dict[t][0])+","+str(dict[t][1])+","+str(dict[t][2]))
y1,y2 = [],[]
dict = {}
for i in c:
    print(i)
    polygon = random_polygon(num_points=i)
    print("done")
    with open("ArtificialInput.txt","w") as f:
        f.write(str(len(polygon))+ '\n')

        polygon.reverse()
        
        for coord in polygon:
            f.write(str(coord[0])+ ' ' +str(coord[1]) + '\n')
    os.system("./a.out 0")
    with open("times.txt") as f:
        a = float(f.readline())
        b = float(f.readline())
        dict[i] = [a,b,a+b]
        
        y1.append(a)
        y2.append(b)
df = pd.DataFrame(dict).T
df[0].plot(label="Decomp Time")
df[1].plot(label="Merge Time")
plt.figure(1)
plt.title("Decomposition Time & Merge Time")
plt.ylabel("Time(s)")
plt.xlabel("Number of vertices")
plt.savefig("Total_time.png")
plt.figure(2)
df[1].plot(color='orange',label="Merge Time")
plt.title("Merge Time")
plt.ylabel("Time(s)")
plt.savefig("Merge_time.png")
# plt.show()
show(dict)