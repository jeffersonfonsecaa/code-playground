#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


/*
 * Jogo da Velha simples em C
 * 
 * Implementa a lógica básica do jogo entre dois jogadores humanos,
 * exibindo o tabuleiro no terminal e verificando condições de vitória e empate.
 * 
 * Futuramente serão adicionadas melhorias na validação de entradas,
 * no tratamento de erros e na clareza semântica do código.
 * 
 * Autor: Jefferson Fonseca
 * GitHub: https://github.com/jeffersonfonsecaa
 */


#define BOARD_SIZE 3
#define MAX_MOVES 9


struct player {
    char name[9];
    char symbol;
};

void createBoard(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i=0; i < 3; i++) {
        for (int j=0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }

}

void showBoard(const char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i=0; i < 3; i++) {
        printf("\n");
        for (int j=0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) printf("|");
        }
        if (i < 2) printf("\n---+---+---");
    }
    
    printf("\n\n");
}

bool winCheck(const char board[BOARD_SIZE][BOARD_SIZE]) {
    int row1, row2, row3,
        col1, col2, col3;

    char winCombinations[8][3][2] = {
        {{0,0}, {0,1}, {0,2}},  // Linha 1
        {{1,0}, {1,1}, {1,2}},  // Linha 2
        {{2,0}, {2,1}, {2,2}},  // Linha 3
        {{0,0}, {1,0}, {2,0}},  // Coluna 1
        {{0,1}, {1,1}, {2,1}},  // Coluna 2
        {{0,2}, {1,2}, {2,2}},  // Coluna 3
        {{0,0}, {1,1}, {2,2}},  // Primeira diagonal
        {{0,2}, {1,1}, {2,0}}   // Segunda diagonal
    };

    for (int i=0; i < 8; i++) {
        row1 = winCombinations[i][0][0];
        col1 = winCombinations[i][0][1];
        
        row2 = winCombinations[i][1][0];
        col2 = winCombinations[i][1][1];

        row3 = winCombinations[i][2][0];
        col3 = winCombinations[i][2][1];

        if (board[row1][col1] != ' ' && board[row1][col1] == board[row2][col2] && board[row2][col2] == board[row3][col3]) {
            return true;
        }
    }
    
    return false;
}

bool restartGameInputChoice() {
    int input;

    do {
        printf("Deseja jogar novamente? (1 - Sim | 0 - Nao): ");
        scanf("%d", &input);

        switch (input) {
            case 0:
                printf("\nObrigado por jogar!\n");
                return false;
                break;
            
            case 1:
                return true;
                break;

            default:
                printf("Opcao invalida...\n");
                break;
        }

    } while (input != 0 && input != 1);

}

void runGame() {
    char boardGame[BOARD_SIZE][BOARD_SIZE];
    int row, col;
    
    int input = 0,
        playerTurn = 0,
        totalMovements = 0;
    
    bool gameLoop = true,
         gameFinish = false;
    
    struct player players[2] = {
        {"Player 1", 'X'},
        {"Player 2", 'O'}
    };
    
    
    createBoard(boardGame);
    printf("\nWelcome to tik tak toe!\n");
    
    do {
        showBoard(boardGame);
    
        do { // Player input
            do {
                printf("%s (%c), escolha uma coluna valida para jogar: ", players[playerTurn].name, players[playerTurn].symbol);
                scanf("%d", &input);
            } while (input < 1 || input > 3);
    
            col = input-1;
            
            do {
                printf("%s (%c), escolha uma linha valida para jogar: ", players[playerTurn].name, players[playerTurn].symbol);
                scanf("%d", &input);
            } while (input < 1 || input > 3);
    
            row = input-1;
    
            if (boardGame[row][col] != ' ') {
                showBoard(boardGame);
                printf("\n\tEste lugar esta ocupado, escolha outro!\n");
            }
    
        } while (boardGame[row][col] != ' ');
    
        boardGame[row][col] = players[playerTurn].symbol;
        totalMovements += 1;
    
    
        if (winCheck(boardGame)) {
            showBoard(boardGame);
    
            printf("\n***** %s venceu! *****\n", players[playerTurn].name);
            gameFinish = true;
    
        } else {
            if (totalMovements == MAX_MOVES) {
                showBoard(boardGame);
                
                printf("\n***** Empate! *****\n");
                gameFinish = true;
            }
    
        }
        
        if (gameFinish) {
            if (restartGameInputChoice()) {
                totalMovements = 0;
                playerTurn = 0;
                gameFinish = false;

                createBoard(boardGame);
            } else {
                gameLoop = false;
            }  
            
            continue;
        }
    
        playerTurn = (playerTurn == 0 ? 1 : 0);
    
    } while (gameLoop);

}


int main() {
    runGame();
    return 0;
}
