# test_Lab6.asm
# Date:  2/27/2018
# Author:  CMPE12 staff
# This code calls EncryptChar and EncryptString from Lab6.asm.

#-------helpful macros to improve readability:-------
.macro sys(%which_sys)
	li $v0, %which_sys
	syscall
.end_macro
.macro print_str(%label)
	la $a0, %label
	li $v0, 4
	syscall
.end_macro
.macro print_char(%register)
	move $a0, %register
	li $v0, 11
	syscall
.end_macro
.eqv maxStrLen 31 #spec only requires encrypting 30 characters


#-------Main testing code for Lab6.asm-------
.text
main: nop
	jal testEC #A subroutine to test student's EncryptChar
	nop
	
	jal testDC #A subroutine to test student's DecryptChar
	nop

	jal testES #A subroutine to test student's EncryptString
	nop
	
	jal testDS #A subroutine to test student's DecryptString
	nop
	
	sys(10) #exit


#-------Subroutines for individual tests:-------
# Subroutine testEC
# input:	None
# output:  	None
# Side effects: Prints report of results of EncryptChar,
#		Also overwrites $a0, $a1, and $v1
.data
testEC_introstr: 	.ascii 	"\n>>> Encrypt char test:"
			.ascii  "\n plain, key, expected, actual = "
testEC_plainchar: 	.ascii 	"N"
testEC_keychar: 	.ascii 	"B"
testEC_expectedchar: 	.asciiz "O"
newLine:		.asciiz "\n"
.text
testEC: nop
	sw $ra, ($sp) #store return address
	subi $sp, $sp, 4

	print_str(testEC_introstr) #print test description
	
	lb $a0, testEC_plainchar #prepare arguments to student's subroutine
	lb $a1,	testEC_keychar
	lb $a2,	testEC_expectedchar
	
	jal EncryptChar  #call student's subroutine
	
	print_char($v0) #print result
	li $v0, 4
	la $a0, newLine
	syscall
	
	addi $sp, $sp, 4 #return to calling code
	lw $ra, ($sp)
	jr $ra
	
#-------Subroutines for individual tests:-------
# Subroutine testDC
# input:	None
# output:  	None
# Side effects: Prints report of results of DecryptChar,
#		Also overwrites $a0, $a1, and $v1
.data
testDC_introstr: 	.ascii 	"\n>>> Decrypt char test:"
			.ascii  "\n plain, key, expected, actual = "
testDC_plainchar: 	.ascii 	"N"
testDC_keychar: 	.ascii 	"B"
testDC_expectedchar: 	.asciiz "M"
.text
testDC: nop
	sw $ra, ($sp) #store return address
	subi $sp, $sp, 4

	print_str(testDC_introstr) #print test description
	
	lb $a0, testDC_plainchar #prepare arguments to student's subroutine
	lb $a1,	testDC_keychar
	lb $a2,	testDC_expectedchar
	
	jal DecryptChar  #call student's subroutine
	
	print_char($v0) #print result
	
	addi $sp, $sp, 4 #return to calling code
	lw $ra, ($sp)
	jr $ra
	
# Subroutine testES
# input:	None
# output:  	None
# Side effects: Prints report of results of EncryptString,
#		Also overwrites $a0, $a1, and $v1
.data
testES_introstr:	.ascii		"\n\n>>> Encrypt string test:"
testES_prefix1: 	.ascii		"\n     Plain: "
testES_plain:   	.asciiz 			"aBcDeFg!123 qRsTuVwXyZ  aBcdeF!"
testES_prefix2: 	.ascii 		"\n       Key: "
testES_key: 		.asciiz  			"SDF"
testES_prefix3: 	.ascii 		"\n  Expected: "
testES_expected:	.asciiz 	 		"sEhVhKy!123 tWkWzNzCqC  fTfiwI"
testES_prefix4: 	.asciiz 	"\n    Actual: "
testES_result: 		.space 	 	maxStrLen				

.text
testES: nop
	sw $ra, ($sp) #store return address
	subi $sp, $sp, 4
	
	print_str(testES_introstr) #print test description
	print_str(testES_prefix2)
	print_str(testES_prefix3)
	print_str(testES_prefix4)
	
	la $a0, testES_plain #prepare arguments to student's subroutine
	la $a1,	testES_key
	la $a2,	testES_result

	jal EncryptString  #call student's subroutine
	
	print_str(testES_result)  #print the result

	addi $sp, $sp, 4 #return to calling code
	lw $ra, ($sp)
	jr $ra
	
# Subroutine testDS
# input:	None
# output:  	None
# Side effects: Prints report of results of DecryptString,
#		Also overwrites $a0, $a1, and $v1
.data
testDS_introstr:	.ascii		"\n\n>>> Decrypt string test:"
testDS_prefix1: 	.ascii		"\n     Plain: "
testDS_plain:   	.asciiz 			"sEhVhKy!123 tWkWzNzCqC  fTfiwI!"
testDS_prefix2: 	.ascii 		"\n       Key: "
testDS_key: 		.asciiz  			"SDF"
testDS_prefix3: 	.ascii 		"\n  Expected: "
testDS_expected:	.asciiz 	 		"aBcDeFg!123 qRsTuVwXyZ  aBcdeF"
testDS_prefix4: 	.asciiz 	"\n    Actual: "
testDS_result: 		.space 	 	maxStrLen				

.text
testDS: nop
	sw $ra, ($sp) #store return address
	subi $sp, $sp, 4
	
	print_str(testDS_introstr) #print test description
	print_str(testDS_prefix2)
	print_str(testDS_prefix3)
	print_str(testDS_prefix4)
	
	la $a0, testDS_plain #prepare arguments to student's subroutine
	la $a1,	testDS_key
	la $a2,	testDS_result

	jal DecryptString  #call student's subroutine
	
	print_str(testDS_result)  #print the result

	addi $sp, $sp, 4 #return to calling code
	lw $ra, ($sp)
	jr $ra
		
nop #---------------------END CALLING CODE-------------------------------------------------
nop
nop #---------------------BEGINNING OF LAB6 CODE-------------------------------------------
#student code is included here:
.include "Lab6.asm"
