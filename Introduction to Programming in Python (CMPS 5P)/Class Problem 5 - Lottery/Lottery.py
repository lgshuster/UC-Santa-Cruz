#1616143 Leon Shuster

import random
import time
guess = int(input("Guess a number between 1 and 10 "))
rand_num = random.randrange(1, 11)
wait = time.sleep(3)

if guess == rand_num:
    print ("Your guess was correct! The number was " + str(rand_num))
else:
    print("Your guess was not correct. The number was " + str(rand_num))
