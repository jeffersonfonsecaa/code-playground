#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


/*
 * Minigame Bomberman — Movimentação, colisão e destruição de paredes
 *
 * Este projeto implementa a lógica fundamental de um minigame estilo Bomberman,
 * com movimentação do personagem, detonação de paredes e feedback visual via matriz.
 * A estrutura separa a camada lógica (logicWorld) da camada visual (visualWorld),
 * com o renderizador (showWorld) mantendo a coesão entre ambas.
 *
 * A atualização do jogo ocorre de forma interativa: cada entrada do usuário
 * provoca uma atualização no estado do mundo.
 *
 * Futuramente, serão adicionadas melhorias na validação de entradas,
 * tratamento de erros e refinamento semântico.
 *
 * Autor: Jefferson Fonseca
 * GitHub: https://github.com/jeffersonfonsecaa
 */



#define CLEAN_COMMAND "cls" //  Para linux use "clear"
#define SIZE_FLAG_MSG 55
#define WORLD_SIZE 5
#define BOMB_TIMER 3
#define WALL_SYMBOL '#'
#define WALL_TRASH_SYMBOL '.'


struct Bmb {
    char symbol;
    int x, y,
        timer;
    bool existOnMap,
         detonation;
};

struct Player {
    int y, x;
    char symbol;
    struct Bmb bomb;
};

struct Tile {
    char symbol;
    bool isWalkable,
         canBeDestroyed;

};


struct Tile Wall = {
    .symbol = '#',
    .isWalkable = false,
    .canBeDestroyed = true
};

struct Tile Ground = {
    .symbol = ' ',
    .isWalkable = true,
    .canBeDestroyed = false
};

void createWorld(const char visualBoard[WORLD_SIZE][WORLD_SIZE], struct Tile logicalBoard[WORLD_SIZE][WORLD_SIZE]) {
    for (int i=0; i < WORLD_SIZE; i++) {
        for (int j=0; j < WORLD_SIZE; j++) {

            logicalBoard[i][j] = visualBoard[i][j] == WALL_SYMBOL ? Wall : Ground;
        }
    }
}

void showWorld(const struct Tile board[WORLD_SIZE][WORLD_SIZE], struct Player p) {
    char render[100];
    int count = 0;

    for (int i=0; i < WORLD_SIZE; i++) {
        render[count++] = '\n';

        for (int j=0; j < WORLD_SIZE; j++) {
            render[count++] = ' ';

            if (i == p.y && j == p.x) {
                render[count++] = p.symbol;
            } else {
                if (p.bomb.existOnMap && i == p.bomb.y && j == p.bomb.x) {
                    if (p.y != p.bomb.y || p.x != p.bomb.x) {
                        render[count++] = p.bomb.symbol;
                    }
                } else render[count++] = board[i][j].symbol;
            }

            render[count++] = ' ';
        }
    }

    render[count++] = '\n';
    render[count++] = '\0';

    printf("%s\n", render);
}

void destroyWall(struct Tile board[WORLD_SIZE][WORLD_SIZE], const int explostionTileY, const int explostionTileX) {
    board[explostionTileY][explostionTileX].symbol = WALL_TRASH_SYMBOL;
    board[explostionTileY][explostionTileX].isWalkable = true;
    board[explostionTileY][explostionTileX].canBeDestroyed = false;
}

void update(struct Tile board[WORLD_SIZE][WORLD_SIZE], struct Player* p) {
    if (p->bomb.existOnMap && !p->bomb.detonation) {
        p->bomb.timer--;

        if (p->bomb.timer <= 0) p->bomb.detonation = true;
    }
    
    if (p->bomb.existOnMap && p->bomb.detonation) {
        int by = p->bomb.y,
            bx = p->bomb.x;

        if (by > 0 && board[by-1][bx].canBeDestroyed) destroyWall(board, by-1, bx);
        if (by < WORLD_SIZE-1 && board[by+1][bx].canBeDestroyed) destroyWall(board, by+1, bx); 
       
        if (bx > 0 && board[by][bx-1].canBeDestroyed) destroyWall(board, by, bx-1);        
        if (bx < WORLD_SIZE-1 && board[by][bx+1].canBeDestroyed) destroyWall(board, by, bx+1);

        p->bomb.existOnMap = false;
        p->bomb.detonation = false;
        p->bomb.timer = BOMB_TIMER;
    }
    
}

void runGame() {
    char input;
    char flag[SIZE_FLAG_MSG] = "";
    bool gameLoop = true;

    struct Player p1;
    p1.symbol = 'P';
    p1.x = 1;
    p1.y = 3;
    p1.bomb.symbol = 'o';
    p1.bomb.detonation = false;
    p1.bomb.timer = BOMB_TIMER;
    
    char visualWorldLayer[WORLD_SIZE][WORLD_SIZE] = {
        {'#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', '#'},
        {'#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#'},
        {'#', '#', '#', '#', '#'},
    };

    struct Tile logicWorldLayer[WORLD_SIZE][WORLD_SIZE];
    
    createWorld(visualWorldLayer, logicWorldLayer);

    
    do {
        update(logicWorldLayer, &p1);

        system(CLEAN_COMMAND);
        showWorld(logicWorldLayer, p1);

        printf("%s", flag);
        strcpy(flag, "");
        
        printf("Use Q para sair\nUse W, A, S, D para mover\nOu B para soltar a bomba: ");
        scanf(" %c", &input);
        getchar();

        switch (tolower(input)) {
            case 'w':
                if (p1.y > 0 && logicWorldLayer[p1.y-1][p1.x].isWalkable) p1.y--;
                break;
            
            case 's':
                if (p1.y < WORLD_SIZE -1 && logicWorldLayer[p1.y+1][p1.x].isWalkable) p1.y++;
                break;
            
            case 'a':
                if (p1.x > 0 && logicWorldLayer[p1.y][p1.x-1].isWalkable) p1.x--;
                break;
            
            case 'd':
                if (p1.x < WORLD_SIZE -1 && logicWorldLayer[p1.y][p1.x+1].isWalkable) p1.x++;
                break;

            case 'b':
                if (!p1.bomb.existOnMap) {
                    p1.bomb.x = p1.x;
                    p1.bomb.y = p1.y;
                    p1.bomb.existOnMap = true;
                    
                } else strcpy(flag, "Nao e possivel colocar duas bombas ao mesmo tempo...\n");
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
