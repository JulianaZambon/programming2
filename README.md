# Programação II (CI1002) - Trabalho Prático 01: Estratégias de Comparação de Imagens

**Autora:** Juliana Zambon (jz22), GRR20224168

Este repositório contém o código desenvolvido para o Trabalho Prático 01 da disciplina **CI1002 - Programação II**,
oferecida pelo Departamento de Informática da Universidade Federal do Paraná (UFPR).

## Descrição

O objetivo deste trabalho é implementar um programa em C capaz de comparar uma imagem de teste com um conjunto de
imagens de referência e identificar a imagem mais similar utilizando a **Distância Euclidiana**.
Além disso, o programa gera uma imagem baseada na técnica **Local Binary Pattern (LBP)**.

## Funcionalidades

- **Comparação de Imagens:** O programa compara a imagem de teste com todas as imagens da base de referência e retorna
a imagem com a menor distância Euclidiana encontrada.
- **Geração de Imagem LBP:** O programa também gera a imagem LBP da imagem de teste.

## Estrutura

O código-fonte está estruturado em arquivos `.c` e `.h` que agrupam as funcionalidades, sendo `lbp.c` o arquivo principal de aplicação, `lib.c` o arquivo de implementação, e `lib.h` o arquivo de cabeçalho.

## Arquivo Cabeçalho (lib.h)

O arquivo `lib.h` contém as estruturas e protótipos de funções necessárias para o processamento de imagens no formato PGM e para o cálculo do histograma LBP. Ele define as funções responsáveis por ler, manipular e comparar as imagens, bem como gerar o vetor LBP e realizar as operações relacionadas à distância Euclidiana.
