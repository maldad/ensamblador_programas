.286
Title

spila SEGMENT STACK
  DB 32 DUP('Stack---') ; definir 32 bytes con valor Stack---
spila ENDS

sdatos SEGMENT
  num1 db 5,'$'
  num2 db 4,'$'
sdatos ENDS

scodigo SEGMENT 'CODE'
  ASSUME SS:spila, DS:sdatos, CS:scodigo
  princ PROC FAR
    ;PUSH DS
    ;PUSH 0
    MOV AL, 5
    MOV BL, 4
    ADD AL, BL
    ADD AL, 30H ;suma a num2 48dec para coincidir con ASCII

    MOV DL, AL
    MOV AH, 02H
    INT 21H ;llamar a la interrupcion de DOS
    ;RET ;instruccion para retornar
    .EXIT
  princ ENDP

scodigo ENDS

END princ
