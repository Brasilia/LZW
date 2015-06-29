#ifndef CODECBASE_H_INCLUDED
#define CODECBASE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STR_SIZE 100
#define HEADER_BYTES sizeof(short int)
#define ENCODE "encode"
#define DECODE "decode"

static const short int G_MAX_indexBits = 22; //supports up to G_MAX_indexBits bit word size

//Armazena atributos do codificador
struct encoder {
    unsigned short int indexBits;  //número de bits do código - tamanho das palavras
    char** dictionary;  //dicionário de palavras
    unsigned int nextCode; //próximo índice do código a ser inserido
    char strBuffer[MAX_STR_SIZE]; //acumula a string de caracteres sendo lida do arquivo
    unsigned int* outBuffer; //buffer de saída do compressor
    long int outBufferSize; //número de palavras do buffer de saída
	int dLZWupdate[100]; //vetor de update do tamanho da palavra - armazena os pontos de aumento de tamanho
};

//Aloca a memória necessária para o dicionário
void allocate_dictionary(struct encoder* enc, int wordMaxSize);
//Insere uma palavra no dicionário
void insert_into_dictionary(struct encoder* enc, const char* str);
//Carrega os códigos ASCII até 127 no dicionário
void load_ascii (struct encoder* enc);
//Configura o codificador ou decodificador para começar
void initialize_codec(struct encoder* enc, int wordMaxSize);

#endif // CODECBASE_H_INCLUDED
