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
#define INTERVALO 100
//estos 2 arreglos deberian tener la misma longitud
char cadena[] = "aeropuerto";
char cadenaRevelada[30];
char caracterLeido; //caracter leido
int longitudCadena;
int i; //contador para FOR's
int intentos = 10;
char abecedario[54] = "a b c d e f g h i j k l m n o p q r s t u v w x y z";


void rellena_cadenaRevelada(){
    for(i = 0; i < strlen(cadena); i++){
        cadenaRevelada[i] = '_';
    }
}

void instrucciones(){
    printf("%s\n", "Presiona una letra para revelar si la palabra misteriosa la incluye");
    printf("%s\n", "Presiona 1 para adivinar la palabra de una vez");
}

void imprimirIntentos(){
    printf("Te quedan %d intentos\n", intentos);
}

void palabra(){
    /*
     * ESTA FORMA DE IMPRIMIR LA CADENA TRABA EL PROGRAMA!
     */
    for(i = 0; i < longitudCadena; i++){
        printf("%c", cadenaRevelada[i]);
        printf("  ");
    }printf("\n");
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
    longitudCadena = strlen(cadena);
    for(i = 0; i < longitudCadena; i++){
        if(cadena[i] == caracterLeido){
            cadenaRevelada[i] = caracterLeido;
        }
    }
}

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
        /*no puedo pasar directamente 
         * mov dh, x
         * mov dl, y
         * don't know why (._.)
         * */
        MOV CX, x 
        mov dh, cl
        mov cx, y
        mov dl, cl
        MOV AH, 02H
        MOV BH, 00H
        INT 10H
    }
}

void interfaz_del_juego(){
    //instrucciones(); //imprime las instrucciones
    imprimirIntentos(); //te quedan N intentos
    //teclaLetra(); //usa la interrupcion para oprimir una tecla
    //identifica(); //compara si letra pulsada esta en la cadena
    palabra(); //
}

void imprimeAbecedario(){
    /*
    for(i = 0; i < 10; i++){
        irxy(i, 16);
        printf("%s\n", abecedario);
        //irxy(20, 0);
        //interfaz_del_juego();
        //imprimirIntentos();
        //palabra();
        delay(INTERVALO);
        clearScreen();
    }
    */
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
    imprimeAbecedario();
    /*
    instrucciones();
    palabra(); //imprime la palabra
    while(intentos > 1){
        imprimirIntentos();
        teclaLetra();
        identifica();
        palabra();  
    }
    */
    system("pause");
}
