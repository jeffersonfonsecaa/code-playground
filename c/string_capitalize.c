#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define NAME_MAX_LENGTH 50


/*
    Capitaliza a primeira letra de cada palavra de uma string.

    A função recebe:
    - source: cadeia de caracteres original
    - result: vetor de caracteres que receberá a string modificada

    Observações:
    - O vetor 'result' deve ter tamanho suficiente para conter a string original + '\0'.
*/


void capitalizeWords(const char source[], char result[]) {
    bool firstLetter = true;
    int i = 0;

    while (source[i] != '\0') {
        result[i] = firstLetter ? toupper(source[i]) : source[i];
        firstLetter = (source[i] == ' ');
        i++;
    }

    result[i] = '\0';
}

int main() {
    char name[NAME_MAX_LENGTH] = "joao pe de feijao";
    char capitalizedName[NAME_MAX_LENGTH];

    capitalizeWords(name, capitalizedName);

    printf("Normal: %s\n", name);
    printf("Modificado: %s\n", capitalizedName);
    
    return 0;
}
