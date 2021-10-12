	AREA datos,DATA
init_region	DCB	0,0,0,3,3,3,6,6,6
		SPACE 3
num_filas EQU 9
num_columnas EQU 9
mascara_valor EQU 0x0000000F
mascara_quitar_candidatos EQU 0x0000003F
salto_fila EQU 5
salto_columna EQU 1
eliminar_candidato EQU 6
bytes_siguiente_columna	EQU 2
bytes_siguiente_fila	EQU 32
bit_mascara	EQU 1
	
			
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
	ADDEQ r4,r4,#1
	BEQ ini_bucle1
	
	ADD R2,R6,R4,LSL #salto_fila 		
	ADD R2,R2,R5,LSL #salto_columna
	LDRH R0,[R2] 					;r2 = valor de la dirección de la celda que toca. Carga half-word.	
	AND r0,r0,#mascara_quitar_candidatos 		;Hace and del valor con F y lo guarda en r2.
	STRH r0,[r2] 					;Guarda el half-word de la celda sin candidatos que hay en r0 en la @ de r2.
	
	ADD r5,r5,#1
	B nueva_iter_j
	
fin_bucle1
	MOV r4,#0
	MOV r5,#0
	
ini_bucle2
	CMP r4,#num_filas
	BEQ fin_bucles
	MOV r5,#0
	
nueva_iter_j2						;r0=valor_celda r1=@celda
	CMP r5,#num_columnas
	ADDEQ r4,r4,#1
	BEQ ini_bucle2
	
	ADD r1,r6,r4,LSL #salto_fila 
	ADD r1,r1,r5,LSL #salto_columna	
	LDRH r0,[r1] 					
	AND r1,r0,#mascara_valor 		;Se obtiene el valor de la celda
	AND r0,r0,#mascara_valor
	CMP r0,#0
	ADDEQ r7,r7,#1 					;Si es igual a 0 no se propaga nada y se suma uno
	ADDEQ r5,r5,#1
	BEQ nueva_iter_j2
	
	MOV r0,r6 						;Primer parámetro de la función es la @ inicial de la cuadrícula
	MOV r2,r5 						;Se prepara para llamar a la funcion candidatos_propagar
	MOV r5,r1						;Esto antes no estaba
	MOV r1,r4					
	;Empieza propagar
	
	push{r1,r2,r7}					;r1=i, r2=j, r4=@celda, r5=valor_celda
	
	add r5,r5,#eliminar_candidato 	;r5 contiene el valor q hace falta para eliminar candidatos
	mov r9, #bit_mascara			;r9 contiene la cte 1 necesaria para eliminar candidatos
	
	mov r6,#0
	mov r4,r0 						;ponemos en r4 el valor de cuadricula
	add r4,r4,r1, lsl#salto_fila	;preparamos la cuadricula con la fila 
	
filas	cmp r6,#num_filas			;i<N_FILAS
	beq siguiente
	ldrh r8,[r4]
	orr r8,r9,lsl r5
	strh r8,[r4],#bytes_siguiente_columna
	add r6,r6,#1
	b	filas
	
siguiente	mov r6,#0
	mov r4,r0 						;ponemos en r4 el valor de cuadricula
	add r4,r2, lsl#salto_columna 	;preparamos la cuadricula con la columna
	
columnas	cmp r6,#num_filas		;i<N_FILAS
	beq siguiente2
	ldrh r8,[r4]
	orr r8,r9,lsl r5
	strh r8,[r4],#bytes_siguiente_fila
	add r6,r6,#1
	b columnas
	
siguiente2	
	mov r4,r0
	ldr r3,=init_region 			
	ldrb r6, [r3,r1] 				;r6 <- filaaux
	ldrb r7, [r3,r2] 				;r7 <- columnaaux
	add r4,r4,r6, lsl#salto_fila 	;preparamos la cuadricula en r4
	add r4,r4,r7, lsl#salto_columna
	add r2,r7, #3 					;r2<- para comparar con columnaaux
	add r1,r6,#3 					;r1<- para comparar con filaaux
	
inicioiter	cmp r2, r7
	beq end_columna
	cmp r1, r6
	beq end_fila
	ldrh r8,[r4]
	orr r8,r9,lsl r5
	strh r8,[r4],#bytes_siguiente_fila
	add r6, r6, #1
	b inicioiter
	
end_fila	mov r4,r0
	sub r6,r6, #3
	add r4,r4,r6, lsl#salto_fila
	add r7,r7,#1
	add r4,r4,r7, lsl#salto_columna
	b inicioiter
	
end_columna
	pop{r1,r2,r7}
	
	MOV r5,r2						
	MOV r4,r1
	MOV r6,r0						
	
	add R5,R5,#1
	and r5,r5,#0x000000FF
	B nueva_iter_j2 				;Salta siempre, igual que en primer bucle pero de forma distinta
	
fin_bucles
	MOV r0,r7
	LDMIA r13!,{r4-r10,lr}
	BX r14

	END

