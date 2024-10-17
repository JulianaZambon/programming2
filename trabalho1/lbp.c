/* Autora: Juliana Zambon (jz22), GRR20224168 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <dirent.h>
#include "lib.h"

int main(int argc, char *argv[])
{
    int opt;
    char *arquivo_entrada = NULL;
    // char *diretorio = NULL;
    // char *arquivo_saida = NULL;
    FILE *arquivo = NULL;
    // int aux = 0;

    while ((opt = getopt(argc, argv, "d:i:o:")) != -1)
    {
        switch (opt)
        {
        // case 'd':
        //     diretorio = strdup(optarg);
        //     break;
        case 'i':
            arquivo_entrada = optarg;
            break;
        // case 'o':
        //     arquivo_saida = optarg;
        //     break;
        default:
            fprintf(stderr, "Uso: %s -d <diretório> -i <arquivo_entrada> -o <arquivo_saida>\n", argv[0]);
            exit(1);
        }
    }

    // if (diretorio == NULL || arquivo_entrada == NULL || arquivo_saida == NULL)
    // {
    //     fprintf(stderr, "Uso: %s -d <diretório> -i <arquivo_entrada> -o <arquivo_saida>\n", argv[0]);
    //     exit(EXIT_FAILURE);
    // }

    /* Lê o arquivo de entrada */
    arquivo = fopen(arquivo_entrada, "r"); /* r = read */
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada\n");
        free(arquivo_entrada);
        exit(1);
    }

    /* Aloca memória para a imagem */
    struct imagemPGM *img = alocar_imagem();
    // /* Lê a imagem */
    img = ler_imagem(arquivo, img);
    if (img == NULL)
    {
        fprintf(stderr, "Erro ao ler a imagem\n");
        free(arquivo_entrada);
        fclose(arquivo);
        exit(1);
    }

    /* Aloca memória para a nova imagem */
    struct imagemPGM *nova_img = alocar_imagem();
    /* Inicializa a nova imagem */
    inicializar_nova_imagem(nova_img, img);

    /* Gerar a imagem LBP */
    /*
     Exemplo:
     ./lbp -i img1.tif -o img_out.tif
     Nesse caso, o programa recebe como entrar imagem img1.tif e gerar a imagem LPB
     com o nome informado pela opção -o. NENHUMA saída é esperada no terminal; caso
     algum erro ocorra, basta não criar a imagem de saída.
    */

    /* Gera a imagem LBP */
    gerar_lbp(img, nova_img);

    // printf("gerando lbp:\n");

    // for (int i = 0; i < nova_img->altura; i++)
    // {
    //     for (int j = 0; j < nova_img->largura; j++)
    //     {
    //         printf("%d ", nova_img->pixels[i][j]);
    //     }
    //     printf("\n");
    // }

    /* Gera a imagem de saída */
    gerar_imagem_saida(nova_img, stdout);
    // if (aux)
    // {
    //     FILE *arquivo_saida;
    //     if (!(arquivo_saida = fopen(arquivo_saida, "wb"))) /* wb = write binary */
    //     {
    //         fprintf(stderr, "Erro ao abrir o arquivo de saída\n");
    //         exit(EXIT_FAILURE);
    //     }
    //     gerar_imagem_saida(nova_img, arquivo_saida);
    // }

    /* Comparar uma imagem de teste com todas as imagens da base de referência */
    /*  A saída deve ser EXATAMENTE a seguinte
        Imagem mais similar: <img mais similar> <distância>
    */

    /* Exibir a imagem mais similar e a distância */
    /* printf("Imagem mais similar: %s %.6f\n", img_similar, distancia); */

    /*
        em que <img mais similar> é a imagem com a menor distância Euclidiana encontrada
        no diretório ./base (informar apenas o nome do arquivo com sua extensão) e
        <distância> é o valor da distância associada a esta imagem, um número com
        EXATAMENTE 6 casas decimais. Nome e valor devem ser separados APENAS por
        um espaço.
    */

    // struct LBPHistograma *histograma = alocar_histograma();
    // gerar_histograma(nova_img, histograma, arquivo_entrada);
    fclose(arquivo); /* fecha o arquivo de entrada */

    /* percorre o diretório */
    // double distancia = 1e12;
    // char menor_distancia[256];

    /* ERRO A PARTIR DAQUI */
    // ler_diretorio(diretorio);
    //  encontrar_imagem_similar(diretorio, histograma, &distancia, menor_distancia);

    // /* Libera a memória alocada */
    // free (arquivo_entrada);
    // free (arquivo_saida);
    // free (diretorio);
    // liberar_imagem(img);
    // liberar_imagem(nova_img);
    // free(histograma);

    return 0;
}
