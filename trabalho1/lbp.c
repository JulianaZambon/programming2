#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <getopt.h>
#include "lib.h"

int main(int argc, char *argv[]) {
    // parsear argumentos de linha de comando (use getopt)
    getopt(argc, argv, "i:r:o:");
        
    // calcular o LBP da imagem de teste
    void lbp(struct imagemPGM *img, int **lbp);
    
    // Para cada imagem na base de referência:
    //     verificar se o arquivo .lbp existe
    //     se não, calcular e salvar o vetor LBP
    //     calcular a distância Euclidiana entre os vetores LBP


    
    
    // exibir a imagem com a menor distância == imagem mais similar


    // gerar a imagem LBP
    
    return 0;
}
