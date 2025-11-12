#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

// Assimilando um nome simbolico para um valor constante, fazer isso torna a manutenção do código melhor

// Constantes do jogo
#define BOARD_SIZE 10
#define SKILL_SIZE 5

// Estados das Células
#define WATER 0
#define SHIP 1
#define SKILL_ON_WATER 2
#define SKILL_ON_SHIP 3

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
            switch (board[i][j])
            {
            case WATER:
                printf(" ~ "); // Água
                break;
            case SHIP:
                printf(" N "); // Navio
                break;
            case SKILL_ON_WATER:
                printf(" * "); // Habilidade na água
                break;
            case SKILL_ON_SHIP:
                printf(" X "); // Habilidade acertou um navio
                break;
            default:
                printf(" ? ");
            }
        }
        // Finalizando com a quebra de linha
        printf("\n");
    }
    // Pula uma linha para dar um espaçamento
    printf("\n");
}

// Função auxiliar simples para pegar o maior de dois inteiros
int maximum(int a, int b)
{
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

    // CONCEITO CHAVE: Calculando a "distância" entre início e fim
    // deltaRow = quantas linhas o navio vai ocupar (pode ser negativo se for "para cima")
    // deltaCol = quantas colunas o navio vai ocupar (pode ser negativo se for "para esquerda")
    // Exemplo: de A1 para C3 -> deltaRow = 2, deltaCol = 2 (diagonal)
    // Exemplo: de A1 para A3 -> deltaRow = 2, deltaCol = 0 (vertical)
    int deltaRow = endRow - startRow;
    int deltaCol = endCol - startCol;

    // VALIDAÇÃO DE ORIENTAÇÃO: O navio só pode ser colocado em 3 direções
    // 1. Horizontal: deltaRow = 0 (mesmo número da linha)
    // 2. Vertical: deltaCol = 0 (mesma letra da coluna)
    // 3. Diagonal: abs(deltaRow) = abs(deltaCol) (movimento igual em ambos os eixos)
    // Exemplo VÁLIDO diagonal: A1 para C3 -> abs(2) = abs(2) ✓
    // Exemplo INVÁLIDO: A1 para C2 -> abs(1) ≠ abs(2) ✗
    if (deltaRow != 0 && deltaCol != 0 && abs(deltaRow) != abs(deltaCol))
    {
        printf("Erro: O navio so pode ser posicionado na horizontal, vertical ou diagonal (45 graus).\n");
        return 0;
    }

    // CÁLCULO DO TAMANHO: Pega a maior "distância" entre início e fim
    // Para horizontal/vertical: um dos deltas é 0, então pega o que não é 0
    // Para diagonal: ambos são iguais, então tanto faz qual pegar
    // Exemplo: A1 até A3 -> abs(2) e abs(0) -> máximo é 2 -> tamanho = 3 células
    int shipSize = maximum(abs(deltaRow), abs(deltaCol)) + 1;

    // SISTEMA DE "PASSOS": Como andar célula por célula do início ao fim
    // rowStep: +1 = desce, -1 = sobe, 0 = fica na mesma linha
    // colStep: +1 = vai pra direita, -1 = vai pra esquerda, 0 = fica na mesma coluna
    // Exemplo: A1→C3 = rowStep=+1, colStep=+1 (desce e vai pra direita)
    int rowStep = (deltaRow == 0) ? 0 : (deltaRow > 0 ? 1 : -1);
    int colStep = (deltaCol == 0) ? 0 : (deltaCol > 0 ? 1 : -1);

    // VALIDAÇÃO DE SOBREPOSIÇÃO: Verifica se já existe navio no caminho
    // "Simulamos" colocar o navio, andando célula por célula
    // Para cada posição que o navio ocuparia, verificamos se está livre
    for (int i = 0; i < shipSize; i++)
    {
        // Calcula onde estaria a i-ésima célula do navio
        // Exemplo: se i=0, fica na posição inicial. Se i=1, anda 1 passo, etc.
        int currentRow = startRow + (i * rowStep);
        int currentCol = startCol + (i * colStep);
        
        // Se essa posição já tem navio (valor 1), não pode colocar
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

// Habilidades

// Inicializa em 0 as áreas não afetadas
void initSkillMatrix(int matrix[SKILL_SIZE][SKILL_SIZE])
{
    for (int i = 0; i < SKILL_SIZE; i++)
    {
        for (int j = 0; j < SKILL_SIZE; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

void createCross(int matrix[SKILL_SIZE][SKILL_SIZE])
{
    int center = SKILL_SIZE / 2;
    for (int i = 0; i < SKILL_SIZE; i++) {
        for (int j = 0; j < SKILL_SIZE; j++) {
            if (i == center || j == center) {
                matrix[i][j] = 1;
            }
        }
    }
}

void createOctahedron(int matrix[SKILL_SIZE][SKILL_SIZE])
{
    int center = SKILL_SIZE / 2; // Centro da matriz 5x5 = posição (2,2)
    
    // ALGORITMO DO LOSANGO: Usa "distância Manhattan"
    // Distância Manhattan = soma das distâncias absolutas em cada eixo
    // Exemplo: distância de (2,2) até (0,2) = |0-2| + |2-2| = 2 + 0 = 2
    // Se a distância for <= centro (2), então faz parte do losango
    for (int i = 0; i < SKILL_SIZE; i++) {
        for (int j = 0; j < SKILL_SIZE; j++) {
            // Calcula a distância Manhattan desta posição até o centro
            int distancia = abs(i - center) + abs(j - center);
            if (distancia <= center) {
                matrix[i][j] = 1; // Marca como área afetada
            }
        }
    }
}

void createCone(int matrix[SKILL_SIZE][SKILL_SIZE])
{
    int center = SKILL_SIZE / 2; // Centro = 2
    
    // ALGORITMO DO CONE: Forma triangular que "aponta para cima"
    // Duas condições devem ser atendidas:
    // 1. i <= center: só afeta as linhas da metade superior (linhas 0, 1, 2)
    // 2. abs(j - center) <= (center - i): largura diminui conforme sobe
    //
    // Exemplo linha por linha:
    // Linha 0 (i=0): largura = 2-0 = 2 → afeta colunas 1,2,3 (centro ± 2)
    // Linha 1 (i=1): largura = 2-1 = 1 → afeta colunas 1,2,3 (centro ± 1)  
    // Linha 2 (i=2): largura = 2-2 = 0 → afeta apenas coluna 2 (centro ± 0)
    for (int i = 0; i < SKILL_SIZE; i++) {
        for (int j = 0; j < SKILL_SIZE; j++) {
            // Verifica se está na parte superior E dentro da largura permitida
            if (i <= center && abs(j - center) <= (center - i)) {
                matrix[i][j] = 1;
            }
        }
    }
}

void applySkill(int board[BOARD_SIZE][BOARD_SIZE], int skill[SKILL_SIZE][SKILL_SIZE], int originRow, int originCol)
{
    int center = SKILL_SIZE / 2; // Centro da matriz de habilidade = posição (2,2)
    printf("Aplicando habilidade em (%c%d)...\n", 'A' + originCol, originRow + 1);

    // MAPEAMENTO DE COORDENADAS: Converte da matriz pequena (5x5) para o tabuleiro grande (10x10)
    // A ideia é que o centro da habilidade (2,2) corresponda ao ponto escolhido pelo jogador
    
    // Itera na matriz de habilidade (linha)
    for (int i = 0; i < SKILL_SIZE; i++) 
    {
        // Itera na matriz de habilidade (coluna)
        for (int j = 0; j < SKILL_SIZE; j++) 
        {
            // Se esta parte da habilidade afeta uma área...
            if (skill[i][j] == 1) 
            {
                // CONVERSÃO DE COORDENADAS: 
                // Se estamos na posição (i,j) da matriz de habilidade,
                // qual posição isso representa no tabuleiro principal?
                //
                // Exemplo: jogador escolheu E5 (posição 4,4 no array)
                // - Centro da habilidade (2,2) → vai para E5 (4,4)
                // - Posição (0,2) da habilidade → vai para C5 (2,4) 
                // - Posição (1,1) da habilidade → vai para D4 (3,3)
                // Fórmula: tabuleiro = origem + (posição_habilidade - centro)
                int board_r = originRow + (i - center);
                int board_c = originCol + (j - center);

                // VERIFICAÇÃO DE LIMITES: Garante que não saia do tabuleiro
                // Algumas partes da habilidade podem "vazar" para fora do tabuleiro
                if (board_r >= 0 && board_r < BOARD_SIZE && board_c >= 0 && board_c < BOARD_SIZE)
                {
                    // APLICAÇÃO DO EFEITO: Muda o estado da célula baseado no que tinha antes
                    if (board[board_r][board_c] == WATER) {
                        board[board_r][board_c] = SKILL_ON_WATER; // Água vira "tiro na água"
                    }
                    else if (board[board_r][board_c] == SHIP) {
                        board[board_r][board_c] = SKILL_ON_SHIP;  // Navio vira "navio atingido"
                    }
                    // Se já foi atingido antes, não muda novamente
                }
            }
        }
    }
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

            // CONVERSÃO DE COORDENADAS: Do formato humano (A1) para índices de array (0,0)
            // 
            // Para COLUNAS: 'A' vira 0, 'B' vira 1, 'C' vira 2, etc.
            // Método: pega o código ASCII da letra e subtrai o código ASCII de 'A'
            // Exemplo: 'C' (código 67) - 'A' (código 65) = 2
            // toupper() garante que funcione mesmo se o usuário digitar minúscula
            startCol = toupper(startColChar) - 'A';
            endCol = toupper(endColChar) - 'A';
            
            // Para LINHAS: usuário digita 1-10, mas array usa índices 0-9
            // Então subtraímos 1 para fazer a conversão
            // Exemplo: usuário digita linha 5 → array usa índice 4
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

    printf("\nPosicionamento concluido!\n");
    updateBoard(gameBoard);

    printf("\nAgora, vamos usar as Habilidades.\n");
    printf("Voce quer usar alguma habilidade? 1 para SIM / 0 para NAO\n");
    scanf("%i", &playerChoice);

    while (playerChoice == 1)
    {
        int skillType = 0;
        int originRow = 0;
        int originCol = 0;
        char originColChar;

        printf("Qual habilidade deseja usar?\n");
        printf("1. Cone\n2. Cruz\n3. Octaedro\n");
        printf("Escolha: ");
        scanf("%d", &skillType);

        if (skillType < 1 || skillType > 3) {
            printf("Escolha invalida.\n");
            // Pula para o início do loop 'while'
            continue; 
        }

        printf("Digite a coordenada de ORIGEM da habilidade (ex: E5): ");
        scanf(" %c%i", &originColChar, &originRow);

        // CONVERSÃO DE COORDENADAS (mesma lógica explicada anteriormente)
        originCol = toupper(originColChar) - 'A';  // Letra para número: A=0, B=1, etc.
        originRow--;  // Usuário digita 1-10, mas array usa 0-9 

        // CRIAÇÃO DA MATRIZ DE HABILIDADE: 
        // 1. Primeiro criamos uma matriz 5x5 vazia (tudo = 0)
        // 2. Depois preenchemos com o padrão da habilidade escolhida (1 = afeta, 0 = não afeta)
        int skillMatrix[SKILL_SIZE][SKILL_SIZE];
        initSkillMatrix(skillMatrix); // Limpa a matriz (preenche com 0s)

        // Escolhe qual padrão geométrico usar baseado na escolha do usuário
        switch (skillType)
        {
            case 1:
                createCone(skillMatrix);      // Padrão triangular
                break;
            case 2:
                createCross(skillMatrix);     // Padrão de cruz (+)
                break;
            case 3:
                createOctahedron(skillMatrix); // Padrão de losango (◊)
                break;
        }

        // Define a origem e sobrepõe no tabuleiro
        applySkill(gameBoard, skillMatrix, originRow, originCol);

        // Exibe o tabuleiro atualizado
        updateBoard(gameBoard);

        printf("\nVoce quer usar outra habilidade? 1 para SIM / 0 para NAO\n");
        scanf("%i", &playerChoice);
    }

    printf("\nTabuleiro Final:\n");
    updateBoard(gameBoard);

    return 0;
}
