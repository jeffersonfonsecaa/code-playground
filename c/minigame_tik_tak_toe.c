#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/*
 * Jogo da Velha simples em C
 * 
 * Implementa a lógica básica do jogo entre dois jogadores humanos,
 * exibindo o tabuleiro no terminal e verificando condições de vitória.
 * 
 * Futuramente serão adicionadas melhorias na validação de entradas,
 * no tratamento de erros e na clareza semântica do código.
 * 
 * Autor: Jefferson Fonseca
 * GitHub: https://github.com/jeffersonfonsecaa
 */



struct player {
    char name[9];
    char simbol;
};

void createBoard(char board[3][3]) {
    for (int i=0; i < 3; i++) {
        for (int j=0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }

}

void showBoard(const char board[3][3]) {
    for (int i=0; i < 3; i++) {
        printf("\n");
        for (int j=0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) printf("|");
        }
        printf("");
        if (i < 2) printf("\n---+---+---");
    }
    
    printf("\n\n");
}

bool winCheck(const char board[3][3]) {
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

int main() {
    char boardGame[3][3];
    int row, col;

    int input = 0,
        playerTurn = 0;
    bool gameLoop = true;
    
    
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
                printf("%s (%c), escolha uma coluna valida para jogar: ", players[playerTurn].name, players[playerTurn].simbol);
                scanf("%d", &input);
            } while (input < 1 || input > 3);
    
            col = input-1;
            
            do {
                printf("%s (%c), escolha uma linha valida para jogar: ", players[playerTurn].name, players[playerTurn].simbol);
                scanf("%d", &input);
            } while (input < 1 || input > 3);
    
            row = input-1;

            if (boardGame[row][col] != ' ') {
                showBoard(boardGame);
                printf("\n\tEste lugar esta ocupado, escolha outro!\n");
            }

        } while (boardGame[row][col] != ' ');

        boardGame[row][col] = players[playerTurn].simbol;


        if (winCheck(boardGame)) {
            showBoard(boardGame);

            printf("\n***** %s venceu! *****\n", players[playerTurn].name);

            do {
                printf("Deseja jogar novamente? (1 - Sim | 0 - Nao): ");
                scanf("%d", &input);

                switch (input) {
                    case 0:
                        printf("\nObrigado por jogar!\n");
                        gameLoop = false;
                        break;
                    
                    case 1:
                        createBoard(boardGame);
                        playerTurn = 1;
                        break;

                    default:
                        printf("Opcao invalida...\n");
                        break;
                }
            } while (input != 0 && input != 1);

        }

        
        playerTurn = (playerTurn == 0 ? 1 : 0);

    } while (gameLoop);

    return 0;
}
