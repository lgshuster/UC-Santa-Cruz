#1616143 Leon Shuster
import random

num_experiments = int(input("How many experiments did you perform? "))

y = 0
for a in range(num_experiments):
    y += random.randrange(1, 7)

avg = y / num_experiments
print ("The average after " + str(num_experiments) + " experiments is " + str(avg))
