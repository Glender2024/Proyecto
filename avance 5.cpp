#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>

struct Contacto {
    char nombre[50];
    char telefono[20];
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

int wherex() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
}

int wherey() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
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

void ingresarTexto(const char* msj, char* dato, int max_len) {
    int pos = 0;
    char c;
    printf("%s ", msj);
    dato[0] = '\0';

    while (1) {
        c = getch();

        if (c == 13) break;

        if (c == 8 && pos > 0) {
            pos--;
            dato[pos] = '\0';
            printf("\b \b");
        } else if (c >= 32 && c <= 126 && pos < max_len - 1) {
            dato[pos] = c;
            pos++;
            dato[pos] = '\0';
            printf("%c", c);
        } else if (c == 75 && pos > 0) {
            pos--;
            gotoxy(wherex() - 1, wherey());
        } else if (c == 77 && pos < strlen(dato)) {
            pos++;
            gotoxy(wherex() + 1, wherey());
        }
    }
}

void ingresarTelefono(const char* msj, char* numero, int max_len) {
    int pos = 0;
    char c;
    printf("%s ", msj);
    numero[0] = '\0';

    while (1) {
        c = getch();

        if (c == 13) break;

        if (c == '+' && pos == 0) {
            numero[pos] = c;
            pos++;
            numero[pos] = '\0';
            printf("%c", c);
        } else if (c >= '0' && c <= '9' && pos < max_len) {
            numero[pos] = c;
            pos++;
            numero[pos] = '\0';
            printf("%c", c);
        } else if (c == 8 && pos > 0) {
            pos--;
            numero[pos] = '\0';
            printf("\b \b");
        } else if (c == 75 && pos > 0) {
            pos--;
            gotoxy(wherex() - 1, wherey());
        } else if (c == 77 && pos < strlen(numero)) {
            pos++;
            gotoxy(wherex() + 1, wherey());
        }
    }
}

bool validarTelefono(const char* telefono) {
    int longitud = strlen(telefono);
    if (longitud < 7 || longitud > 15) {
        return false;  
    }

    
    if (telefono[0] == '+') {
        for (int i = 1; i < longitud; i++) {
            if (telefono[i] < '0' || telefono[i] > '9') {
                return false;  
            }
        }
    }
    
    else {
        for (int i = 0; i < longitud; i++) {
            if (telefono[i] < '0' || telefono[i] > '9') {
                return false;  
            }
        }
    }

    return true;  
}

bool validarCorreo(const char* correo) {
    return (strchr(correo, '@') != NULL && (strstr(correo, ".com") != NULL || strstr(correo, ".edu.ec") != NULL));
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
    ingresarTexto("Ingrese el nombre:", agenda[totalContactos].nombre, 50);

    generarNombreUnico(agenda[totalContactos].nombre);

    char telefono[20];
    int indiceExistente;
    while (1) {
        gotoxy(15, 4);
        ingresarTelefono("Ingrese el telefono:", telefono, 20);

        
        if (strlen(telefono) < 7 || strlen(telefono) > 15) {
            gotoxy(15, 6);
            printf("Telefono invalido, intente nuevamente.");
            getch();
            gotoxy(15, 6);
            printf("                                             ");
            continue;
        }

        
        if (!validarTelefono(telefono)) {
            gotoxy(15, 6);
            printf("Formato de telefono invalido, intente nuevamente.");
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
                
                gotoxy(15, 4);
                printf("                                             ");
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
        ingresarTexto("Ingrese el correo electronico:", agenda[totalContactos].correo, 50);

        if (!validarCorreo(agenda[totalContactos].correo)) {
            gotoxy(15, 7);
            printf("Correo no valido. Debe contener '@' y '.com' o '.edu.ec'. Intente nuevamente.");
            getch();
            gotoxy(15, 7);
            printf("                                             ");
            continue;
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
    ingresarTexto("Ingrese el nombre a buscar:", nombre, 50);
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

    FILE *archivo = fopen("contactos_ordenados.txt", "w");
    if (archivo == NULL) {
        gotoxy(15, 5);
        printf("Error al abrir el archivo.");
        getch();
        return;
    }
    for (int i = 0; i < totalContactos; i++) {
        fprintf(archivo, "%d. %s - %s - %s\n", i + 1, agenda[i].nombre, agenda[i].telefono, agenda[i].correo);
    }
    fclose(archivo);
    gotoxy(15, 5);
    printf("Contactos ordenados guardados en 'contactos_ordenados.txt'.");
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