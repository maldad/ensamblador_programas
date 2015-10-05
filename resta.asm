.286
Title

spila SEGMENT STACK
    DB 32 DUP('Stack000')
spila ENDS

sdatos SEGMENT

sdatos ENDS

scodigo SEGMENT 'CODE'
    ASSUME SS:spila, DS:sdatos, CS:scodigo

    principal PROC FAR
        MOV AL, 9
        MOV BL, 3
        SUB AL, BL
        ADD AL, 30H

        MOV DL, AL
        MOV AH, 02H
        INT 21H

        .EXIT
    principal ENDP

scodigo ENDS

END principal
