# -*- coding: utf-8 -*-
"""Polygon_Decomp
"""

from time import sleep
from shapely.geometry import Polygon
import matplotlib.pyplot as plt
import sys

cmdargs = sys.argv
print(cmdargs[1])

file = open(cmdargs[1],"r")
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

# polygon = [[15, 16], [13, 8], [9, 9], [4, 9],[0,8],[-5,5], [0,2], [2,-2], [6,-4], [10,-5], [15,-3], [17,0], [15,2], [10,2],[7,3], [10,5]]
convex_polygons = temp
print(temp)
fig, ax = plt.subplots()

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
    
    
    ax.fill(x_coords, y_coords, alpha=0.4, label=f'Face {i}')
    ax.plot(x_coords, y_coords, color='navy')
    label=f'Face {i}'
    ax.annotate(label,xy=getCentroid(convex_polygons[i]))
    plt.gca().set_aspect("auto")
    # plt.text(x_coords, y_coords, "rectangle", fontsize = 4)
    plt.draw()
    #plt.annotate("Rectangle", (x_coords, y_coords), color='black', weight='bold', fontsize=10, ha='center', va='center')

    # label=f'Face {i}'
    # plt.annotate(label, # this is the text
    #              (x_coords,y_coords), # these are the coordinates to position the label
    #              textcoords="", # how to position the text
    #              xytext=(0,0.1), # distance from text to points (x,y)
    #              ha='center')
    plt.pause(0.01)
    sleep(0.1)



ax.set_title('Convex Polygon Decomposition')
ax.legend()
plt.show()
