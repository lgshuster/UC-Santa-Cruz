Leonid Shuster, lshuster@ucsc.edu
Lab 4: DEADBEEF in MIPS
2/16/18
01J, Ehsan Hemmati

Magic Numbers:
9671
0x1224
3333
------------------------------------------------------------------------------------
In this lab, I learned the basics of how to program in MIPS. I learned how to work with registers, how to write a function, a for loop, and if statements. It worked well to write my program slowly and one step at a time. At first, I was lost with how to start. However, just as the lab recommended, I started with outputting something, then took input from the user, then implemented a for loop that printed out numbers, then added conditionals that controlled the flow of the program and ultimately got me the result I wanted. I found it hard to learn this language on my own, and had to search up and watch videos every time I was unsure about how to do something. In the end though, I got it.

I believe that how large N can be depends on the power of CPU, and how much memory your computer can store. Once the program has taken up all of the CPU, and cannot store any more memory, then the program will begin to fail. 

The prompt is found in address 0x10010000

Instructions such as move, li, la, beqz, and add using a 16 bit immediate value are translated into other instructions. They produce the appropriate result by changing not only the names but the arguments given to it so that they produce the same result as the names you entered.

I used 11 registers. I could have used fewer registers if I put new values into old registers, replacing the value that was previously there. I could have also used less registers if I wrote my code more effectively, and didn't have to keep using a new register every time I had a new value I wanted to store.