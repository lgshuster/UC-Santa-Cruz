#1616143 Leon Shuster

#function for incrementing
def inc(num):
    return num + 1

#function for decrementing
def dec(num):
    return num - 1

#function for adding
def add(num1, num2):
    if num2 == 0:
        return num1
    else:
        return add(inc(num1), dec(num2))

#function for multiplying
def mul(num1, num2):
    if num1 == 0 or num2 == 0:
        return 0
    elif num2 == 1:
        return num1
    else:
        return add(num1, mul(num1, dec(num2)))

#asking user if they want to add or multiply
operation = input("Do you want to add or multiply numbers? [add/mul] ")

#adds or multiplies based on input, prints error if input is invalid
if operation == "add":
    num1 = int(input("What is your first number? "))
    num2 = int(input("What is your second number "))
    print(num1, "+", num2, "=", add(num1, num2))
elif operation == "mul":
    num1 = int(input("What is your first number? "))
    num2 = int(input("What is your second number "))
    print(num1, "*", num2, "=", mul(num1, num2))
else:
    print("Invalid Input")
