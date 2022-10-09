	.arch armv7-a
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	push	{fp, lr}
	mov	fp, sp
	sub	sp, sp, #32
	mov	r4, #0
	str	r4, [fp, #-32]           @i=0   
	bl	getint(PLT)
	str	r0, [fp, #-28]           @ans=getint()
.L2:   
	ldr	r4, [fp, #-32]
	cmp	r4, #5                   @i<5    blt
	bge	.L3       
	ldr	r4, [fp, #-32]
	add	r2, r4, #1               @r2=i+1
	ldr	r4, [fp, #-32]
	lsl	r4, r4, #2               @4i  
	add	r4, r4, fp
	str	r2, [r4, #-24]           @a addr
	ldr	r4, [fp, #-32]
	lsl	r4, r4, #2      
	add	r4, r4, fp
	ldr	r4, [r4, #-24]
	ldr	r2, [fp, #-28]
	orr	r4, r4, r2               @| instruction 
	str	r4, [fp, #-28]
	ldr	r4, [fp, #-32]
	add	r4, r4, #1     
	str	r4, [fp, #-32]           @i++ 
	b  	.L2
.L3:               
	ldr	r0, [fp, #-28]           @ans=putint()
	bl	putint(PLT)
.L4:
	mov	sp, fp
	pop	{fp, pc}

	.section	.note.GNU-stack,"",%progbits
