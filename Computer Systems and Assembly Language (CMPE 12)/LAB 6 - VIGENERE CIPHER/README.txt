Leonid Shuster, lshuster@ucsc.edu
Lab 6: Vigenere Cipher
3/11/18
01J, Ehsan Hemmati
--------------------------------------------------------------------------------------------------------
In this lab, I learned more about the ASCII table, and how to work with addresses and access characters from strings. I also became more efficient with conditional statements and creating functions, and returning to my original code. I found that it worked well to run my code step by step and have a syscall that printed out various characters every loop. The hardest part for me was to figure out how to store my register address. I tried to use the stack, and couldn't figure out how to use it. I also tried to store the address in an $s register, but that also didn't work out for me.

I did write additional code. I needed to be able to branch when certain conditions were met, so I wrote extra functions. It did work, as the functions were only called when I needed them and did what I wanted them to do.

When you attempt encryption with a keystring that has illegal characters, the program begins printing out characters that aren't letters. This is because when you add the plaintext character and the key character, and then do mod 26, it gives you a character that isn't a letter.

In order to write a recursive subroutine, you would need the function to call itself with new arguments every time, with the function ending once the results it outputs reach a certain condition.

To pass more than 4 arguments, you could pass those four arguments, store something new into the $a registers, and then pass those new arguments. You could also use store new arguments into the temporary $t registers.