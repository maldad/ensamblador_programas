#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

/*
TODO
[x] - implement a main flow
[x] - decrement the number of tries
[x] - put the irxy method into a loop for print the abc across the screen
[] - load different words
[] - words coming from a file?
*/

#define ESC 27
#define INTERVALO 500
//#define REGISTRO_BX 0
//estos 2 arreglos deberian tener la misma longitud
//de mientras, la cadena a formarse se establece aqui
char cadena[] = "uno";
char cadenaRevelada[30];
char caracterLeido; //caracter leido
char abecedario[54] = "a b c d e f g h i j k l m n o p q r s t u v w x y z";
int longitudCadena;
int i; //contador para FOR's
int intentos = 10;
int REGISTRO_BX = 0;
int posx, posy;
short fila, columna;
short game_over;

//Asembly instructions
void clearScreen(){
    asm{
        MOV AH, 06H
        MOV AL, 0H
        MOV CX, 0H
        MOV DH, 24 
        MOV DL, 79 
        MOV BH, 00001101b 
        INT 10H
    }
}

void irxy(int x, int y){
    asm{
        MOV CX, x 
        mov dh, cl
        mov cx, y
        mov dl, cl
        MOV AH, 02H
        MOV BH, 00H
        INT 10H
    }
}

void implementar_mouse(){
    asm{
        mov ax, 00
        int 33h
        mov ax, 01
        int 33h
    }
}

void mouse_clic(){
    asm{
        mov ax, 03
        int 33h
        mov REGISTRO_BX, bx 

        mov posx, cx
        mov posy, dx
    }
}
//Asembly instructions

//C instructions
void rellena_cadenaRevelada(){
    for(i = 0; i < strlen(cadena); i++){
        cadenaRevelada[i] = '_';
    }
}

void imprimirIntentos(){
    printf("Te quedan %d intentos\n", intentos);
}

void teclaLetra(){
    intentos--;
    asm{
        mov ah, 01h
        int 21h
        mov caracterLeido, al
    }printf("\n");
    if(caracterLeido == ESC) //salir del programa con ESC
        exit(EXIT_SUCCESS);
}

void identifica(){
    intentos--;
    longitudCadena = strlen(cadena);
    for(i = 0; i < longitudCadena; i++){
        if(cadena[i] == caracterLeido){
            cadenaRevelada[i] = caracterLeido;
        }
    }
}

void which_caracter(){
    switch(fila){
        case(16): caracterLeido = 'a';break;
        case(18): caracterLeido = 'b';break;
        case(20): caracterLeido = 'c';break;
        case(22): caracterLeido = 'd';break;
        case(24): caracterLeido = 'e';break;
        case(26): caracterLeido = 'f';break;
        case(28): caracterLeido = 'g';break;
        case(30): caracterLeido = 'h';break;
        case(32): caracterLeido = 'i';break;
        case(34): caracterLeido = 'j';break;
        case(36): caracterLeido = 'k';break;
        case(38): caracterLeido = 'l';break;
        case(40): caracterLeido = 'm';break;
        case(42): caracterLeido = 'n';break;
        case(44): caracterLeido = 'o';break;
        case(46): caracterLeido = 'p';break;
        case(48): caracterLeido = 'q';break;
        case(50): caracterLeido = 'r';break;
        case(52): caracterLeido = 's';break;
        case(54): caracterLeido = 't';break;
        case(56): caracterLeido = 'u';break;
        case(58): caracterLeido = 'v';break;
        case(60): caracterLeido = 'w';break;
        case(62): caracterLeido = 'x';break;
        case(64): caracterLeido = 'y';break;
        case(66): caracterLeido = 'z';break;
        default: caracterLeido = ' ';break;
    }
}

void revisa_cadenaRevelada(){
    char guion_default = '_';
    char *puntero;
    puntero = strchr(cadenaRevelada, guion_default);
    //busca en cadenaRevelada si aun tiene guiones
    if(puntero == NULL)
        game_over = 1;
    else
        game_over = 0;
}

void imprimeAbecedario(){
    i = 0;
    while(intentos > 1){
        irxy(i, 16);
        printf("%s\n", abecedario);
        delay(INTERVALO);
        clearScreen();
        i++;
        //que el abc no baje mas alla de la mitad de la pantalla
        if(i >= 10)
            i = 0;
        //manejar los clics
        mouse_clic();

        if(REGISTRO_BX == 1){
            //irxy(18, 1);
            //printf("clic izquierdo");
            fila = posx/8;
            columna = posy/8;
            which_caracter(fila);
            identifica();
            revisa_cadenaRevelada();
            if(game_over == 1){
                printf("Has ganado! \n");
                system("pause");
                exit(EXIT_SUCCESS);
            }else{}
        }

        if(REGISTRO_BX == 2){
            //irxy(19, 1);
            //printf("clic derecho");
            exit(EXIT_SUCCESS);
        }

        irxy(18, 1);
        printf("posicion %d %d", fila, columna);
        irxy(19, 1);
        printf("caracter leido: %c ", caracterLeido);
        //imprimir la interfaz del juego
        irxy(20, 1);
        imprimirIntentos();
        irxy(21, 1);
        printf("Forma la palabra: %s\n", cadena);
        irxy(22, 1);
        printf("%s\n", cadenaRevelada);
    }
}

void main(){
    clearScreen();
    rellena_cadenaRevelada();
    implementar_mouse();
    imprimeAbecedario();
    printf("se acabaron los intentos \n");
    system("pause");
}
