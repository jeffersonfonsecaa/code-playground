#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


/*
 * Movimentação de personagem com colisão
 * 
 * Implementa a lógica básica de movimento e colisão utilizando matriz,
 * com a movimentação sendo feita através de inputs do teclado no terminal.
 * A ordem da lógica de coordenadas é [y][x] (linha e coluna).
 * 
 * Futuramente serão adicionadas melhorias na validação de entradas,
 * no tratamento de erros e na clareza semântica do código.
 * 
 * Autor: Jefferson Fonseca
 * GitHub: https://github.com/jeffersonfonsecaa
 */


#define WORLD_SIZE 5
#define CLEAN_COMMAND "cls" //  Para linux use "clear"

struct player {
    int y, x;
    char symbol;
};


void showWorld(const char m[WORLD_SIZE][WORLD_SIZE], struct player p) {
    char render[1024];
    int count = 0;

    for (int i=0; i < WORLD_SIZE; i++) {
        render[count++] = '\n';

        for (int j=0; j < WORLD_SIZE; j++) {
            render[count++] = ' ';

            if (i == p.y && j == p.x) {
                render[count++] = p.symbol;
            } else {
                render[count++] = m[i][j];
            }

            render[count++] = ' ';
        }
    }

    render[count++] = '\n';
    render[count++] = '\0';

    printf("%s\n", render);
}

void runGame() {
    char input;

    char flag[50] = "";
    bool gameLoop = true;
    struct player p1 =  {3, 1, 'P'};
    
    char world[WORLD_SIZE][WORLD_SIZE] = {
        {'#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', '#'},
        {'#', ' ', '#', ' ', '#'},
        {'#', '#', '#', '#', '#'},
    };

    
    do {
        system(CLEAN_COMMAND);
        showWorld(world, p1);

        printf("%s", flag);
        strcpy(flag, "");
        
        printf("Use Q para sair\nUse W, A, S, D para mover: ");
        scanf(" %c", &input);

    
        switch (tolower(input)) {
            case 'w':
                if (p1.y > 0 && world[p1.y-1][p1.x] != '#') p1.y--;
                break;
            
            case 's':
                if (p1.y < WORLD_SIZE -1 && world[p1.y+1][p1.x] != '#') p1.y++;
                break;
            
            case 'a':
                if (p1.x > 0 && world[p1.y][p1.x-1] != '#') p1.x--;
                break;
            
            case 'd':
                if (p1.x < WORLD_SIZE -1 && world[p1.y][p1.x+1] != '#') p1.x++;
                break;

            case 'q':
                printf("\nObrigado por jogar!\n");
                gameLoop = false;
                continue;
                break;

            default:
                strcpy(flag, "Opcao invalida!\n");
                break;
        }

    } while (gameLoop);

}


int main() {
    runGame();
    return 0;
}
