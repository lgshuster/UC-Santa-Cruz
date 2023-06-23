#1616143 Leon Shuster

def Sum(n):
    total = 0
    for num in range(n):
        total += Sum(n-1) + n
    return total

upper_limit = int(input("What is your upper limit? "))
print(Sum(upper_limit))
