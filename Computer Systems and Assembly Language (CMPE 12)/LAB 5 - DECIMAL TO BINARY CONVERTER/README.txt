Leonid Shuster, lshuster@ucsc.edu
Lab 5: Decimal to Binary Converter
2/28/18
01J, Ehsan Hemmati

Magic Numbers:
0b1010
0x0745
0123
------------------------------------------------------------------------------------
In this lab, I learned about how values and addresses are stored, and how to use and manipulate those addresses. I also more deeply understood about MIPS's functionalities, and got better at using conditional statements. I found it useful to make a plan about what I wanted to do before I did it, because then I had a plan to follow, and would know what to do after every step. I found it hard to do more complicated tasks in MIPS. I would know how to do the lab if it were in another language, but I found it challenging to do what I wanted in MIPS when I didn't know which words and functionalities would do what I wanted.

When the user inputs a number that is too large to fit, the program begins printing numbers that are out of the range of 32 bits, and the only numbers that are visible are 0's.

When the user inputs a number that is too small to fit, the program does the same thing, and begins printing numbers that are out of the range of 32 bits, and the only numbers the are visible are 0's.

The difference between mult and multu is that mult is multiplication for signed numbers, and multu is multiplication for unsigned numbers. I used mul, since I wanted to store the product within a register on one line rather than having to access the product from mfhi and then store it into a register.

If the user inputted a string of 0's and 1's, I would first need to convert that string of numbers into integers. Then, I would need start from the beginning of the integer and multiply the digit (0 or 1) by 2^N depending on what place it's at, and have a running total that sums up every product.