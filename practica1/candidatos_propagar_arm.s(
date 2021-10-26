	AREA datos,DATA
init_region	DCB	0,0,0,3,3,3,6,6,6
mascara_valor_1	EQU	0x0000000F
mascara_valor_2 EQU 0x000f0000
eliminar_candidato_15_0	EQU 6
eliminar_candidato_31_16	EQU 16
bit_mascara	EQU 1
NUM_FILAS	EQU 9
bytes_siguiente_columna	EQU 4
bytes_siguiente_columna_half	EQU	2
bytes_siguiente_fila	EQU 32
salto_columna_half	EQU	1
salto_columna_word	EQU 2
salto_fila	EQU	5
dividir_entre_2e16	EQU	16
lado_cuadrado	EQU	3
	SPACE 3

	AREA codigo,CODE,READONLY
	EXPORT candidatos_propagar_arm

candidatos_propagar_arm 
						;-r2=end_i
						;-r3=@init_region
						;-r4=@cuadricula
						;-r5=valor_celda_15:0
						;-r6=i
						;-r7=j
						;-r8=celda_completa
						;-r9=bit_mascara
						;-r10=valor_celda2_31:16
	push{r2-r10,lr}	
	mov r4,r0 
	add r4,r4, r2, lsl#salto_columna_half
	add r4,r4,r1,lsl#salto_fila
	ldrh r5,[r4] 
	and r5,r5,#mascara_valor_1 
	add r5,r5,#eliminar_candidato_15_0 
	add r10,r5,#eliminar_candidato_31_16
	mov r9, #bit_mascara ;r9 contiene la cte 1 necesaria para eliminar candidatos
	
	;propagar misma fila
	mov r6,#0
	mov r4,r0 
	add r4,r4,r1, lsl#salto_fila
filas	add r6,r6,#2
	cmp r6,#NUM_FILAS 
	bge ultimo
	ldr r8,[r4]
	orr r8,r9,lsl r5
	orr r8,r9,lsl r10
	str r8,[r4],#bytes_siguiente_columna
	b	filas
ultimo
	ldrh r8,[r4]
	orr r8,r9,lsl r5
	strh r8,[r4],#bytes_siguiente_fila
	
	;propagar misma columna
siguiente	mov r6,#0
	mov r4,r0 
	add r4,r2, lsl#salto_columna_half 
columnas	cmp r6,#NUM_FILAS 
	bge siguiente2
	ldrh r8,[r4]
	orr r8,r9,lsl r5
	strh r8,[r4],#bytes_siguiente_fila
	add r6,r6,#1
	b columnas
	
	;propagar cuadrado
siguiente2
	ldr r3,=init_region ;r3 es el inicio de init_region
	ldrb r6, [r3,r1] ;si es 3 hay q hacer half y word,sino word y half
	ldrb r7, [r3,r2] 
	
	
	add r2,r6,#lado_cuadrado
inicio_bucle	
	cmp r2,r6
	beq fin
	mov r4,r0
	add r4,r4,r6, lsl#salto_fila 
	add r4,r4,r7, lsl#salto_columna_half

	cmp r7,#3
	bne primero_word
	ldrh r8,[r4]
	orr r8,r9,lsl r5
	strh r8,[r4],#bytes_siguiente_columna_half
primero_word	
	ldr r8,[r4]
	orr r8,r9,lsl r5
	orr r8,r9,lsl r10
	str r8,[r4],#bytes_siguiente_columna
	beq sumar_fila
	ldrh r8,[r4]
	orr r8,r9,lsl r5
	strh r8,[r4]
	
	
	
	
sumar_fila	add r6,r6,#1
	b inicio_bucle
	

fin	pop{r2-r10}
	pop{pc}
	
	END
