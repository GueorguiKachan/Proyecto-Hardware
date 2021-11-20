	else if(aux.ID_evento == pulsacion1){
				if (eint1_comprobar()){ //si se ha dejado de pulsar
					if(leerFila() == 0 && leerColumna()== 0 && leerValorNuevo() == 0){
						//Fin de la partida
					}
					if (!powerDownActive){
						//se comprueba que se pueda escribir en la celda
						//q no sea pista, q el valor este entre los candidatos, y que la columna, fila y valor introducidos sean validos
						int filaAct =  leerFila() ;
						int colAct = leerColumna();
						int valorNuevo = leerValorNuevo();
						int esPista = (cuadricula_C_C[filaAct-1][colAct-1] >> 4) & 0x1;
						bool esCandidato = false;
						int candidatos = cuadricula_C_C[filaAct-1][colAct-1] >> 7;
						int i;
						int uno;
						for(i=1;i<9;i++){
							uno = candidatos &0x1;
							if(uno==1 && valorNuevo == i){
								esCandidato = true;
							}
							candidatos = candidatos >> 1;
						}
						if(filaAct <= filaColumnaSudoku && colAct <= filaColumnaSudoku && 0 < valorNuevo && valorNuevo <= filaColumnaSudoku && esPista ==0 && esCandidato){
							cuadricula_C_C[filaAct-1][colAct-1] =  (cuadricula_C_C[filaAct-1][colAct-1] & 0xfff0) + valorNuevo;
							temporizador_iniciar();
							temporizador_empezar();
							candidatos_actualizar_c(cuadricula_C_C);
							tiempo = temporizador_parar();
							valorCambiado=true;
							escribirError(1);
							idleCambiado = true;
							cola_guardar_eventos(alarmaSet,apagarVal);
							
						}
					}else{ powerDownActive = false; escribirIdle(0);}
					cola_guardar_eventos(alarmaSet,eventoPD);
					
				}
			}


//Version Ernesto
else if(aux.ID_evento == pulsacion1){
				if (eint1_comprobar()){ //si se ha dejado de pulsar
					if (!powerDownActive){
						int filaAct =  leerFila() ;
						int colAct = leerColumna();
						int valorNuevo = leerValorNuevo();
						int esPista = (cuadricula_C_C[filaAct-1][colAct-1]>>4) &0x1;
						bool esCandidato = false;
						int candidatos = cuadricula_C_C[filaAct-1][colAct-1]>>7;
						int i;
						int uno;
						if(filaAct==0 && colAct==0 && valorNuevo==0){
							
						}
						if (valorNuevo <= 9 && valorNuevo >= 1){
							candidatos = candidatos >> (valorNuevo -1);
							esCandidato = 1 == (candidatos &0x1);
						}
						
						//se comprueba que se pueda escribir en la celda
						//q no sea pista, q el valor este entre los candidatos, y que la columna, fila y valor introducidos sean validos
						
						if(filaAct <= filaColumnaSudoku && colAct <= filaColumnaSudoku && 0 < valorNuevo && valorNuevo <= filaColumnaSudoku && esCandidato && esPista == 0){
						cuadricula_C_C[filaAct-1][colAct-1] =  (cuadricula_C_C[filaAct-1][colAct-1] & 0xfff0) + valorNuevo;
						temporizador_iniciar();
						temporizador_empezar();
						candidatos_actualizar_c(cuadricula_C_C);
						tiempo = temporizador_parar();
						valorCambiado=true;
						escribirError(1);
						idleCambiado = true;
						cola_guardar_eventos(alarmaSet,apagarVal);}
					}else{ powerDownActive = false; escribirIdle(0);}
					cola_guardar_eventos(alarmaSet,eventoPD);
				}
