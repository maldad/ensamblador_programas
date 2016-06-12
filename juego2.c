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
[x] - words coming from a file
*/

#define ESC 27
#define INTERVALO 500
#define ENTER 13
//variables utilizadas para jugar
char cadena[] = "......";
char cadenaRevelada[] = "......";
int longitudCadena = 6;
char caracterLeido; //caracter leido
int intentos = 10;
char abecedario[54] = "a b c d e f g h i j k l m n o p q r s t u v w x y z";
//variables para la logica del juego
int i; //contador para FOR's
int REGISTRO_BX = 0;
int posx, posy;
short fila, columna;
short game_over;
int flujo;
int apuntador = 0;
//variables para lectura de archivos
char databuffer[36]; //bytes que vamos a leer
char handle[2]; //debe ser de 2 bytes (1 word) porque se le pasa al registro AX
char filename[]="a2.txt0";
int bytes_to_read = 36;
int buffer_size = 36;
char char_from_buffer;

//switch
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
        //mover a variables locales la posicion actual
        mov posx, cx
        mov posy, dx
    }
}

void abrir(){
    asm{
        lea dx, filename
        mov ah, 3dh
        mov al, 00h
        int 21h
        mov handle, ax
        }
}

void leer(){
    asm{
        mov ah, 3fh
        mov bx, handle
        mov cx, bytes_to_read
        lea dx, databuffer
        int 21h
    }
}

void cerrar(){
    asm{
        mov ah, 3eh
        mov bx, handle
        int 21h
    }
}
//Asembly instructions

//C instructions

void imprimirIntentos(){
    printf("Te quedan %d intentos\n", intentos);
}

void teclaLetra(){ //identifica la tecla pulsada, utilizado para continuar o dejar el juego
    asm{
        mov ah, 01h
        int 21h
        mov caracterLeido, al
    }
    if(caracterLeido == ESC){
        system("pause");
        exit(EXIT_SUCCESS); 
    } //salir del programa con ESC
    if(caracterLeido == ENTER)
        return;
        //flujo = 0;
        //otra ronda de juego
}

//refactorizar para buscar por comas
/*
void copy_from_buffer_to_local(){
    if(apuntador == 36)
        apuntador = 0;
    for(i = 0; i < longitudCadena; i++){
        cadena[i] = databuffer[apuntador];
        apuntador++;            
    }apuntador++; //adelanta la coma, y empieza a leer la siguiente palabra
}
*/
void copy_from_buffer_to_local(){
    int t = 1;
    int j = 0;
    while(t == 1){
        char_from_buffer = databuffer[apuntador];
        if(char_from_buffer == ',')
            break;
        cadena[j] = char_from_buffer;
        j++;
        apuntador++;
    }apuntador++; //adelanta la coma, y empieza a leer la siguiente palabra
}

void rellena_cadenaRevelada(){
    for(i = 0; i < longitudCadena; i++)
        cadenaRevelada[i] = '_';
}

void identifica(){
    intentos--;
    for(i = 0; i < longitudCadena; i++)
        if(cadena[i] == caracterLeido)
            cadenaRevelada[i] = caracterLeido;
}

void revisa_cadenaRevelada(){
    char guion_default = '_';
    char *puntero;
    puntero = strchr(cadenaRevelada, guion_default);
    //busca en cadenaRevelada si aun tiene guiones
    if(puntero == NULL)
        game_over = 1; //gana por formar la palabra entera
    else
        game_over = 0; //sigue en el juego
}

void imprimeAbecedario(){
    int px = 0;
    while(intentos > 1){
        irxy(px, 16);
        printf("%s\n", abecedario);
        delay(INTERVALO);
        clearScreen();
        px++;
        //que el abc no baje mas alla de la mitad de la pantalla
        if(px >= 10)
            px = 0;
        //manejar los clics
        mouse_clic();

        if(REGISTRO_BX == 1){
            fila = posx/8;
            columna = posy/8;
            which_caracter(fila);
            identifica();
            revisa_cadenaRevelada();
            if(game_over == 1){
                printf("revisando palabra...");
                delay(1000);
                printf("Has ganado! \n");
                printf("Formando la palabra: %s\n", cadenaRevelada);
                //flujo = 1;
                return;
            }else{}
        }

        if(REGISTRO_BX == 2){
            exit(EXIT_SUCCESS);
        }

        irxy(18, 1);
        //printf("posicion %d %d", fila, columna);
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
    flujo = 0;
    abrir();
    leer();
    cerrar();
    printf("buffer leido: %s \n", databuffer);
    system("pause");
    while(flujo == 0){
        intentos = 10;
        clearScreen();
        copy_from_buffer_to_local();
        rellena_cadenaRevelada(); //rellenar con guiones
        implementar_mouse();
        imprimeAbecedario();
    if(game_over == 0){
        printf("se acabaron los intentos \n");
        printf("llegaste a formar: %s\n", cadenaRevelada);
        //flujo = 1;
    }
    printf("volver a jugar? (presiona ENTER)\n");
    printf("salir de la aplicacion? (presiona ESC)\n");
        
    teclaLetra();
    }
    
    system("pause");
}

