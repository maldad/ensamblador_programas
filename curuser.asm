.286
print MACRO buffer
    LEA DX, buffer
    MOV AH, 09H
    INT 21H
ENDM

read_one MACRO
    MOV AH, 01H
    INT 21H
ENDM

clear_screen MACRO
    MOV AH, 06H
    MOV AL, 0H
    MOV CX, 0H
    MOV DH, 24 ;24 renglones
    MOV DL, 79 ;79 columnas
    MOV BH, 00001101b ;magenta claro
    INT 10H
ENDM
        
gotoxy macro ren, col
    mov ah, 02h
    mov bh, 00h
    mov dh, ren
    mov dl, col
    int 10h
endm


spila SEGMENT STACK
    DB 32 DUP('Stack000') ;definir 32 bytes con valor Stack---
                          ;repetir 32 veces 'Stack---'
spila ENDS

sdatos SEGMENT
    xDecenas DB 13,10,'X decenas: (0 a 2)',13,10,'$'
    xUnidades DB 13,10,'X unidades: (0 a 9)',13,10,'$'
    yDecenas DB 13,10,'Y decenas (0 a 7)',13,10,'$'
    yUnidades DB 13,10,'Y unidades (0 a 9)',13,10,'$'
    posicion DB '.(X, Y)',13,10,'$'
    ren DB ?,'$'
    col DB ?,'$'
sdatos ENDS

scodigo SEGMENT 'CODE'
    ASSUME SS:spila, DS:sdatos, CS:scodigo

    princ PROC FAR
        PUSH DS
        PUSH 0
        MOV AX, sdatos
        MOV DS, AX


        print xDecenas
        read_one
        SUB AL, 48
        ;leemos la decena , multiplicamos *10
        MOV BL, 10
        MUL BL  ;AX = AL * BL = AL * 10
        MOV CX, AX      ;almacenamos el resultado parcial

        print xUnidades
        read_one
        SUB AL, 48
        ADD CL, AL
        MOV ren, CL

        MOV CX, 0       ;limpiamos CX
        ;termina la parte de posicion en X

        print yDecenas
        read_one
        SUB AL, 48
        MUL BL
        MOV CX, AX

        print yUnidades
        read_one
        SUB AL, 48
        ADD CL, AL
        mov col, CL

        clear_screen
        gotoxy ren, col 
        print posicion

        ;print ren
        ;print col

        RET
    princ ENDP
scodigo ENDS

END princ
