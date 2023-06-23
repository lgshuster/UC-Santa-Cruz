#Leonid Shuster, lshuster@ucsc.edu
#Lab 4: DEADBEEF in MIPS
#2/16/18
#01J, Ehsan Hemmati

.data
  	prompt: .asciiz "Please enter a number N: "
  	dead: .asciiz "DEAD"
  	beef: .asciiz "BEEF"
  	deadbeef: .asciiz "DEADBEEF"
  	NL: .asciiz "\n"
  
.text
main:
	# Prints out message
    	li $v0, 4
    	la $a0, prompt
    	syscall
  
    	# Asks user for input
    	li $v0, 5
    	syscall
  
    	# Move input to different register
    	move $s1, $v0
  
    	# Initializing the loop variables
    	la $t0, ($s1)	# Last number, the user's input
    	li $t1, 1		# First number
  
# For loop
loop:
	# Checking for Divisibility
	jal isDivisible
      
      	return_here:
      		# Branches to end if $t1 is equal to $t0
        	beq $t1, $t0, end
        
        	jal printLine
    
        	# Increments by one
        	addi $t1, $t1, 1
      
        	# Loops again
        	j loop
  
isDivisible:
	# Puts the numbers into register
    	addi $t2, $zero, 4
    	addi $t3, $zero, 9
    
    	# Num divided by 4
    	div $t1, $t2
    	mfhi $t4
    
    	# Num divided by 9
    	div $t1, $t3
    	mfhi $t5
    
    	# Branches off depending on whether divisible by 4 and 9
    	beqz $t4, printDead
    	beqz $t5, printBeef
    
    	jal printInt
    
    	# Returns back to loop
    	j return_here
    
# Prints Dead if divisible by just 4
printDead:
    	# Checks if divisible by 9 also
    	div $t1, $t3
    	mfhi $t6
    
    	beqz $t6, printDeadBeef
    
    	li $v0, 4
    	la $a0, dead
    	syscall
    
    	j return_here
  
# Prints Beef if divisible by just 9
printBeef:
	# Checks if divisible by 4 also
    	div $t1, $t2
    	mfhi $t6
    
    	beqz $t6, printDeadBeef
    
    	li $v0, 4
    	la $a0, beef
    	syscall
    
    	j return_here
  
# Prints DeadBeef if divisible by both 4 and 9 
printDeadBeef:
    	li $v0, 4
    	la $a0, deadbeef
    	syscall
    
    	j return_here
  
# Prints the current number
printInt: 
    	li $v0, 1
    	la $a0, ($t1)
    	syscall
    
    	jr $ra
  
# Prints new line
printLine:
    	li $v0, 4
    	la $a0, NL
    	syscall
    
    	jr $ra
  
# Terminates program
end:
    	li $v0, 10
    	syscall
