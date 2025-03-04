#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

struct Contacto {
    char nombre[50];
    char telefono[15];
    char correo[50]; 
};

const int MAX_CONTACTOS = 100;
Contacto agenda[MAX_CONTACTOS];
int totalContactos = 0;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void dibujarCuadro(int x1, int y1, int x2, int y2) {
    for (int i = x1; i <= x2; i++) {
        gotoxy(i, y1); printf("%c", 205);
        gotoxy(i, y2); printf("%c", 205);
    }
    for (int i = y1; i <= y2; i++) {
        gotoxy(x1, i); printf("%c", 186);
        gotoxy(x2, i); printf("%c", 186);
    }
    gotoxy(x1, y1); printf("%c", 201);
    gotoxy(x2, y1); printf("%c", 187);
    gotoxy(x1, y2); printf("%c", 200);
    gotoxy(x2, y2); printf("%c", 188);
}

void ingresarEnteros(const char* msj, char* numero, int max_len) {
    int pos = 0;
    char c;
    printf("%s ", msj);
    numero[0] = '\0'; // Inicializar la cadena de caracteres
    
    while (1) {
        c = getch();
        
        if (c == 13) break;  // Enter key
        
        if (c >= '0' && c <= '9' && pos < max_len) {
            numero[pos] = c;
            pos++;
            numero[pos] = '\0'; // Asegurarse de que la cadena esté terminada en null
            printf("%c", c);
        }
        else if (c == 8 && pos > 0) {  // Backspace key
            pos--;
            numero[pos] = '\0';
            printf("\b \b");
        }
    }
}

int mostrarMenu() {
    int opcion = 1;
    char tecla;
    do {
        system("cls");
        dibujarCuadro(10, 2, 80, 10);
        gotoxy(15, 3);
        printf("           AGENDA TELEFONICA           ");
        gotoxy(15, 5);
        if (opcion == 1) printf("> 1. Mostrar contactos");
        else printf("  1. Mostrar contactos");
        gotoxy(15, 6);
        if (opcion == 2) printf("> 2. Buscar por nombre");
        else printf("  2. Buscar por nombre");
        gotoxy(15, 7);
        if (opcion == 3) printf("> 3. Agregar contactos");
        else printf("  3. Agregar contactos");
        gotoxy(15, 8);
        if (opcion == 4) printf("> 4. Ordenar por nombre");
        else printf("  4. Ordenar por nombre");
        gotoxy(15, 9);
        if (opcion == 5) printf("> 5. Salir");
        else printf("  5. Salir");
        tecla = getch();
        if (tecla == 72 && opcion > 1) opcion--;
        if (tecla == 80 && opcion < 5) opcion++;
    } while (tecla != 13);
    return opcion;
}

void mostrarContactos() {
    system("cls");
    dibujarCuadro(10, 2, 80, 15);
    if (totalContactos == 0) {
        gotoxy(15, 5);
        printf("No hay contactos en la agenda.");
        return;
    }
    gotoxy(15, 3);
    printf("Lista de contactos:");
    for (int i = 0; i < totalContactos; i++) {
        gotoxy(15, 5 + i);
        printf("%d. %s - %s - %s", i + 1, agenda[i].nombre, agenda[i].telefono, agenda[i].correo);
    }
}

void buscarPorNombre() {
    system("cls");
    dibujarCuadro(10, 2, 80, 15);
    char nombre[50];
    gotoxy(15, 3);
    printf("Ingrese el nombre a buscar: ");
    scanf("%s", nombre);
    bool encontrado = false;
    for (int i = 0; i < totalContactos; i++) {
        if (strstr(agenda[i].nombre, nombre) != NULL) {
            gotoxy(15, 5 + i);
            printf("%s - %s - %s", agenda[i].nombre, agenda[i].telefono, agenda[i].correo);
            encontrado = true;
        }
    }
    if (!encontrado) {
        gotoxy(15, 5);
        printf("No se encontraron contactos con ese nombre.");
    }
}

void agregarContacto() {
    system("cls");
    dibujarCuadro(10, 2, 80, 12); 
    if (totalContactos >= MAX_CONTACTOS) {
        gotoxy(15, 3);
        printf("La agenda esta llena. No se pueden agregar mas contactos.");
        return;
    }
    gotoxy(15, 3);
    printf("Ingrese el nombre: ");
    scanf("%s", agenda[totalContactos].nombre);
    gotoxy(15, 4);
    ingresarEnteros("Ingrese el telefono:", agenda[totalContactos].telefono, 10);
    gotoxy(15, 5);
    printf("Ingrese el correo electronico: ");
    scanf("%s", agenda[totalContactos].correo);
    totalContactos++;
    gotoxy(15, 7);
    printf("Contacto agregado exitosamente.");
}

void ordenarPorNombre() {
    system("cls");
    dibujarCuadro(10, 2, 80, 10);
    for (int i = 0; i < totalContactos - 1; i++) {
        for (int j = 0; j < totalContactos - i - 1; j++) {
            if (strcmp(agenda[j].nombre, agenda[j + 1].nombre) > 0) {
                Contacto temp = agenda[j];
                agenda[j] = agenda[j + 1];
                agenda[j + 1] = temp;
            }
        }
    }
    gotoxy(15, 3);
    printf("Contactos ordenados por nombre.");
}

int main() {
    int opcion;
    do {
        opcion = mostrarMenu();
        switch (opcion) {
            case 1:
                mostrarContactos();
                break;
            case 2:
                buscarPorNombre();
                break;
            case 3:
                agregarContacto();
                break;
            case 4:
                ordenarPorNombre();
                break;
            case 5:
                gotoxy(15, 12);
                printf("Saliendo del programa...");
                break;
        }
        if (opcion != 5) {
            gotoxy(15, 20);
            printf("Presione Enter para continuar...");
            getch();
        }
    } while (opcion != 5);
    return 0;
}