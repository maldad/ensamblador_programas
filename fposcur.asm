.286

spila SEGMENT STACK
    DB 32 DUP('Stack000') ;definir 32 bytes con valor Stack---
                          ;repetir 32 veces 'Stack---'
spila ENDS

sdatos SEGMENT
    cadena DB 'hola mundo en alguna posicion',13,10,'$'
sdatos ENDS

scodigo SEGMENT 'CODE'
    ASSUME SS:spila, DS:sdatos, CS:scodigo

    princ PROC FAR
        PUSH DS
        PUSH 0
        MOV AX, sdatos
        MOV DS, AX


        ;clear_screen MACRO
        MOV AH, 06H
        MOV AL, 0H
        MOV CX, 0H
        MOV DH, 24 ;24 renglones
        MOV DL, 79 ;79 columnas
        MOV BH, 00001101b ;magenta claro
        INT 10H
        ;ENDM
        
        ;imprimir una cadena de prueba...
        LEA DX, cadena
        MOV AH, 09H
        INT 21H

        ;gotoxy macro ren,col
        mov ah, 02h
        mov bh, 00h
        mov dh, 12
        mov dl, 45
        int 10h
        ;endm

        LEA DX, cadena
        MOV AH, 09H
        INT 21H

        RET
    princ ENDP
scodigo ENDS

END princ
