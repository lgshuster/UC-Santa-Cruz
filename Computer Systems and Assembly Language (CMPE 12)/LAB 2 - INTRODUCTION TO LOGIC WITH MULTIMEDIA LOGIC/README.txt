Leonid Shuster, lshuster@ucsc.edu
Lab 2: Introduction to Logic with MultiMedia Logic
1/26/18
01J, Ehsan Hemmati

Sources: 
https://en.wikipedia.org/wiki/Pseudorandom_number_generator
http://whatis.techtarget.com/definition/pseudo-random-number-generator-PRNG
https://www.random.org/randomness/


- How would you make your own 7-segment display from Part B if you didn’t have one in MML?
- How do you think the random number generator works?
- How can things be really random in a computer when it is made of logic gates, which are
supposed to be deterministic?

------------------------------------------------------------------------------------------
I would create a circuit that would, with the help of logic gates, output a digit depending on the placement of 0's and 1's in a four digit input.

Random number generators work by choosing a truly random starting value, known as a seed value. From there, the computer uses an algorithm to generate numbers which appear random, but are actually predictable.

A computer uses what's known as a pseudorandom number generator, which is predictable. Although at first, the numbers may seem random, because it uses an algorithm, a pattern starts to occur and the numbers repeat. In order for a computer to be truly random, it must measure something that is truly random, like some natural phenomenon that can never be predicted, such as atmospheric noise or the behavior of a lava lamp.

I learned that we can use a simulation of a circuit to imitate how binary works. By using a combination of on and off switches that replicate a series of 0s and 1s in binary. I was really impressed that instead of coding if statements, and using logical operators, we instead used wires and gates that controlled the flow of the circuit.
