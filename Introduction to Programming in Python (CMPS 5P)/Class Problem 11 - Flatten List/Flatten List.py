#1616143 Leon Shuster
s = [[1, 4, 1], [3], [2, 3], []]
res = []

for i in s:
    for j in i:
        res.append(j)

res.sort()
print(res)
