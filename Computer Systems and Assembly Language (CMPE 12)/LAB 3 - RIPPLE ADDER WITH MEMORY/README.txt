Leonid Shuster, lshuster@ucsc.edu
Lab 3: Ripple Adder with Memory
2/7/18
01J, Ehsan Hemmati

Magic Numbers:
0234
010110
------------------------------------------------------------------------------------------
In this lab, I learned not only how to build a circuit that added or subtracted numbers in binary, but also further understood generally how exactly adding or subtracting numbers in binary works. Furthermore, I also did not know that you could use a MUX to choose a specific input based on what you tell it to select, and that a Flip-Flop could store something to be used for later. Finally, I saw how combinational and sequential logic can be put into practice. I found that it worked well to think about addition and subtraction with every bit individually, and what procedures I would have to do to get the output I wanted. 

At first, I found it hard to see how each of the modules would work together to output a result. Later did I see that I had to pass the keyboard inputs into an inverter if they were being subtracted, and then into an adder where they were added to the current numbers in the Flip-Flop, and then finally into a Flip-Flop where they were stored for later and outputted. In addition, I also did not have an issue adding numbers, but I did for subtracting them. I did not understand at first that I had to use a MUX to invert my numbers and then add them.

For all of my Modules, I added LEDs to each of the outputs to keep track of what output I was getting. If something went wrong, I was able to write down what should have happened for every step and find exactly where the problem occurred.

Since we are doing unsigned numbers, when you subtract a larger number from a smaller number, it goes back up to its highest value, 3F (or 63), and then decrements from there. If you add two numbers that don't fit into 6 bits, it goes back to 0s, and then increments from there. 