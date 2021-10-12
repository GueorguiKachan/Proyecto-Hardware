AREA datos,DATA
num_filas EQU 9
num_columnas EQU 9
mascara_valor EQU 0x0000000F
mascara_quitar_candidatos EQU 0x0000003F
mas EQU 0x003F003F
salto_fila EQU 5
salto_columna EQU 1

			
	AREA codigo,CODE
	EXPORT candidatos_actualizar_arm_c
	IMPORT candidatos_propagar_c	
candidatos_actualizar_arm_c
					;r0-> @ inicio de la cuadricula
					;r1-> i
					;r2-> j
					;r7-> celdas_vacias
					;r8-> valor de la celda pero se puede modificar
					;r3,r8,r9,r10,r11,r4,r5,r6
	STMDB R13!,{R4-R8,lr} 			;Se guardan los valores de los registros r4 a r8 y el link register 
	MOV R6,R0 						;En r6 se guarda la dirección inicial de la cuadrícula?
	MOV R7,#0 						;CELDAS_VACIAS=0
	MOV R4,#0 						;i=0

ini_bucle1
	CMP R4,#num_filas
	BEQ fin_bucle1
	MOV r5,#0 						;j=0
	
nueva_iter_j
	CMP R5,#num_columnas					;r4=i, r5=j, r6=@inicio_cuadricula, r0=valor_celda, r2=@celda, r7=celdas_vacias
	ADDGE r4,r4,#1
	BGE ini_bucle1
	
	ADD R2,R6,R4,LSL #salto_fila 		
	ADD R2,R2,R5,LSL #salto_columna
	;LDRH R0,[R2] 							;r2 = valor de la dirección de la celda que toca. Carga half-word.	
	ldr r0,[r2]								; r0 = 2 celdas
	MOV r8,r0,LSR #16
	AND r0,r0,#0x000003F 	;Hace and del valor con F y lo guarda en r2.
	AND r8,r8,#0x000003F
	MOV r8,r8,LSL #16
	add r0,r0,r8
	;AND r0,r0,#0x003F0000
	str r0,[r2]
	;STRH r0,[r2] 					
	
	add r5,r5,#2
	;ADD r5,r5,#1
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
	ADDGE r4,r4,#1
	BGE ini_bucle2
	
	ADD r1,r6,r4,LSL #salto_fila 
	ADD r1,r1,r5,LSL #salto_columna	
	;LDRH r0,[r1] 					
	LDR r0,[r1]						; r0= 2 valores de celdas
	mov r8,r0,LSR #16
	AND r8,r8,#mascara_valor
	AND r1,r0,#mascara_valor		;Se obtiene el valor de la celda
	CMP r1,#0
	ADDEQ r7,r7,#1 					;Si es igual a 0 no se propaga nada y se suma uno
	ADDEQ r5,r5,#1
	;BEQ nueva_iter_j2
	BEQ compr_segundo_num
	
	MOV r2,r5 							;Se prepara para llamar a la funcion candidatos_propagar
	MOV r1,r4
	MOV r0,r6 							;Primer parámetro de la función es la @ inicial de la cuadrícula
	BL candidatos_propagar_c 			;r14 será la dirección de retorno
	
;-----Nuevo----------		
	add r5,r5,#1
	and r5,r5,#0x000000FF
compr_segundo_num

	CMP r5,#9
	addeq r4,r4,#1
	BEQ ini_bucle2
	CMP r8,#0
	ADDEQ r7,r7,#1 					;Si es igual a 0 no se propaga nada y se suma uno
	ADDEQ r5,r5,#1
	BEQ nueva_iter_j2
	
	MOV r2,r5 							;Se prepara para llamar a la funcion candidatos_propagar
	MOV r1,r4
	MOV r0,r6 							;Primer parámetro de la función es la @ inicial de la cuadrícula
	BL candidatos_propagar_c 			;r14 será la dirección de retorno
;-----Nuevo---------
	
	add R5,R5,#1
	and r5,r5,#0x000000FF
	B nueva_iter_j2 					;Salta siempre, igual que en primer bucle pero de forma distinta
	
fin_bucles
	MOV r0,r7
	LDMIA r13!,{r4-r8,r14}
	BX r14

	END
