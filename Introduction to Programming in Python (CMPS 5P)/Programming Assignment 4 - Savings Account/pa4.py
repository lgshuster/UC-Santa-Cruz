#1616143 Leon Shuster

A = float(input("What is your initial deposit? "))
r = float(input("What is the annual interest rate? "))

for a in range(1, 13):
    d = float(input("How much money do you want to deposit? "))
    w = float(input("How much money do you want to withdraw? "))
    if (((1 + (1/12) * (r/100)) * A) + d - w) < 0:
        print("Withdrawal is refused, not enough money")
        w = 0
    A = (((1 + (1/12) * (r/100)) * A) + d - w)
    print("Your new balance at the end of month", a, "is %0.2f" % A)
    print()
