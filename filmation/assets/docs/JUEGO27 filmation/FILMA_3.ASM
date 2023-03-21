
_TEXT           SEGMENT BYTE PUBLIC 'CODE'

        ASSUME  CS:_TEXT

                PUBLIC  DIR_MEM_TECLA

DIR_MEM_TECLA   DD      0

                PUBLIC  VECTOR_TECLADO

VECTOR_TECLADO  DD      0

                PUBLIC  GUARDA_VECTORES

GUARDA_VECTORES         PROC    NEAR
                PUSH    AX
                PUSH    BX
                PUSH    ES
                MOV     AH,35h          ;Guarda el vector del teclado.
                MOV     AL,09h
                INT     21h
                MOV     WORD PTR CS:[VECTOR_TECLADO],BX
                MOV     WORD PTR CS:[VECTOR_TECLADO+2],ES
                POP     ES
                POP     BX
                POP     AX
                RET
GUARDA_VECTORES         ENDP

                PUBLIC  _SET_VECTORES

_SET_VECTORES   PROC    NEAR
                CALL    GUARDA_VECTORES
                PUSH    BP
                MOV     BP,SP
                PUSH    AX
                PUSH    DS
                PUSH    DX
                PUSH    SI

                LDS     SI,[BP+4]
                MOV     WORD PTR CS:[DIR_MEM_TECLA],SI
                MOV     WORD PTR CS:[DIR_MEM_TECLA+2],DS

                MOV     DX,OFFSET CS:TECLADO
                MOV     AX,_TEXT
                MOV     DS,AX
                MOV     AL,09h
                MOV     AH,25h
                INT     21h             ;Cambia el vector del teclado.

                POP     SI
                POP     DX
                POP     DS
                POP     AX
                POP     BP
                RET
_SET_VECTORES   ENDP

                PUBLIC  _UNSET_VECTORES

_UNSET_VECTORES PROC    NEAR
                PUSH    BP
                MOV     BP,SP
                PUSH    AX
                PUSH    DS
                PUSH    DX

                MOV     DX,WORD PTR CS:[VECTOR_TECLADO]
                MOV     AX,WORD PTR CS:[VECTOR_TECLADO+2]
                MOV     DS,AX
                MOV     AL,09h
                MOV     AH,25h
                INT     21h             ;Repone el vector del teclado.

                POP     DX
                POP     DS
                POP     AX
                POP     BP
                RET
_UNSET_VECTORES ENDP

                PUBLIC  TECLADO

TECLADO         PROC    FAR
                PUSH    AX
                PUSH    BX
                PUSH    DS
                PUSH    SI
                LDS     SI,CS:[DIR_MEM_TECLA]
                IN      AL,60h
                MOV     AH,AL
                IN      AL,61h
                OR      AL,10000000b
                OUT     61h,AL
                AND     AL,01111111b
                OUT     61h,AL
                MOV     AL,AH
                MOV     BL,AL
                MOV     BH,00h
                AND     BX,007Fh
                CMP     AL,00h
                JE      TECLEADO
                CMP     AL,0E0h
                JE      TECLEADO
                CMP     AL,0F0h
                JE      TECLEADO
                CMP     BYTE PTR [SI],0E0h      ;£ltima tecla
                JNE     NO_DOBLE
                ADD     BX,128
NO_DOBLE:       TEST    AL,80h
                JZ      NO_SOLT
                MOV     BYTE PTR [BX+SI],0
                JMP     TECLEADO
NO_SOLT:        MOV     BYTE PTR [BX+SI],1
TECLEADO:       MOV     [SI],AL                 ;£ltima tecla
                LDS     SI,CS:[DIR_MEM_TECLA]
                MOV     BYTE PTR [SI+84],0
                MOV     BYTE PTR [SI+85],0
                MOV     AL,[SI+42]
                OR      AL,[SI+54]
                CMP     AL,1
                JNE     NO_IMPR_PANT
                CMP     BYTE PTR [SI+55],1
                JNE     NO_IMPR_PANT
                MOV     BYTE PTR [SI+84],1
NO_IMPR_PANT:   MOV     AL,[SI+29]
                OR      AL,[SI+157]
                CMP     AL,1
                JNE     NO_PAUSA
                CMP     BYTE PTR [SI+69],1
                JNE     NO_PAUSA
                MOV     BYTE PTR [SI+85],1
NO_PAUSA:       MOV     AL,20h
                OUT     20h,AL
                POP     SI
                POP     DS
                POP     BX
                POP     AX
                IRET
TECLADO         ENDP

                PUBLIC  _DIBUJA_BLOQUE

_DIBUJA_BLOQUE  PROC    NEAR
                PUSH    BP              ;Salva registros afectados a la pila.
                MOV     BP,SP           ;Pone el puntero de la pila en BP.
                PUSH    AX              ;Salva registros afectados a la pila.
                PUSH    BX
                PUSH    CX
                PUSH    DX
                PUSH    DI
                PUSH    SI
                PUSH    DS
                PUSH    ES
                LDS     BX,[BP+4]

                MOV     SI,[BX]
                MOV     AX,[BX+2]
                MOV     DX,320
                MUL     DX
                ADD     SI,AX
                ADD     SI,[BP+8]

                MOV     DI,[BX+4]
                MOV     AX,[BX+6]
                MOV     DX,320
                MUL     DX
                ADD     DI,AX
                ADD     DI,[BP+12]

                MOV     CX,[BX+8]
                MOV     DX,[BX+10]
                MOV     DS,[BP+10]
                MOV     ES,[BP+14]
                MOV     BX,CX
                CLD

MAS_LINE:       MOV     CX,BX
MISM_LINE:      LODSB
                CMP     AL,0FFh         ;Color de la m scara
                JE      PIX_MASC
                MOV     ES:[DI],AL
PIX_MASC:       INC     DI
                LOOP    MISM_LINE
                SUB     SI,BX
                SUB     DI,BX
                ADD     SI,320
                ADD     DI,320
                DEC     DX
                JNZ     MAS_LINE

                POP     ES              ;Reestablece registros desde la pila.
                POP     DS
                POP     SI
                POP     DI
                POP     DX
                POP     CX
                POP     BX
                POP     AX
                POP     BP
                RET                     ;Vuelve al C.
_DIBUJA_BLOQUE  ENDP

                PUBLIC  _DIBUJA_BLOQUE_CUT

_DIBUJA_BLOQUE_CUT      PROC    NEAR
                PUSH    BP              ;Salva registros afectados a la pila.
                MOV     BP,SP           ;Pone el puntero de la pila en BP.
                PUSH    AX              ;Salva registros afectados a la pila.
                PUSH    BX
                PUSH    CX
                PUSH    DX
                PUSH    SI
                PUSH    DI
                PUSH    DS
                PUSH    ES
                LDS     BX,[BP+4]       ;Obtiene el puntero de los datos. Este
                                        ;valor lo puso el C antes de llamar.
                MOV     AX,[BX+4]
                CMP     AX,320
                JGE     NO_PAINT_EM
                ADD     AX,[BX+8]
                CMP     AX,0
                JLE     NO_PAINT_EM
                MOV     AX,[BX+6]
                CMP     AX,200
                JGE     NO_PAINT_EM
                ADD     AX,[BX+10]
                CMP     AX,0
                JLE     NO_PAINT_EM
                JMP     SHORT SI_PAINT
NO_PAINT_EM:    JMP     NO_PAINT

SI_PAINT:       PUSH    BP              ;Guarda el registro BP.

                MOV     CX,[BX+8]       ;Ancho del dibujo.
                MOV     DX,[BX+10]      ;Alto del dibujo.

                MOV     BP,[BX+6]
                CMP     BP,0
                JGE     BUEN_Y_SUP
                ADD     DX,BP           ;Ajusta la parte superior del dibujo.
BUEN_Y_SUP:     ADD     BP,[BX+10]
                SUB     BP,200
                JLE     BUEN_Y_INF
                SUB     DX,BP           ;Ajusta la parte inferior del dibujo.

BUEN_Y_INF:     MOV     BP,[BX+4]
                CMP     BP,0
                JGE     BUEN_X_IZQ
                ADD     CX,BP           ;Ajusta la parte izquierda del dibujo.
BUEN_X_IZQ:     ADD     BP,[BX+8]
                SUB     BP,320
                JLE     BUEN_X_DER
                SUB     CX,BP           ;Ajusta la parte derecha del dibujo.

BUEN_X_DER:     PUSH    DX

                MOV     SI,[BX+2]
                MOV     BP,[BX+6]
                CMP     BP,0
                JGE     Y_POSIT
                NEG     BP
                ADD     SI,BP
Y_POSIT:        MOV     AX,320
                MUL     SI
                MOV     SI,AX           ;Prepara el eje Y de origen del dibujo.

                MOV     DI,[BX+6]
                CMP     DI,0
                JGE     Y_POSI
                MOV     DI,0
Y_POSI:         MOV     AX,320
                MUL     DI
                MOV     DI,AX           ;Prepara el eje Y de destino del dibujo.

                POP     DX

                MOV     BP,[BX+4]
                CMP     BP,0
                JGE     X_POSIT
                NEG     BP
                ADD     SI,BP
X_POSIT:        ADD     SI,[BX]         ;Prepara el eje X de origen del dibujo.

                MOV     BP,[BX+4]
                CMP     BP,0
                JGE     X_POSI
                MOV     BP,0
X_POSI:         ADD     DI,BP           ;Prepara el eje X de destino del dibujo.

                POP     BP

                ADD     SI,[BP+8]       ;Prepara la direcci¢n de origen.
                ADD     DI,[BP+12]      ;Prepara la direcci¢n de destino.
                MOV     DS,[BP+10]      ;Asigna el segmento origen del dibujo
                MOV     ES,[BP+14]      ;Asigna el segmento destino del dibujo

                MOV     BP,320
                SUB     BP,CX           ;Cantidad a sumar en cada linea.
                MOV     BX,CX           ;Ancho del dibujo.
                CLD                     ;Pone direcci¢n creciente.

MAS_LYNES:      MOV     CX,BX           ;Vuelca el dibujo en pantalla.
MISM_LYNE:      LODSB
                CMP     AL,0FFh         ;Color de la m scara
                JE      PIX_MASK
                MOV     ES:[DI],AL
PIX_MASK:       INC     DI
                LOOP    MISM_LYNE
                ADD     SI,BP
                ADD     DI,BP
                DEC     DX
                JNZ     MAS_LYNES

NO_PAINT:       POP     ES              ;Reestablece registros desde la pila.
                POP     DS
                POP     DI
                POP     SI
                POP     DX
                POP     CX
                POP     BX
                POP     AX
                POP     BP
                RET                     ;Vuelve al C.
_DIBUJA_BLOQUE_CUT      ENDP

                PUBLIC  _VUELCA_PANTALLA

_VUELCA_PANTALLA        PROC    NEAR
                PUSH    BP              ;Salva registros afectados a la pila.
                MOV     BP,SP           ;Pone el puntero de la pila en BP.
                PUSH    AX              ;Salva registros afectados a la pila.
                PUSH    BX
                PUSH    CX
                PUSH    DI
                PUSH    DS
                PUSH    DX
                PUSH    ES
                PUSH    SI

                MOV     SI,[BP+4]       ;X inicial del bloque a volcar.
                MOV     AX,[BP+6]       ;Y inicial del bloque a volcar.
                MOV     DX,320
                MUL     DX
                ADD     SI,AX
                ADD     SI,[BP+16]

                MOV     DI,[BP+8]       ;X inicial del sitio a volcar.
                MOV     AX,[BP+10]      ;Y inicial del sitio a volcar.
                MOV     DX,320
                MUL     DX
                ADD     DI,AX

                MOV     CX,[BP+12]      ;Obtine el ancho a volcar.
                MOV     DX,[BP+14]      ;Obtine el alto  a volcar.

                MOV     DS,[BP+18]      ;Segmento del bloque a volcar.
                MOV     AX,0A000h       ;Asigna el segmento del v¡deo.
                MOV     ES,AX

                MOV     BX,320
                SUB     BX,CX
                MOV     BP,CX
                CLD

VUELC:          MOV     CX,BP
                REP     MOVSB
                ADD     SI,BX
                ADD     DI,BX
                DEC     DX
                JNZ     VUELC

                POP     SI              ;Reestablece registros desde la pila.
                POP     ES
                POP     DX
                POP     DS
                POP     DI
                POP     CX
                POP     BX
                POP     AX
                POP     BP
                RET                     ;Vuelve al C.
_VUELCA_PANTALLA        ENDP

                PUBLIC  _BORRA_ZONA_MEMORIA

_BORRA_ZONA_MEMORIA     PROC    NEAR
                PUSH    BP              ;Salva registros afectados a la pila.
                MOV     BP,SP           ;Pone el puntero de la pila en BP.
                PUSH    AX              ;Salva registros afectados a la pila.
                PUSH    CX
                PUSH    DI
                PUSH    ES

                LES     DI,[BP+4]       ;Direcci¢n de memoria a borrar.
                MOV     AL,[BP+8]
                MOV     AH,AL
                MOV     CX,320*200/2
                CLD
                REP     STOSW

                POP     ES              ;Reestablece registros desde la pila.
                POP     DI
                POP     CX
                POP     AX
                POP     BP
                RET                     ;Vuelve al C.
_BORRA_ZONA_MEMORIA     ENDP

                PUBLIC  _COPIA_ZONA_MEMORIA

_COPIA_ZONA_MEMORIA     PROC    NEAR
                PUSH    BP              ;Salva registros afectados a la pila.
                MOV     BP,SP           ;Pone el puntero de la pila en BP.
                PUSH    AX              ;Salva registros afectados a la pila.
                PUSH    CX
                PUSH    DI
                PUSH    SI
                PUSH    DS
                PUSH    ES

                LDS     SI,[BP+4]       ;Direcci¢n de memoria origen.
                LES     DI,[BP+8]       ;Direcci¢n de memoria destino.
                MOV     CX,320*200/2
                CLD
                REP     MOVSW

                POP     ES              ;Reestablece registros desde la pila.
                POP     DS
                POP     SI
                POP     DI
                POP     CX
                POP     AX
                POP     BP
                RET                     ;Vuelve al C.
_COPIA_ZONA_MEMORIA     ENDP

_TEXT           ENDS

                END

