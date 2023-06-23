#list of digits
ones = {0: "zero", 1: "one", 2: "two",
        3: "three", 4: "four", 5: "five",
        6: "six", 7: "seven", 8: "eight", 9: "nine"}
teens = {11: "eleven", 12: "twelve", 13: "thirteen",
         14: "fourteen", 15: "fifteen", 16: "sixteen",
         17: "seventeen", 18: "eighteen", 19: "nineteen"}
tens = {10: "ten", 20: "twenty", 30: "thirty",
        40: "forty", 50: "fifty", 60: "sixty",
        70: "seventy", 80: "eighty", 90: "ninety"}
hundreds = {100: "hundred", 1000: "thousand",
            1000000: "million"}

#functions for each digits place
def sayDigit(num):
    return ones.get(num)

def sayTeen(num):
    if num > 10:
        return teens.get(num)
    elif num < 10:
        return sayDigit(num)

def sayTens(num):
    if num == 10:
        return tens.get(num)
    return tens.get(num)

def sayHundreds(num):
    return hundreds.get(num)

#less than 3 digits
def lessThanThreeDigits(num):
    if num // 10 == 0:
        return sayDigit(num % 10)
    elif num % 10 == 0:
        return sayTens((num // 10) * 10)
    else:
        if num < 20:
            return sayTeen(num)
        if num >= 20:
            return sayTens((num // 10) * 10) + "-" + sayDigit(num % 10)

#less than 4 digits
def lessThanFourDigits(num):
    if num // 100 == 0:
        return lessThanThreeDigits(num % 100)
    elif num % 100 == 0:
        return sayDigit(num // 100) + " " + sayHundreds(100)
    else:
        return sayDigit(num // 100) + " " + sayHundreds(100) + " " + lessThanThreeDigits(num % 100)

#less than 7 digits
def lessThanSevenDigits(num):
    if num // 1000 == 0:
        return lessThanFourDigits(num % 1000)
    elif num % 1000 == 0:
        if (num // 1000) < 100:
            return lessThanThreeDigits(num // 1000) + " " + sayHundreds(1000)
        else:
            return lessThanFourDigits(num // 1000) + " " + sayHundreds(1000)
    else:
        if (num // 1000) < 100:
            return lessThanThreeDigits(num // 1000) + " " + sayHundreds(1000) + " " + lessThanFourDigits(num % 1000)
        else:
            return lessThanFourDigits(num // 1000) + " " + sayHundreds(1000) + " " + lessThanFourDigits(num % 1000)

#less than 10 digits
def lessThanTenDigits(num):
    if num % 1000000 == 0:
        if num // 1000000 < 100:
            return lessThanThreeDigits(num // 1000000) + " " + sayHundreds(1000000)
        else:
            return lessThanFourDigits(num // 1000000) + " " + sayHundreds(1000000)
    else:
        if num // 1000000 < 100:
            return lessThanThreeDigits(num // 1000000) + " " + sayHundreds(1000000) + " " + lessThanSevenDigits(num % 1000000)
        else:
            return lessThanFourDigits(num // 1000000) + " " + sayHundreds(1000000) + " " + lessThanSevenDigits(num % 1000000)

#asks for input
number = int(input("What is your number? "))

#quits if program is out of range
if number < 0 or number > 999999999:
    print("Invalid Number")
    exit()

#changing number to string and finding length
str_number = str(number)
length_number = len(str_number)

#calls functions depending on how long number is
if length_number < 3:
    print(lessThanThreeDigits(number))
elif length_number < 4:
    print(lessThanFourDigits(number))
elif length_number < 7:
    print(lessThanSevenDigits(number))
elif length_number < 10:
    print(lessThanTenDigits(number))
