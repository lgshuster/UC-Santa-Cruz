for num in range(1, 1001):
    if num % 4 == 0 and num % 9 == 0:
        print("DEADBEEF")
    elif num % 4 == 0:
        print("DEAD")
    elif num % 9 == 0:
        print("BEEF")
    else:
        print(num)
