#1616143 Leon Shuster
import time

num_sec = int(input("How many seconds? "))

for a in range(num_sec, -1, -1):
    print("Remaining seconds = " + str(a))
    time.sleep(1)
print("Time's up! ")
