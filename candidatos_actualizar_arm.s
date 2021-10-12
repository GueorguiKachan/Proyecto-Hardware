	AREA datos,DATA
init_region	DCB	0,0,0,3,3,3,6,6,6
		SPACE 3
num_filas EQU 9
num_columnas EQU 9
mascara_quitar_candidatos_15_0 EQU 0x0000003F
mascara_quitar_candidatos_31_16 EQU 0x003f0000
salto_columna EQU 1
eliminar_candidato EQU 6

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
			
	AREA codigo,CODE
	EXPORT candidatos_actualizar_arm
		
candidatos_actualizar_arm
					;r0-> @ inicio de la cuadricula
					;r1-> i
					;r2-> j
					;r7-> celdas_vacias
					;r8-> valor de la celda pero se puede modificar
					;r3,r8,r9,r10,r11,r4,r5,r6
					
	STMDB R13!,{R4-R10,lr} 			;Se guardan los valores de los registros r4 a r8 y el link register 
	MOV R6,R0 						;En r6 se guarda la dirección inicial de la cuadrícula?
	MOV R7,#0 						;CELDAS_VACIAS=0
	MOV R4,#0 						;i=0

ini_bucle1
	CMP R4,#num_filas
	BEQ fin_bucle1
	MOV r5,#0 						;j=0
	
nueva_iter_j
	CMP R5,#num_columnas			;r4=i, r5=j, r6=@inicio_cuadricula, r0=valor_celda, r2=@celda, r7=celdas_vacias
	ADDGE r4,r4,#1
	BGE ini_bucle1
	;ADDEQ r4,r4,#1
	;BEQ ini_bucle1
	
	ADD R2,R6,R4,LSL #salto_fila 		
	ADD R2,R2,R5,LSL #salto_columna
	;LDRH R0,[R2] 					;r2 = valor de la dirección de la celda que toca. Carga half-word.	
	LDR R0,[R2]						;r0 = valor celda 4B
	
	;MOV r8,r0,LSR #16
	AND r8,r0,#mascara_quitar_candidatos_31_16 	;Hace and del valor con F y lo guarda en r2.
	AND r0,r0,#mascara_quitar_candidatos_15_0
	;MOV r8,r8,LSL #dividir_entre_2e16
	add r0,r0,r8
	;AND r0,r0,#0x003F0000
	str r0,[r2]
	;STRH r0,[r2] 		
	
	
	;ADD r5,r5,#1
	ADD r5,r5,#2
	B nueva_iter_j
	
fin_bucle1
	MOV r4,#0
	MOV r5,#0
	
ini_bucle2
	CMP r4,#num_filas
	BEQ fin_bucles
	MOV r5,#0
	
nueva_iter_j2	;r0=valor_celda r1=@celda
	;mov r10, #0
	CMP r5,#num_columnas
	ADDGE r4,r4,#1
	BGE ini_bucle2
	;ADDEQ r4,r4,#1
	;BEQ ini_bucle2
	
	ADD r1,r6,r4,LSL #salto_fila 
	ADD r1,r1,r5,LSL #salto_columna	
	;LDRH r0,[r1] 					
	LDR r0,[r1]
	;mov r8,r0,LSR #16
	AND r8,r0,#mascara_valor_2
	AND r1,r0,#mascara_valor_1 		;Se obtiene el valor de la celda
	AND r0,r0,#mascara_valor_1
	mov r10,#1
	CMP r0,#0
	ADDEQ r7,r7,#1 					;Si es igual a 0 no se propaga nada y se suma uno
	ADDEQ r5,r5,#1
	;BEQ nueva_iter_j2
	BEQ compr_seg_num
	
	MOV r0,r6 						;Primer parámetro de la función es la @ inicial de la cuadrícula
	MOV r2,r5 						;Se prepara para llamar a la funcion candidatos_propagar
	MOV r5,r1						;Esto antes no estaba
	MOV r1,r4					
	;Empieza propagar
	bl prop
	
	MOV r5,r2						
	MOV r4,r1
	MOV r6,r0						

	add R5,R5,#1
	and r5,r5,#0x000000FF
;----Nuevo------------
	cmp r10,#2
	BEQ nueva_iter_j2
	
compr_seg_num
	
	cmp r5,#9
	addeq r4,r4,#1
	beq ini_bucle2
	
	mov r8,r8,lsr#dividir_entre_2e16
	CMP r8,#0
	ADDEQ r7,r7,#1 					;Si es igual a 0 no se propaga nada y se suma uno
	ADDEQ r5,r5,#1
	BEQ nueva_iter_j2
;----Nuevo------------
	add r10,r10,#1
	
	MOV r0,r6 						;Primer parámetro de la función es la @ inicial de la cuadrícula
	MOV r2,r5 						;Se prepara para llamar a la funcion candidatos_propagar
	MOV r5,r8						;Esto antes no estaba
	MOV r1,r4
	
	bl prop
	add R5,R5,#1
	and r5,r5,#0x000000FF
	B nueva_iter_j2 				;Salta siempre, igual que en primer bucle pero de forma distinta
	
fin_bucles
	MOV r0,r7
	LDMIA r13!,{r4-r10,lr}
	BX r14
	
prop ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	push{r1-r10,lr}		;-r2=end_i
						;-r3=@init_region
						;-r4=@cuadricula
						;-r5=valor_celda_15:0
						;-r6=i
						;-r7=j
						;-r8=celda_completa
						;-r9=bit_mascara
						;-r10=valor_celda2_31:16
	
	add r5,r5,#eliminar_candidato_15_0 	
	mov r9, #bit_mascara			;r9 contiene la cte 1 necesaria para eliminar candidatos
	add r10,r5,#eliminar_candidato_31_16
	
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
fin	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop{r1-r10}
	pop{pc}
	

	END
