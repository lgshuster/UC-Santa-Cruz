#1616143 Leon Shuster
Distance = int(input("What is the total distance of the trip (in miles)? "))
FuelEfficiency = int(input("How many miles per gallon? "))

TripCost = (Distance/FuelEfficiency) * 2.59
print("Your trip will cost you $" + str(TripCost))
