#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


/*
 * Minigame Bomberman — Movimentação, explosões e destruição de paredes
 *
 * Implementação modular de um minigame inspirado em Bomberman.
 * O código separa claramente as camadas de lógica (logicWorldLayer) e visual (visualWorldLayer),
 * mantendo a coesão entre ambas por meio do renderizador (showWorld).
 *
 * A estrutura interna foi projetada para destacar clareza semântica e isolamento de responsabilidades:
 * - Funções como attemptMovePlayer e attemptPlaceBomb definem ações do jogador.
 * - updateWorld gerencia o ciclo de atualização e detonação de bombas.
 * - getTileSymbol abstrai a lógica de renderização célula a célula.
 *
 * O jogo opera em ciclos interativos: cada entrada do usuário provoca uma
 * atualização completa do estado do mundo e uma nova renderização visual.
 *
 * Próximos aprimoramentos incluirão validação de entradas, tratamento de erros
 * e extensão do comportamento dos elementos do mundo.
 *
 * Autor: Jefferson Fonseca
 * GitHub: https://github.com/jeffersonfonsecaa
 */



#define CLEAN_COMMAND "cls" //  Para linux use "clear"
#define SIZE_FLAG_MSG 55
#define WORLD_SIZE 5
#define BUFFER_RENDER_SIZE 100
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

char getTileSymbol(const int y, const int x, const struct Tile board[WORLD_SIZE][WORLD_SIZE], const struct Player* p) {
    bool isPlayerHere = (y == p->y && x == p->x),
         isBombHere = (p->bomb.existOnMap && y == p->bomb.y && x == p->bomb.x),
         sameTile = (isPlayerHere && isBombHere);

    if (sameTile) return tolower(p->symbol);
    else if (isPlayerHere) return p->symbol;
    else if (isBombHere) return p->bomb.symbol;
    else return board[y][x].symbol;  
}

void showWorld(const struct Tile board[WORLD_SIZE][WORLD_SIZE], struct Player p) {
    int count = 0;
    char render[BUFFER_RENDER_SIZE];
    
    for (int i=0; i < WORLD_SIZE; i++) {
        render[count++] = '\n';
        
        for (int j=0; j < WORLD_SIZE; j++) {
            render[count++] = ' ';
            render[count++] = getTileSymbol(i, j, board, &p);
            render[count++] = ' ';
        }
    }

    render[count++] = '\n';
    render[count++] = '\0';

    printf("%s\n", render);
}

bool attemptMovePlayer(struct Player* p, const struct Tile board[WORLD_SIZE][WORLD_SIZE], const int dirY, const int dirX) {
    int targetY = p->y + dirY,
        targetX = p->x + dirX;

    if (targetY < 0 || targetY >= WORLD_SIZE || targetX < 0 || targetX >= WORLD_SIZE) return false;

    if (board[targetY][targetX].isWalkable) {
        p->y = targetY;
        p->x = targetX;
        return true;
    }
    
    return false;
}

bool attemptPlaceBomb(struct Player* p) {
    if (!p->bomb.existOnMap) {
        p->bomb.x = p->x;
        p->bomb.y = p->y;
        
        p->bomb.existOnMap = true;
        p->bomb.detonation = false;
        p->bomb.timer = BOMB_TIMER;
        return true;
    }

    return false;
}

void destroyWall(struct Tile board[WORLD_SIZE][WORLD_SIZE], const int explostionTileY, const int explostionTileX) {
    board[explostionTileY][explostionTileX].symbol = WALL_TRASH_SYMBOL;
    board[explostionTileY][explostionTileX].isWalkable = true;
    board[explostionTileY][explostionTileX].canBeDestroyed = false;
}

void updateWorld(struct Tile board[WORLD_SIZE][WORLD_SIZE], struct Player* p) {
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
    }
    
}

bool userInput(struct Tile board[WORLD_SIZE][WORLD_SIZE], struct Player* p, char flag[SIZE_FLAG_MSG]) {
    char input;

    scanf(" %c", &input);
    getchar();
    
    switch (tolower(input)) {
        case 'w':
            attemptMovePlayer(p, board, -1, 0);
            break;
        
        case 's':
            attemptMovePlayer(p, board, 1, 0);
            break;
        
        case 'a':
            attemptMovePlayer(p, board, 0, -1);
            break;
        
        case 'd':
            attemptMovePlayer(p, board, 0, 1);
            break;
    
        case 'b':
            if (!attemptPlaceBomb(p)) {
                strcpy(flag, "Nao e possivel colocar mais de uma bomba...\n");
            }
            break;
    
        case 'q':
            return false;
        
        default:
            strcpy(flag, "Opcao invalida!\n");
            break;
        }

    return true;
}

void runGame() {
    char flag[SIZE_FLAG_MSG] = "";
    bool gameLoop = true;

    struct Player p1;
    p1.symbol = 'P';
    p1.x = 1;
    p1.y = 3;
    p1.bomb.symbol = 'o';
    p1.bomb.detonation = false;
    p1.bomb.timer = BOMB_TIMER;
    
    struct Tile logicWorldLayer[WORLD_SIZE][WORLD_SIZE];

    char visualWorldLayer[WORLD_SIZE][WORLD_SIZE] = {
        {'#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', '#'},
        {'#', ' ', '#', ' ', '#'},
        {'#', ' ', '#', ' ', '#'},
        {'#', '#', '#', '#', '#'},
    };

    createWorld(visualWorldLayer, logicWorldLayer);

    
    do {
        updateWorld(logicWorldLayer, &p1);

        system(CLEAN_COMMAND);
        showWorld(logicWorldLayer, p1);

        printf("%s", flag);
        strcpy(flag, "");

        printf("Use Q para sair\nUse W, A, S, D para mover\nOu B para soltar a bomba: ");

        bool playerChoose = userInput(logicWorldLayer, &p1, flag);

        if(!playerChoose) {
            gameLoop = false;
            printf("\nObrigado por jogar!\n");
        }
            
    } while (gameLoop);

}

int main() {
    runGame();
    return 0;
}
