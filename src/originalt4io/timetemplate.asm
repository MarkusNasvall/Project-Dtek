  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,2
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
  
hexasc:
    	andi $a0, $a0, 0xF   # Keep only the 4 least significant bits (0-15)
    	# We are using 0xF because it has the representation 0B1111 in binary
    
    	# Check if the number is from 10 to 15 and convert to ASCII letters A-F
    	li   $t0, 0xA     	# Load '10' into $t0
    	bge  $a0, $t0, letter_conversion # if $a0 is greater than or equal to '10', execute letter_conversion
    	addi $v0, $a0, 0x30  # Convert numbers 0-9 to ASCII codes '0'-'9' by adding '48' or 0x30
    	j    end_conversion
	nop
letter_conversion:
    	addi $v0, $a0, 0x37  # Convert numbers 10-15 to ASCII codes 'A'-'F' by adding '55' or 0x37

end_conversion:
    	andi $v0, $v0, 0x7F  # Ensure only the 7 least significant bits are set to ASCII code
    	# We are using 0x7F because it has the representation 0B1111111 in binary
    	jr   $ra             # Return from the subroutine
    	nop                  # delay slot filler (just in case)

delay:
    	# Check if ms (in $a0) is less than or equal to 0
    	blez $a0, end_delay       # If $a0 <= 0, jump to end_delay
	nop
outer_loop:
    	# Decrement ms
    	addi $a0, $a0, -1

    	# Load the constant into $t0 for inner loop counter
    	li $t0, 9000

inner_loop:
    	# Decrement the counter
    	addi $t0, $t0, -1         # $t0 = $t0 - 1

    	# Check if counter is 0
    	bnez $t0, inner_loop      # If $t0 != 0, repeat inner loop
	nop
    	# Check if ms (in $a0) is still greater than 0
    	bgtz $a0, outer_loop      # If $a0 > 0, repeat outer loop
	nop
end_delay:
    	jr $ra                    # Return to caller
    	nop                       # Placeholder (for delay slot)
	
	
time2string:
    	# Save registers
    	PUSH($ra)       # Save return address
    	PUSH($s1)       # Another temporary register
    	PUSH($s2)       # Temporary register to save initial $a0

    	# Save the initial value of $a0 in $t2
    	move $s2, $a0

    	# Extract and convert the first minute digit (most significant)
    	srl  $s1, $a1, 12       # Right shift by 12 bits to isolate the most significant digit
    	andi $s1, $s1, 0xF     # Mask other digits
    	move $a0, $s1          # Move the digit to $a0 for hexasc
    	jal  hexasc             # Call hexasc
    	nop
    	sb   $v0, 0($s2)       # Store the ASCII code to memory
    	addi $s2, $s2, 1       # Increment memory address

    	# Extract and convert the second minute digit
    	srl  $s1, $a1, 8        # Right shift by 8 bits to get the next digit
    	andi $s1, $s1, 0xF     # Mask other digits
    	move $a0, $s1          # Move the digit to $a0 for hexasc
    	jal  hexasc             # Call hexasc
    	nop
    	sb   $v0, 0($s2)       # Store the ASCII code
    	addi $s2, $s2, 1       # Increment memory address

    	# Store the colon character
    	li   $v0, 0x3A          # Load ASCII value for colon
    	sb   $v0, 0($s2)       # Store the colon character
    	addi $s2, $s2, 1       # Increment memory address

    	# Extract and convert the first seconds digit (most significant of the two)
    	srl  $s1, $a1, 4        # Right shift by 4 bits to get the next digit
    	andi $s1, $s1, 0xF     # Mask other digits
    	move $a0, $s1          # Move the digit to $a0 for hexasc
    	jal  hexasc             # Call hexasc
    	nop
    	sb   $v0, 0($s2)       # Store the ASCII code
    	addi $s2, $s2, 1       # Increment memory address

    	# Extract and convert the second seconds digit (least significant of all)
    	andi $s1, $a1, 0xF     # Mask all but the least significant digit
    	move $a0, $s1          # Move the digit to $a0 for hexasc
    	jal  hexasc             # Call hexasc
    	nop
    	sb   $v0, 0($s2)       # Store the ASCII code
    	addi $s2, $s2, 1       # Increment memory address



    	# Check if seconds are even or odd and store 'D' or 'E'
    	andi $s1, $a1, 0x1  # Isolate the least significant bit
    	beqz  $s1, storeE  # If LSB is 0 (even), store 'E'
    	li   $v0, 0x44  # ASCII for 'D'
    	j    continueStore
    	nop

storeE:
    	li   $v0, 0x45  # ASCII for 'E'

continueStore:
    	sb   $v0, 0($s2)  # Store byte in $t2
    	
	
    	# Store null byte
    	li   $v0, 0x00          # Load ASCII value for null byte
    	sb   $v0, 1($s2)        # Store null byte at the next address

    	# Restore registers
    	move $a0, $s2           # Restore $a0 from $t2
    	POP($s2)                # Restore temporary register $t2
    	POP($s1)                # Restore $t1
    	POP($ra)                # Restore return address

    	# Return
    	jr   $ra                # Return
    	nop                     # delay slot filler
