#1616143 Leon Shuster

#function for determining if BMI is healthy
def isHealthy(a):
    if a < 18:
        return "You are underweight. Eat more or grow less! "
    elif a > 30:
        return "You are overweight. Eat less or grow more! "
    else:
        return "You are healthy. Good job! "

#asking user if BMI is known
known_BMI = input("Do you know your BMI (Body-Mass-Index)? [yes/no] ")

#if user answers yes
if known_BMI == "yes":
    BMI_input = int(input("What is your BMI? "))
    print("Your BMI is " + str(BMI_input) + ". " + isHealthy(BMI_input))

#if user answers no
if known_BMI == "no":
    weight_lbs = int(input("What is your weight in pounds? "))
    weight_kgs = weight_lbs * (1/2.2)

    height_inches = int(input("What is your height in inches? "))
    height_meters = height_inches * (0.0254)

    BMI = (weight_kgs)/((height_meters)**2)
    print("Your BMI is " + str(BMI) + ". " + isHealthy(BMI))
