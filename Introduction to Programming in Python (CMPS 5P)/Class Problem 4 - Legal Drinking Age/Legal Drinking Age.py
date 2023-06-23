#1616143 Leon Shuster
known_age = input("Do you know your age? [yes/no]")
if known_age == "yes":
age = int(input("What is your age? "))
if 0 <= age <= 150:
    if age < 21:
        print("You are not allowed to drink alcohol ")
    else:
        print("You are allowed to drink alcohol ")
else:
    print("Age is not realistic ")
