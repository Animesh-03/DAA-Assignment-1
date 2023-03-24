# -*- coding: utf-8 -*-
"""Polygon_Decomp
"""

from time import sleep
from shapely.geometry import Polygon
import matplotlib.pyplot as plt
import sys

DELAY = float(sys.argv[2])

flag = int(sys.argv[1])
file_name = "outputNoMerge.txt" if flag == 0 else "outputMerge.txt"
file = open(file_name,"r")
lines = file.readlines()
lines.append("12")

temp = []
curr = []
for a in lines:
    a = [x for x in a.strip().split(' ')]
    if(len(a) == 1):
        if(len(curr)>0):
            curr.append(curr[0])
            temp.append(curr)
        curr = []
    else:
        curr.append([float(a[0]),float(a[1])])

convex_polygons = temp
fig, ax = plt.subplots()
print(temp)
def getCentroid(a):
    sumx,sumy = 0,0
    n = len(a)
    for i in range(0,n):
        sumx += a[i][0]
        sumy += a[i][1]
    sumx/=n
    sumy/=n
    return (sumx,sumy)

for i, polygon in enumerate(convex_polygons):
    x_coords = [v[0] for v in polygon]
    y_coords = [v[1] for v in polygon]
    # if(i == 1):
    #     continue
    ax.fill(x_coords, y_coords, alpha=0.4, label=f'Face {i}')
    ax.plot(x_coords, y_coords, color='navy')
    label=f'Face {i}'
    ax.annotate(label,xy=getCentroid(convex_polygons[i]))
    plt.gca().set_aspect("auto")
    plt.draw()
    plt.pause(0.01)
    sleep(DELAY)



ax.set_title('Convex Polygon Decomposition')
ax.legend()
plt.show()
