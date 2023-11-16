  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.global time2string, hexasc, letter_conversion, end_conversion, delay, inner_loop, outer_loop, end_delay

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
    	PUSH $ra       # Save return address
    	PUSH $t1       # Another temporary register
    	PUSH $t2       # Temporary register to save initial $a0

    	# Save the initial value of $a0 in $t2
    	move $t2, $a0

    	# Extract and convert the first minute digit (most significant)
    	srl  $t1, $a1, 12       # Right shift by 12 bits to isolate the most significant digit
    	andi $t1, $t1, 0xF     # Mask other digits
    	move $a0, $t1          # Move the digit to $a0 for hexasc
    	jal  hexasc             # Call hexasc
    	nop
    	sb   $v0, 0($t2)       # Store the ASCII code to memory
    	addi $t2, $t2, 1       # Increment memory address

    	# Extract and convert the second minute digit
    	srl  $t1, $a1, 8        # Right shift by 8 bits to get the next digit
    	andi $t1, $t1, 0xF     # Mask other digits
    	move $a0, $t1          # Move the digit to $a0 for hexasc
    	jal  hexasc             # Call hexasc
    	nop
    	sb   $v0, 0($t2)       # Store the ASCII code
    	addi $t2, $t2, 1       # Increment memory address

    	# Store the colon character
    	li   $v0, 0x3A          # Load ASCII value for colon
    	sb   $v0, 0($t2)       # Store the colon character
    	addi $t2, $t2, 1       # Increment memory address

    	# Extract and convert the first seconds digit (most significant of the two)
    	srl  $t1, $a1, 4        # Right shift by 4 bits to get the next digit
    	andi $t1, $t1, 0xF     # Mask other digits
    	move $a0, $t1          # Move the digit to $a0 for hexasc
    	jal  hexasc             # Call hexasc
    	nop
    	sb   $v0, 0($t2)       # Store the ASCII code
    	addi $t2, $t2, 1       # Increment memory address

    	# Extract and convert the second seconds digit (least significant of all)
    	andi $t1, $a1, 0xF     # Mask all but the least significant digit
    	move $a0, $t1          # Move the digit to $a0 for hexasc
    	jal  hexasc             # Call hexasc
    	nop
    	sb   $v0, 0($t2)       # Store the ASCII code


    	# Store null byte
    	li   $v0, 0x00          # Load ASCII value for null byte
    	sb   $v0, 1($t2)        # Store null byte at the next address

    	# Restore registers
    	move $a0, $t2           # Restore $a0 from $t2
    	POP $t2                # Restore temporary register $t2
    	POP $t1                # Restore $t1
    	POP $ra                # Restore return address

    	# Return
    	jr   $ra                # Return
    	nop                     # delay slot filler
