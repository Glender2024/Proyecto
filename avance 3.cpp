#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>

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
    numero[0] = '\0'; 
    
    while (1) {
        c = getch();
        
        if (c == 13) break;  
        
        if (c >= '0' && c <= '9' && pos < max_len) {
            numero[pos] = c;
            pos++;
            numero[pos] = '\0'; 
            printf("%c", c);
        }
        else if (c == 8 && pos > 0) {  
            pos--;
            numero[pos] = '\0';
            printf("\b \b");
        }
    }
}

bool validarCorreo(const char* correo) {
    return (strchr(correo, '@') != NULL && strstr(correo, ".com") != NULL);
}

bool numeroYaExiste(const char* telefono, int* indiceExistente) {
    for (int i = 0; i < totalContactos; i++) {
        if (strcmp(agenda[i].telefono, telefono) == 0) {
            *indiceExistente = i; 
            return true; 
        }
    }
    return false; 
}

void generarNombreUnico(char nombre[]) {
    char nombreBase[50];
    strcpy(nombreBase, nombre); 
    int contador = 1;

    while (1) {
        bool nombreExiste = false;
        for (int i = 0; i < totalContactos; i++) {
            if (strcmp(agenda[i].nombre, nombre) == 0) {
                nombreExiste = true;
                break;
            }
        }

        if (!nombreExiste) {
            break; 
        }

        sprintf(nombre, "%s (%d)", nombreBase, contador);
        contador++;
    }
}

void agregarContacto() {
    system("cls");
    dibujarCuadro(10, 2, 80, 15); 
    if (totalContactos >= MAX_CONTACTOS) {
        gotoxy(15, 3);
        printf("La agenda esta llena. No se pueden agregar mas contactos.");
        getch();
        return;
    }

    gotoxy(15, 3);
    printf("Ingrese el nombre: ");
    fgets(agenda[totalContactos].nombre, 50, stdin);
    agenda[totalContactos].nombre[strcspn(agenda[totalContactos].nombre, "\n")] = 0; 

    generarNombreUnico(agenda[totalContactos].nombre);

    char telefono[15];
    int indiceExistente;
    while (1) {
        gotoxy(15, 4);
        ingresarEnteros("Ingrese el telefono:", telefono, 10);

        if (strlen(telefono) != 10) {
            gotoxy(15, 6);
            printf("telefono invalido, intente nuevamente.");
            getch();
            gotoxy(15, 6);
            printf("                                             "); 
            continue;
        }

       
        if (numeroYaExiste(telefono, &indiceExistente)) {
            gotoxy(15, 6);
            printf("Este numero es de %s. Desea sobrescribirlo? (s/n): ", agenda[indiceExistente].nombre);
            char opcion = getch();
            if (opcion == 's' || opcion == 'S') {
                
                strcpy(agenda[indiceExistente].nombre, agenda[totalContactos].nombre);
                gotoxy(15, 8);
                printf("Contacto sobrescrito exitosamente.");
                getch();
                return;
            } else {
                
                gotoxy(15, 6);
                printf("                                             "); 
                continue;
            }
        }

        
        strcpy(agenda[totalContactos].telefono, telefono);
        break;
    }

    
    while (1) {
        gotoxy(15, 5);
        printf("Ingrese el correo electronico: ");
        fgets(agenda[totalContactos].correo, 50, stdin);
        agenda[totalContactos].correo[strcspn(agenda[totalContactos].correo, "\n")] = 0; 

       
        if (!validarCorreo(agenda[totalContactos].correo)) {
            gotoxy(15, 7);
            printf("Correo no valido. Debe contener '@' y '.com'. Intente nuevamente.");
            getch();
            gotoxy(15, 7);
            printf("                                             "); 
        }

        
        break;
    }

    
    totalContactos++;
    gotoxy(15, 9);
    printf("Contacto agregado exitosamente.");
    getch();
}

void mostrarContactos() {
    system("cls");
    dibujarCuadro(10, 2, 80, 15);
    if (totalContactos == 0) {
        gotoxy(15, 5);
        printf("No hay contactos en la agenda.");
        getch();
        return;
    }
    gotoxy(15, 3);
    printf("Lista de contactos:");
    for (int i = 0; i < totalContactos; i++) {
        gotoxy(15, 5 + i);
        printf("%d. %s - %s - %s", i + 1, agenda[i].nombre, agenda[i].telefono, agenda[i].correo);
    }
    getch();
}

void buscarPorNombre() {
    system("cls");
    dibujarCuadro(10, 2, 80, 15);
    char nombre[50];
    gotoxy(15, 3);
    printf("Ingrese el nombre a buscar: ");
    fgets(nombre, 50, stdin);
    nombre[strcspn(nombre, "\n")] = 0; 
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
    getch();
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
    getch();
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