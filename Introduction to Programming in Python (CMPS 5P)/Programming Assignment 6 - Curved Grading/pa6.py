#1616143 Leon Shuster
import math

#getting inputs of grades and putting them into a list
num_grades = input("What are the exam grades? ")
num_grades = num_grades.split(",")

#changing strings in lists into integers
for num in range(len(num_grades)):
    num_grades[num] = int(num_grades[num])

#calculating mean
mean = math.fsum(num_grades) / len(num_grades)
print("Mean: %0.2f" % mean)

#calculating standard deviation
total = 0
for num in num_grades:
    total += (num - mean)**2

Std_Dev = math.sqrt(total / len(num_grades))
print("Standard Deviation: %0.2f" % Std_Dev)

#function for determining letter grade, adds to a new list
letter_grades = []
def getLetterGrade(grade):
    if grade > (mean + 1.5*Std_Dev):
        letter_grades.append("A")
    elif grade > (mean + 0.5*Std_Dev):
        letter_grades.append("B")
    elif grade > (mean - 0.5*Std_Dev):
        letter_grades.append("C")
    elif grade > (mean - 1.5*Std_Dev):
        letter_grades.append("D")
    else:
        letter_grades.append("F")

#loops through num_grades and assigns letter using function
for num in num_grades:
    getLetterGrade(num)

#changing list into strings and printing
final_grades = ",".join(map(str, letter_grades))
print("Grades:", final_grades)
