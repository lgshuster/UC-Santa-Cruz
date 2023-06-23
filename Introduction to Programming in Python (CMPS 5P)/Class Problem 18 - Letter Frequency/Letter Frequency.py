frequency = {}
for char in input():
    if char not in frequency:
        frequency[char] = 1
    else:
        frequency[char] += 1
print(frequency)
