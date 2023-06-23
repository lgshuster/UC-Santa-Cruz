# Leonid Shuster, lshuster@ucsc.edu
# Lab 6: Vigenere Cipher
# 3/11/18
# 01J, Ehsan Hemmati
# 
# Pseudocode:
#
# EncryptString/DecryptString:
# 	While plaintext string is not at end and cipher string is more than 30 characters:
#	Load first string character and first key string character
#	Check if character is invalid:
#		If value of character is less than 65, greater than 90 and less than 97, or greater than 122:
#			Move on to next plaintext character
#			Stay on the same key string character
#		Else:
#			EncryptChar/DecryptChar
# 			Move on to next character of string and key string
# 			Reset key if it reaches the end of the key
# EncryptChar:
#	Check if letter is uppercase or lowercase: 
# 		Convert string character and key character to decimal value 
#		Add them, do modolus 26
# 		Take remainder and convert back to ascii value
# 		Return value to EncryptString
# EncryptChar:
#	Check if letter is uppercase or lowercase: 
# 		Convert string character and key character to decimal value 
#		Add them, do modolus 26
# 		Take remainder and convert back to ascii value
# 		Return value to DecryptString
#
#
.text
#----------------------------Encryption----------------------------
EncryptString:
	# Stores register address
	la $s0, ($ra)
	
	# Resets counter
	__ResetEncryptionCounter:
		li $t5, 0
		
	# Stores a0 and a1	
	__StoreEncryptionArguments:
		# Stores $s
		move $t5, $s0
		move $t6, $s1
		
		# Store $a
		move $s1, $a1
	
	__Encrypting:
		# Loads character from strings
		lb $t0, ($a0)
		lb $t1, ($a1)
				
		# Exits if reaches end of string
		beqz $t0, exit
		
		# Exits if encrypts 30 characters
		beq $t5, 30, exit
		
		# Checks for invalid character between ASCII values 0 - 64, 91 - 96, and 123 - 127
		ble $t0, 64, __InvalidEncryptionCharacter
		bge $t0, 91, __PossibleInvalidEncryptionCharacter
		bge $t0, 123, __InvalidEncryptionCharacter
			
		j EncryptChar
		
	
		__AfterEncryption:
			lb $a2, ($v0)
			addi $a2, $a2, 1
		
			# Moves on to the next character
			addi $a0, $a0, 1
			addi $a1, $a1, 1
	
			# Adds one to the counter for string
			addi $t5, $t5, 1
			
			# Branches if key reaches end of string
			lb $t1, ($a1)
			beqz $t1, __LoopEncryptionKey
			
			# Loops
			j __Encrypting
	
# Invalid Character if ASCII value is greater than 90 but less than 97
__PossibleInvalidEncryptionCharacter:
	ble $t0, 96, __InvalidEncryptionCharacter
	j EncryptChar
		
# If invalid character, print invalid character but don't advance key string
__InvalidEncryptionCharacter:
	lb $a2, ($v0)
	addi $a2, $a2, 1
	
	# Moves on to the next character for string
	addi $a0, $a0, 1
	
	# Adds one to the counter for string
	addi $t5, $t5, 1
	
	j __Encrypting
	
# Loops to beginning of key if it reaches the end
__LoopEncryptionKey:
	move $a1, $s1
	 
	j __Encrypting



	
EncryptChar:
	# Branches depending on if character is uppercase or lowercase
	ble $t0, 90, __EncryptedUpperCase
	ble $t0, 122, __EncryptedLowerCase

	
__EncryptedLowerCase:
	# Converts from ASCII to decimal
	subi $t0, $t0, 71  #adds 26, 97 - 26 = 71
	subi $t1, $t1, 65
		
	# Adds the two numbers
	add $t2, $t1, $t0
	
	# Loads 26 into $t3
	li $t3, 26
	
	# $t2 % 26
	div $t2, $t3
	mfhi $t4
	
	# Converts back to ASCII
	addi $v0, $t4, 97

	j __AfterEncryption
		
__EncryptedUpperCase:
	# Converts from ASCII to decimal
	subi $t0, $t0, 39  #adds 26, 65 - 26 = 39
	subi $t1, $t1, 65
	
	# Adds the two numbers
	add $t2, $t1, $t0
	
	# Loads 26 into $t3
	li $t3, 26
	
	# $t2 % 26
	div $t2, $t3
	mfhi $t4
	
	# Converts back to ASCII
	addi $v0, $t4, 65
	
	j __AfterEncryption

#----------------------------Decryption----------------------------	
DecryptString:
	# Resets counter
	__ResetDecryptionCounter:
		li $t5, 0
		
	# Stores a0 and a1	
	__StoreDecryptionArguments:
		# Stores $s
		move $t5, $s0
		move $t6, $s1
		move $t7, $s2
		
		# Store $a
		move $s1, $a1
		move $s2, $a2
		
		# Stores register address
		move $s0, $ra
	
	__Decrypting:
		# Loads character from strings
		lb $t0, ($a0)
		lb $t1, ($a1)
				
		# Exits if reaches end of string
		beqz $t0, exit
		
		# Exits if encrypts 30 characters
		beq $t5, 30, exit
		
		# Checks for invalid character between ASCII values 0 - 64, 91 - 96, and 123 - 127
		ble $t0, 64, __InvalidDecryptionCharacter
		bge $t0, 91, __PossibleInvalidDecryptionCharacter
		bge $t0, 123, __InvalidDecryptionCharacter
			
		j DecryptChar
		
		__AfterDecryption:
			lb $a2, ($v0)
			addi $a2, $a2, 1
		
			# Moves on to the next character
			addi $a0, $a0, 1
			addi $a1, $a1, 1
	
			# Adds one to the counter for string
			addi $t5, $t5, 1
			
			# Branches if key reaches end of string
			lb $t1, ($a1)
			beqz $t1, __LoopDecryptionKey
			
			# Loops
			j __Decrypting
	
# Invalid Character if ASCII value is greater than 90 but less than 97
__PossibleInvalidDecryptionCharacter:
	ble $t0, 96, __InvalidDecryptionCharacter
	j DecryptChar
		
# If invalid character, print invalid character but don't advance key string
__InvalidDecryptionCharacter:
	lb $a2, ($v0)
	addi $a2, $a2, 1
	
	# Moves on to the next character for string
	addi $a0, $a0, 1
	
	# Adds one to the counter for string
	addi $t5, $t5, 1
	
	j __Decrypting
	
# Loops to beginning of key if it reaches the end
__LoopDecryptionKey:
	# Resets key
	move $a1, $s1
	 
	j __Decrypting



	
DecryptChar:
	# Branches depending on if character is uppercase or lowercase
	ble $t0, 90, __DecryptedUpperCase
	ble $t0, 122, __DecryptedLowerCase
	
	
__DecryptedLowerCase:
	# Converts from ASCII to decimal
	subi $t0, $t0, 71  #adds 26, 97 - 26 = 71
	subi $t1, $t1, 65
		
	# Subtracts the two numbers
	sub $t2, $t0, $t1
	
	# Loads 26 into $t3
	li $t3, 26
	
	# $t2 % 26
	div $t2, $t3
	mfhi $t4
	
	# Converts back to ASCII
	addi $v0, $t4, 97

	j __AfterDecryption
		
__DecryptedUpperCase:
	# Converts from ASCII to decimal
	subi $t0, $t0, 39  #adds 26, 65 - 26 = 39
	subi $t1, $t1, 65
	
	# Adds the two numbers
	sub $t2, $t0, $t1
	
	# Loads 26 into $t3
	li $t3, 26
	
	# $t2 % 26
	div $t2, $t3
	mfhi $t4
	
	# Converts back to ASCII
	addi $v0, $t4, 65
	
	j __AfterDecryption
	
exit:
	# Adds zero to end of cipher string
	sb $a2, 0
	
	__ResetArguments:
		# Resets register address
		la $ra, ($s0)
		
		# Resets cipher string
		move $a2, $s2
		
		# Resets $s registers
		move $s0, $t5
		move $s1, $t6
		move $s2, $t7
		
	jr $ra
