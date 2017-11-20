/****
Andre Ramos Neves - 41445368
Andre Augusto Miguel - 41425340
*****/

#include "../include/engine_game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int ja_completou = 0;

void trocaIndices(int row,int col,int row2, int col2);

void geraMatrizAleatoria(){

    srand(time(NULL));
    int i,j;
    for (i = 0; i < N - 1  ; i++){
        for (j = 0; j < N - 1 ; j++){
              if( j == 1 ){
                eng_matriz[i][j] = RandomInt(0,12) % 3;
              }else{
                eng_matriz[i][j] = RandomInt(0,10);
             }
        }

    }

    for (i = 0; i < N - 1 ; i++){
        if (eng_matriz[i][1] == SOMA)
            eng_matriz[i][N - 1] = eng_matriz[i][0] + eng_matriz[i][2];
        else if(eng_matriz[i][1] == SUBTR)
            eng_matriz[i][N - 1] = eng_matriz[i][0] - eng_matriz[i][2];
         else if(eng_matriz[i][1] == MULT)
            eng_matriz[i][N - 1] = eng_matriz[i][0] * eng_matriz[i][2];
    }


    srand(time(NULL));

    if(RandomInt(1,10) % 2 == 0 )
        trocaIndices(0,4,1,4);
    if(RandomInt(1,10) % 2 == 0 )
        trocaIndices(1,4,2,4);
    if(RandomInt(1,10) % 2 == 0 )
        trocaIndices(0,4,2,4);


}
int verificaResposta(){
    int i = 0;
    for (i = 0; i < N - 2 ; i++){
        if (eng_matriz[i][1] == SOMA){
            if(eng_matriz[i][3] != (eng_matriz[i][0] + eng_matriz[i][2])){
                return UNCOMPLETED;
            }
        }
        else if (eng_matriz[i][1] == SUBTR){
            if(eng_matriz[i][3] != (eng_matriz[i][0] - eng_matriz[i][2])){
                return UNCOMPLETED;
            }
        }
        else if (eng_matriz[i][1] == MULT){
            if(eng_matriz[i][3] != (eng_matriz[i][0] * eng_matriz[i][2])){
                return UNCOMPLETED;
            }
        }
    }
    ja_completou = 1;
    return COMPLETED;
}

int corRespostaCertaErrada(int row){
    int i = row;
    if (eng_matriz[i][1] == SOMA){
        if(eng_matriz[i][3] != (eng_matriz[i][0] + eng_matriz[i][2])){
            return ERRADA;
        }
    }
    else if (eng_matriz[i][1] == SUBTR){
        if(eng_matriz[i][3] != (eng_matriz[i][0] - eng_matriz[i][2])){
            return ERRADA;
        }
    }
    else if (eng_matriz[i][1] == MULT){
        if(eng_matriz[i][3] != (eng_matriz[i][0] * eng_matriz[i][2])){
            return ERRADA;
        }
    }
    return CORRETA;
}

void trocaIndices(int row,int col,int row2, int col2){
    int temp = eng_matriz[row2][col2];

    eng_matriz[row2][col2] = eng_matriz[row][col];

    eng_matriz[row][col] = temp;

}

char* getSinal(int linha, int col){
     if (eng_matriz[linha][col] == SOMA)
     return "+";
     else if(eng_matriz[linha][col] == SUBTR)
     return "-";
     else if(eng_matriz[linha][col] == MULT)
     return "X";
     else
     return "f";

}

int RandomInt(int Lowest, int Highest)
{
    return (rand() % (Highest-Lowest) + Lowest);
}
int isFinalizado()
{
    if(ja_completou == 1){
        return COMPLETED;
    }else{
        return UNCOMPLETED;
    }
}

void resetGame(){
    geraMatrizAleatoria();
    ja_completou = 0;
}

void timeOut(){
    ja_completou = 1;
}



