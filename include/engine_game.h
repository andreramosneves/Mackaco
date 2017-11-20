/****
Andre Ramos Neves - 41445368
Andre Augusto Miguel - 41425340
*****/

#ifndef ENGINE_GAME_H
#define ENGINE_GAME_H

#define N 5
#define SOMA 0
#define SUBTR 1
#define MULT 2

#define COMPLETED 1
#define UNCOMPLETED 2

#define CORRETA 1
#define ERRADA 0

int eng_matriz[N - 2][N];

void geraMatrizAleatoria();
void trocaIndices(int row,int col,int row2, int col2);
void timeOut();
void resetGame();

int RandomInt(int Lowest, int Highest);
int verificaResposta();
int corRespostaCertaErrada(int row);
int isFinalizado();


char* getSinal(int linha, int col);




#endif // ENGINE_GAME_H
