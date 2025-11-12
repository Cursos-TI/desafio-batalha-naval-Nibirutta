#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// Assimilando um nome simbolico para um valor constante, fazer isso torna a manutenção do código melhor
#define BOARD_SIZE 10

void updateBoard(int board[BOARD_SIZE][BOARD_SIZE])
{
    // Dando um espaçamento inicial para o tabuleiro
    // E imprimindo as letras referentes as colunas
    printf("\n   ");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf(" %c ", 'A' + i);
    }
    printf("\n");

    // Iterando sobre as linhas e colunas para imprimir o tabuleiro
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        // Coloquei o 2 ali no printf para reserva um espaço de 2 digitos
        printf("%2i ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            // Caso a celula tenha um navio, optei por colocar N
            // Caso contrario, coloquei o simbolo ~ para representar a agua
            if (board[i][j] == 1)
            {
                printf(" N ");
            }
            else
            {
                printf(" ~ ");
            }
        }
        // Finalizando a ultima linha
        printf("\n");
    }
    // Pula uma linha para dar um espaçamento
    printf("\n");
}

// Função auxiliar simples para pegar o maior de dois inteiros
int maximum(int a, int b) {
    return (a > b) ? a : b;
}

int placeShip(int board[BOARD_SIZE][BOARD_SIZE], int startRow, int startCol, int endRow, int endCol)
{
    // Validacao de limite nas coordenadas, dentro do espaco do tabuleiro
    if (startRow < 0 || startRow >= BOARD_SIZE || startCol < 0 || startCol >= BOARD_SIZE ||
        endRow < 0 || endRow >= BOARD_SIZE || endCol < 0 || endCol >= BOARD_SIZE)
    {
        printf("Erro: Posicao fora do tabuleiro.\n");
        return 0;
    }

    int deltaRow = endRow - startRow;
    int deltaCol = endCol - startCol;

    // Validacao de orientacao (horizontal, vertical ou diagonal 45 graus)
    // Se for horizontal, deltaRow eh 0
    // Se for vertical, deltaCol eh 0
    // Se for diagonal, abs(deltaRow) deve ser igual a abs(deltaCol)
    if (deltaRow != 0 && deltaCol != 0 && abs(deltaRow) != abs(deltaCol))
    {
        printf("Erro: O navio so pode ser posicionado na horizontal, vertical ou diagonal (45 graus).\n");
        return 0;
    }

    // O tamanho eh o maior dos deltas (em modulo) + 1
    int shipSize = maximum(abs(deltaRow), abs(deltaCol)) + 1;

    // Determina a direção do "passo" para linha e coluna
    // Se deltaRow > 0, passo eh 1. Se < 0, passo eh -1. Se = 0, passo eh 0.
    int rowStep = (deltaRow == 0) ? 0 : (deltaRow > 0 ? 1 : -1);
    int colStep = (deltaCol == 0) ? 0 : (deltaCol > 0 ? 1 : -1);

    // Validacao de sobreposicao
    // "Andamos" pelo caminho do navio antes de coloca-lo
    for (int i = 0; i < shipSize; i++)
    {
        int currentRow = startRow + (i * rowStep);
        int currentCol = startCol + (i * colStep);
        if (board[currentRow][currentCol] == 1)
        {
            printf("Erro: Ja existe um navio nessa posicao (%c%d).\n", 'A' + currentCol, currentRow + 1);
            return 0;
        }
    }

    // Se todas as validacoes passaram, marca as posicoes no tabuleiro
    for (int i = 0; i < shipSize; i++)
    {
        int currentRow = startRow + (i * rowStep);
        int currentCol = startCol + (i * colStep);
        board[currentRow][currentCol] = 1;
    }

    printf("Navio posicionado com sucesso!\n");
    return 1;
}

int main()
{
    // Inicializa o tabuleiro com 0 em todas as posições (representando água)
    int gameBoard[BOARD_SIZE][BOARD_SIZE] = {0};

    char startColChar, endColChar;
    int startRow, endRow;
    int startCol, endCol;

    printf("Batalha Naval - Posicionamento de Navios\n");
    printf("O tabuleiro eh %dx%d. As linhas vao de 1 a %d e as colunas de A a %c.\n",
           BOARD_SIZE, BOARD_SIZE, BOARD_SIZE, 'A' + BOARD_SIZE - 1);

    int playerChoice = 0;
    printf("\nVoce quer posicionar algum navio no tabuleiro? 1 para SIM / 0 para NAO\n");
    scanf("%i", &playerChoice);

    while (playerChoice == 1)
    {
        printf("\nPosicione um navio nas coordenadas do tabuleiro.\n");

        // Loop para garantir que o usuario insira uma posicao valida
        int placed = 0;
        while (!placed)
        {
            printf("Digite a coordenada inicial (ex: A1): ");
            scanf(" %c%i", &startColChar, &startRow);

            printf("Digite a coordenada final (ex: C3): ");
            scanf(" %c%i", &endColChar, &endRow);

            // Converte as coordenadas do formato 'A1' para indices de matriz (0,0)
            // Subtrai 1 da linha porque o usuario digita de 1 a 10, mas o array vai de 0 a 9
            // Converte a letra da coluna para um indice numerico (A=0, B=1, ...)
            startCol = toupper(startColChar) - 'A';
            endCol = toupper(endColChar) - 'A';
            startRow--;
            endRow--;

            // Tenta posicionar o navio com as coordenadas fornecidas
            placed = placeShip(gameBoard, startRow, startCol, endRow, endCol);
        }

        // Mostra o tabuleiro atualizado apous cada posicionamento
        updateBoard(gameBoard);

        printf("\nVoce ainda quer posicionar algum navio no tabuleiro? 1 para SIM / 0 para NAO\n");
        scanf("%i", &playerChoice);
    }

    printf("\n--- Tabuleiro Final ---\n");
    updateBoard(gameBoard);

    return 0;
}
