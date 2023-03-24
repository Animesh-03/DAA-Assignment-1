import matplotlib.pyplot as plt
import random
import sys
from polygenerator import (
    random_polygon,
    random_star_shaped_polygon,
    random_convex_polygon,
)
    
    
def plot_polygon(polygon, out_file_name):
    plt.figure()
    plt.gca().set_aspect("equal")
    
    polygon.append(polygon[0])
    
    xs, ys = zip(*polygon)
    plt.plot(xs, ys, "r-", linewidth=0.4)
    
    plt.savefig(out_file_name, dpi=300)
    plt.close()
    
def plot_polygon1(polygon, out_file_name):
    plt.figure()
    plt.gca().set_aspect("equal")
    
    polygon.append(polygon[0])
    
    xs, ys = zip(*polygon)
    plt.plot(xs, ys, "r-", linewidth=0.4)
    plt.savefig(out_file_name, dpi=300)
    plt.close()
    
    
polygon = random_polygon(num_points=int(sys.argv[1]))
    
with open("ArtificialInput.txt","w") as f:
    f.write(str(len(polygon))+ '\n')

    polygon.reverse()
    
    for coord in polygon:
        f.write(str(coord[0])+ ' ' +str(coord[1]) + '\n')
plot_polygon1(polygon, "random_polygon.png")