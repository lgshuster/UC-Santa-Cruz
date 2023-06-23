#1616143 Leon Shuster

def bound100(num):
    if num > 100:
        return 100
    elif num < 1:
        return 1
    else:
        return num

print(bound100(-5))
print(bound100(50))
print(bound100(101))
