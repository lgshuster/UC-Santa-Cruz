import math

p = [2, 1]
q = [5, 5]

def dist(x1, y1, x2, y2):
    x = (x2 - x1)**2
    y = (y2 - y1)**2
    total_dist = math.sqrt(x+y)
    return total_dist



