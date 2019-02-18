
# mp1.S - missile-command version
# Good luck, and have fun!

.data
	
    # Constants for accessing the fields of a struct missile, 
    # struct missile is defined in rtc.h and mp1.h

	NEXT 	  = 0
	X	  = 4
	Y	  = 8
	VX	  = 12
	VY	  = 16
	DEST_X	  = 20
	DEST_Y	  = 24
	EXPLODED  = 28
	C	  = 32

    # Character to draw for an explosion - '@'

	EXPLOSION = 64

    missile_size = 33

# Data shared between this file and rtc.c helper functions

# This '.globl' directive makes these symbols visible externally
.globl mp1_missile_list, base_alive, mp1_score

mp1_missile_list:	.long 0x0	# Head of list pointer
base_alive:		.long 0x0	# Array of 3 bytes, plus a padding byte
mp1_score:		.long 0x0	# Player's current score


# Data private to this  file

base_pic:     	    .string "/^^^\\"	# Picture of a live base
dead_base_pic: 	    .string "xxxxx"	# Picture of a dead base
crosshairs_x:		.long 0x0	# X-position of the crosshairs
crosshairs_y:		.long 0x0	# Y-position of the crosshairs
score_tobe_copied:  .long 0x0   # score_tobe_copied to user

.text

# void mp1_poke(void);
# You can use this function to write to video memory.
#
# Interface: Register-based arguments (not C-style)
#    Inputs: %cl  - The byte you wish to write
#            %eax - Offset from the start of video memory that you wish
#                   to write to
#   Outputs: Text-mode video screen is written to at location %eax with
#            the byte in %cl
# Registers: Clobbers EDX

mp1_poke:
		movl    vmem_base_addr(,1),%edx
		movb    %cl,(%edx,%eax,1)
		ret

#   ----------------- Exported functions ---------------------

# void mp1_rtc_tasklet(unsigned long garbage);
# Performs three tasks:
#	(1) updates the list of missiles (implement this in update_missiles,
#           below, and call it from here).
#	(2) Redraw the bases - they may have been overwritten with missiles
#	(3) Redraw the crosshairs - it may have been overwritten with missiles
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention

.globl mp1_rtc_tasklet
mp1_rtc_tasklet:
		pushl %ebp
        movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi 
        movl mp1_missile_list, %edx
next_missile:
        cmpl $0, %edx
        je done_all_missiles                       					
        cmpl $0, EXPLODED(%edx)                     #check if exploding
        je update_missile                          #if not, update missile
        jne missile_exploding					   #if it is, do missile_explode
		
done_all_missiles:
		call redraw_cities
		call redraw_crosshairs
        popl %edi
        popl %esi
        popl %ebx
        leave 
		ret



# int mp1_ioctl(unsigned long arg, unsigned int cmd)
# The dispatch function for the MP1 ioctls - should use the cmd argument
# and a jumptable to execute one of the specific ioctls implemented below.
# Inputs   : unsigned long arg - parameter to the mp1_ioctl_....
#          : unsigned int cmd  - specifies which mp1_ioctl_... function 
#          :                     to execute
# Outputs  : Returns an integer - depends on which ioctl() function is called
# Registers: Standard C calling convention

.globl mp1_ioctl
mp1_ioctl:
		movl 8(%esp), %esi
		cmpl $4, %esi

        cmpl $0, %esi
        jb not_valid
        cmpl $4, %esi
        ja not_valid

        jmp *jump_tables(, %esi, 4) 


not_valid:
        movl $-1, %eax
        jmp done

jump_tables:
.long mp1_ioctl_startgame, mp1_ioctl_addmissile, mp1_ioctl_movexhairs, mp1_ioctl_getstatus, mp1_ioctl_endgame, mp1_missile_remove


# ----------------- Functions private to this file -------------------

# This function updates missile
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention

update_missile:     
		movl X(%edx), %esi
        shrl $16, %esi                   #x and y positions to be erased
		movl Y(%edx), %edi
        shrl $16, %edi
        pushl %esi
        pushl %edi               
		call erase_from_screen
		addl $8, %esp
        movl VX(%edx), %ebx           
        addl %ebx, X(%edx)
        movl VY(%edx), %ebx           
        addl %ebx, Y(%edx)
        movl X(%edx), %esi
        shrl $16, %esi                   
        movl Y(%edx), %edi
        shrl $16, %edi
        cmpl $79, %esi             #check if x and y coordinates are within range
        jg only_remove_missile
        cmpl $0, %esi
        jl only_remove_missile
        cmpl $0, %edi
        jl only_remove_missile
        cmpl $24, %edi
        jg only_remove_missile

check_reached_dest:
        cmpl DEST_X(%edx), %esi
        je check_reached_dest_Y
        pushl %edx
        call draw_missile                #if not draw missile
        popl %edx
        movl NEXT(%edx), %edx
        jmp go_to_next_missile

check_reached_dest_Y:
        cmpl DEST_Y(%edx), %edi
        je missile_exploding                 #if reached dest, set EXPLODED
        pushl %edx
        call draw_missile                #if not draw missile
        popl %edx
        movl NEXT(%edx), %edx
        jmp go_to_next_missile


missile_exploding:
        pushl %edx
		call missile_explode
        popl %edx
        cmpl $0, %eax
        je skip_notify

        pushl %eax
        pushl %ecx
        pushl %edx
        call mp1_notify_user
        popl %edx
        popl %ecx
        popl %eax

skip_notify:
        pushl %edx
        call draw_explosion                #draw explosion
        popl %edx
		addl $-1, EXPLODED(%edx)
		cmpl $0, EXPLODED(%edx)
		jbe remove_missile
        movl NEXT(%edx), %edx
		jmp go_to_next_missile

remove_missile:
		movl X(%edx), %esi
        shrl $16, %esi                   
        movl Y(%edx), %edi
        shrl $16, %edi
        pushl %esi
        pushl %edi               
		call erase_from_screen
		addl $8, %esp

only_remove_missile:
        pushl %edx
        call mp1_missile_remove
        addl $4, %esp
        jmp go_to_next_missile


# This function remove a missile from the list
# Inputs   : *curr_missile
# Outputs  : none
# Registers: Standard C calling convention
mp1_missile_remove:                        #void mp1_missile_remove(struct missile *curr_missile)
		pushl %ebp
		movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi


        movl 8(%ebp), %edx                 #edx = current missile
        movl NEXT(%edx), %ebx              #save next missile

search_pointer:
        leal mp1_missile_list, %ecx
loop:
        cmpl NEXT(%ecx), %edx
        je found_prev
        movl NEXT(%ecx), %ecx
        jmp loop

found_prev:
        movl %ebx, NEXT(%ecx)

free_missile:

        pushl %edx   					   #remove current missile structure
        call mp1_free
        addl $4, %esp

        movl %ebx, %edx
		popl %edi
        popl %esi
        popl %ebx
        leave 
		ret


go_to_next_missile:
		jmp next_missile

draw_explosion:
		pushl %ebp
		movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi

        movl 8(%ebp), %edx
		movl X(%edx), %esi
        shrl $16, %esi
        movl Y(%edx), %ebx
        shrl $16, %ebx
        imull $160, %ebx, %ebx
        leal (,%esi, 2), %esi
        leal (%esi, %ebx), %eax
        movb $EXPLOSION, %cl
        pushl %edx 
        call mp1_poke
        popl %edx

        popl %edi
        popl %esi
        popl %ebx
        leave 
		ret

draw_missile:
		pushl %ebp
		movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi

        movl 8(%ebp), %edx
		movl X(%edx), %esi
        shrl $16, %esi
        movl Y(%edx), %ebx
        shrl $16, %ebx
        imull $160, %ebx, %ebx
        leal (,%esi, 2), %esi
        leal (%esi, %ebx), %eax
        movb C(%edx), %cl
        pushl %edx 
        call mp1_poke
        popl %edx

		popl %edi
        popl %esi
        popl %ebx
        leave 
		ret

# This function erases the text-mode pos
# Inputs   : x_text_pos
#             y_text_pos
# Outputs  : none
# Registers: Standard C calling convention
erase_from_screen:                          #void erase_from_screen(int x_text_pos, int y_text_pos)
		pushl %ebp
		movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi

        movl 12(%ebp), %esi
        movl 8(%ebp), %ebx
        imull $160, %ebx, %ebx
        leal (,%esi, 2), %esi
        leal (%esi, %ebx), %eax
        movb $0x20, %cl
        pushl %edx
        call mp1_poke
        popl %edx 

        popl %edi
        popl %esi
        popl %ebx
        leave 
		ret





# Initialize crosshair position and base_alive 
# and corss hair at the beginning of the game
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention
mp1_ioctl_startgame:
		pushl %ebp
        movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi

        movl $40, crosshairs_x
        movl $12, crosshairs_y
        movl $0x00010101, base_alive

		jmp done


# This ioctl must add a new missile to the game and will copy a user space missile to kernel
# Inputs   : one argument, points to the missile in user space
# Outputs  : return 0 if copy succeeds and -1 if fails
# Registers: Standard C calling convention
mp1_ioctl_addmissile:
		pushl %ebp
        movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi

        movl 8(%ebp), %ebx     #save argument to ebx
        
        movl $33, %esi
        pushl %esi
        call mp1_malloc        #allocate memory for copy
        addl $4, %esp

        cmpl $0, %eax
        je fail_to_alloc       #return -1 if fail to alloc
        movl %eax, %edi         #else edi = new memory location

        pushl %esi
        pushl %ebx
        pushl %edi 
        call mp1_copy_from_user      #copy from user from ebx to esi
        addl $12, %esp

        cmpl $0, %eax
        jne fail_to_copy        #return -1 if fail to copy
        je add_to_list

fail_to_copy:
        pushl %edi
        call mp1_free
        addl $4, %esp

fail_to_alloc:
        movl $-1, %eax
        jmp return_

add_to_list:
        movl mp1_missile_list, %ebx
        movl %ebx, NEXT(%edi)
        movl %edi, mp1_missile_list
        movl $0, %eax                     #return 0

return_:
        jmp done




# This function moves the crosshairs properly within the game screen
# Inputs   : one argument, Vx_xhair in low 16 bits and Vy_xhair in high 16 bits
# Outputs  : return 0 and never fails
# Registers: Standard C calling convention
mp1_ioctl_movexhairs:
		pushl %ebp
        movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi

        pushl crosshairs_x                   #x and y positions to be erased
		pushl crosshairs_y              
		call erase_from_screen
		addl $8, %esp

        movl 8(%ebp), %ebx
        addw %bx, crosshairs_x
        cmpw $0, crosshairs_x                 #maintain the crosshair in the game screen
        jl x_below_0
        cmpw $79, crosshairs_x
        ja x_above_79
        jmp y_compare

x_below_0:
        movw $0, crosshairs_x
        jmp y_compare
x_above_79:
        movw $79, crosshairs_x 

y_compare:
        shrl $16, %ebx
        addw %bx, crosshairs_y
        cmpw $0, crosshairs_y
        jl y_below_0
        cmpw $24, crosshairs_y
        ja y_above_24
        jmp done_movexhairs

y_below_0:
        movw $0, crosshairs_y
        jmp done_movexhairs
y_above_24:
        movw $24, crosshairs_y

done_movexhairs: 
        jmp done



# This function allows the user to retrieve the current score and the status of the three cities
# Inputs   : one argument, pointer to user space
# Outputs  : return 0 if copy succeeds and -1 if fails
# Registers: Standard C calling convention
mp1_ioctl_getstatus:
   		pushl %ebp
        movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi

        movl 8(%ebp), %ebx                   #ebx = pointer to a 32-bit integer in user space
        movl mp1_score, %ecx                 #ecx = mp1_score
check_first:
		movl $0x000000ff, %esi               #check base_alive
		andl base_alive, %esi
		cmpl $0, %esi
        jne first_non_zero
        je check_second
first_non_zero:
        addl $0x00010000, %ecx
check_second:
        movl $0x0000ff00, %esi 
		andl base_alive, %esi
		cmpl $0, %esi
        jne second_non_zero
        je check_third
second_non_zero:
        addl $0x00020000, %ecx
check_third:
        movl $0x00ff0000, %esi 
		andl base_alive, %esi
		cmpl $0, %esi
        jne third_non_zero
        je done_all_three
third_non_zero:
        addl $0x00040000, %ecx

done_all_three:
		movl %ecx, score_tobe_copied                    #copy the score to user
		pushl $4
		pushl $score_tobe_copied
		pushl %ebx
		call mp1_copy_to_user
		addl $12, %esp
		cmpl $0, %eax	
		je done
		movl $-1, %eax
        jmp done


# Called when the game is over, this function must perform all the cleanup work
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention
mp1_ioctl_endgame:
		pushl %ebp
        movl %esp, %ebp
        pushl %ebx
        pushl %esi
        pushl %edi
        
remove_head:
        movl mp1_missile_list, %ebx                  #clean the list
		cmpl $0, %ebx
        je no_component_list
        movl NEXT(%ebx), %esi
        movl %esi, mp1_missile_list
        pushl %ebx
        call mp1_free
        addl $4, %esp
        jmp remove_head

no_component_list:
		movl $1, %eax

done:
        popl %edi
        popl %esi
        popl %ebx
        leave 
        ret




# This function redraws the crosshair
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention
redraw_crosshairs:
		movl crosshairs_y, %eax
		imull $80, %eax
		addl crosshairs_x, %eax
		addl %eax, %eax
		pushl %ecx 
		movb $43, %cl             #draw '+' for the xhair
		call mp1_poke
		popl %ecx
		ret



# This function redraws the cities in proper state
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention
redraw_cities:
		movl $18, %ecx
        movl	base_alive, %edx
		andl $0x000000ff, %edx
		cmpl $0, %edx
		je draw_first_destroyed_city
		call draw_city
		jmp	 draw_second_city
draw_first_destroyed_city:
		call draw_destroyed_city
draw_second_city:
		movl $38, %ecx
		movl base_alive, %edx
		andl $0x0000ff00, %edx
		cmpl $0, %edx
		je draw_second_destroyed_city
		call draw_city
		jmp	draw_third_city
draw_second_destroyed_city:
		call draw_destroyed_city
draw_third_city:
		movl $58, %ecx
		movl base_alive, %edx
		andl $0x00ff0000, %edx
		cmpl $0, %edx
		jne draw_third_good_city
		call draw_destroyed_city
		ret
draw_third_good_city:
		call	draw_city
		ret


# This function is draw the alive city on the screen 
# Inputs   : %ecx, offset to an alive city
# Outputs  : none
# Registers: Standard C calling convention
draw_city:

		pushl %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi

		leal base_pic, %esi

		movl $24, %eax
		imull $80, %eax
		addl %ecx, %eax
		addl %eax, %eax

		movb 0(%esi), %cl                   #draw all characters
		call mp1_poke
		addl $2, %eax
		movb 1(%esi), %cl
		call mp1_poke
		addl $2, %eax
		movb 2(%esi), %cl
		call mp1_poke
		addl $2, %eax
		movb 3(%esi), %cl
		call mp1_poke
		addl $2, %eax
		movb 4(%esi), %cl
		call mp1_poke
\
		popl %edi
		popl %esi
		popl %ebx
		leave
		ret		



# This function is draw the destroyed city on the screen 
# Inputs   : %ecx, offset to an alive city
# Outputs  : none
# Registers: Standard C calling convention
draw_destroyed_city:
        push %ebp
		movl %esp, %ebp
		pushl %ebx
		pushl %esi
		pushl %edi

		leal dead_base_pic, %esi

		movl $24, %eax
		imull $80, %eax
		addl %ecx, %eax
		addl %eax, %eax

		movb 0(%esi), %cl                   #draw all characters
		call mp1_poke
		addl $2, %eax
		movb  1(%esi), %cl
		call mp1_poke
		addl $2, %eax
		movb 2(%esi), %cl
		call mp1_poke
		addl $2, %eax
		movb 3(%esi), %cl
		call mp1_poke
		addl $2, %eax
		movb 4(%esi), %cl
		call mp1_poke

		popl %edi
		popl %esi
		popl %ebx
		leave
		ret



