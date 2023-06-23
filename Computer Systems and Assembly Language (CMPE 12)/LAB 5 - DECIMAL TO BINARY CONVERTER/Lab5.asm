# Leonid Shuster, lshuster@ucsc.edu
# Lab 5: Decimal to Binary Converter
# 2/28/18
# 01J, Ehsan Hemmati
#
# Got help from Maxim Kuznestov 
#
# Pseudocode:
#
# Store program argument in $s0
# 
# Read first character, check if it's a minus sign
#	If it isn't, start with converting string to integer
# 	If it is, add 1 to $s1
# 
# While string is not \0:
# 	Check first character to see if it's a minus sign:
#		If it is, continue with conversion and increment counter by 1
#		If it isn't, change string number into integer
# 	Multiply previous value in $s2 by 10 and add new integer value
#
# If the first character is negative:
# 	multiply integer by -1
#
# For 32 bits:
#	Shift over the address of the integer:
#		If bit is 0, print 0
#		If bit is 1, print 1
# 	If address > 32 bits:
#		print remainder 0's or 1's
#
#
.data
	message1: .asciiz "User input number: \n"
	message2: .asciiz "\nThis number in binary is: \n"
  	
.text

main:
	# Moves address of string to $s0
	move $s0, $a1
	lw $s0, ($s0)
	
	# Prints out first message
	la $a0, message1
	li $v0, 4	
	syscall           	
	
    	# Prints out program argument
    	la $a0, ($s0)
    	li $v0, 4
    	syscall		
	
	# Register for if first character is negative 	
	li $s1, 0
		
	# Register for integer number of string	
	li $s2, 0		
	
	# Loads first character to see if negative
	lb $t0, ($s0)
	
	# Branches if first character is not negative	
	bne $t0, 0x2D, stringToInt
	
	# Sets $s1 = 1 if first character is negative
	li $s1, 1		

	# Calls function that changes string to integer
	jal stringToInt
	
	onceNumberIsSigned:
		# Copies integer into $s0
		move $s0, $s2
		
    		# Prints out 2nd message
    		la $a0, message2
    		li $v0, 4
    		syscall 
    		
    		
    		li $s3, 0
    		
    		# Calls function that changes integer to binary
 		jal intToBinary
 		
 		onceNumberIsInBinary:
 			# Ends the program
 			jal end
 			
#------------------------------------Functions--------------------------------------
stringToInt:
	# Loads first character into $t0 			
	lb $t0, ($s0)
	
	# Branches if first character is minus sign
	beq $t0, 0x2D, skipArith
	
	# Branches if it reaches the end of the loop
	beqz $t0, signingTheInteger
	
	# Subtracts new string character by 48 to get integer value
	addi $t0, $t0, -48
	
	# Multiplies current number in $s2 by 10
	mul $s2, $s2, 10
	
	# Adds integer to previous integer (0 if there isn't previous)
	add $s2, $s2, $t0
	
	skipArith:
		# Iterates to next character
		addi $s0, $s0, 1
	
		# Loops back to the beginning
		j stringToInt	
		
signingTheInteger:	
	# Checks to see if $s1 is 0 (positive) or 1 (negative)
	beq $s1, 0, onceNumberIsSigned
	
	# Multiplies number by -1 to make negative
	mul $s2, $s2, -1
	
	# Returns back to main
	j onceNumberIsSigned
	
intToBinary:
	# Copies integer into $t1
	move $t1, $s0
	
	# Shifts left by $s3
	sllv $t1, $t1, $s3	
	
	# Branches if $t1 = 0
	beqz $t1, endWithZeros
	
	# Masks every bit except the MSB	
	andi $t1, $t1, 0x80000000	
	
	# Branches if the bit is 0
	beqz $t1, zeroBit 
	
	# Prints 1
	li $a0, 1 	
	li $v0, 1 	
	syscall
	
	# Skips adding a 0 bit
	b continueLoop
	
	zeroBit: 
		# Prints 0
		li $a0, 0 
		li $v0, 1 	
		syscall
	
	continueLoop:
		# Increments by 1
		addi $s3, $s3, 1 
		
		# Branches if reaches 32 bits
		beq $s3, 32, onceNumberIsInBinary 	

		# Loops back to the beginning
		b intToBinary

	endWithZeros:
		# Adds the remaining zeroes
		li $a0, 0 
		li $v0, 1 	
		
		# Branches if reaches 32 bits
		beq $s3, 32, onceNumberIsInBinary
		syscall 
		
		# Increments by 1		
		addi $s3, $s3, 1 
		
		# Keeps looping to add zeroes
		b endWithZeros
	
end:
    	li $v0, 10
    	syscall
