  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,17	# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register
	
	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #

hexasc:
    	andi $a0, $a0, 0x0F   # Keep only the 4 least significant bits (0-15)
    	# We are using 0x0F because it has the representation 0B1111 in binary
    
    	# Check if the number is from 10 to 15 and convert to ASCII letters A-F
    	li   $t0, 0xA     	# Load '10' into $t0
    	bge  $a0, $t0, letter_conversion # if $a0 is greater than or equal to '10', execute letter_conversion
    	addi $v0, $a0, 0x30  # Convert numbers 0-9 to ASCII codes '0'-'9' by adding '48' or 0x30
    	j    end_conversion

letter_conversion:
    	addi $v0, $a0, 0x37  # Convert numbers 10-15 to ASCII codes 'A'-'F' by adding '55' or 0x37

end_conversion:
    	andi $v0, $v0, 0x7F  # Ensure only the 7 least significant bits are set to ASCII code
    	# We are using 0x7F because it has the representation 0B1111111 in binary
    	jr   $ra             # Return from the subroutine
    	nop                  # delay slot filler (just in case)
