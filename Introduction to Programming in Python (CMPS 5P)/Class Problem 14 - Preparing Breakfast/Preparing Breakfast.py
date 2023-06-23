#1616143 Leon Shuster

def fryEggs(numEggs):
    print("You wanted", numEggs, "eggs")

def toastBread(slices):
    print("You wanted", slices, "slices of bread")

def brewCoffee(cups):
    print("You wanted", cups, "cups of coffee")

def prepareBreakfast():
    eggs = int(input("How many eggs would you like? "))
    fryEggs(eggs)
    bread = int(input("How many slices of bread would you like? "))
    toastBread(bread)
    coffee = int(input("How many cups of coffee would you like? "))
    brewCoffee(coffee)
    print("Enjoy your breakfast! ")

prepareBreakfast()
