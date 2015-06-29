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
    unsigned short int indexBits;  //n�mero de bits do c�digo - tamanho das palavras
    char** dictionary;  //dicion�rio de palavras
    unsigned int nextCode; //pr�ximo �ndice do c�digo a ser inserido
    char strBuffer[MAX_STR_SIZE]; //acumula a string de caracteres sendo lida do arquivo
    unsigned int* outBuffer; //buffer de sa�da do compressor
    long int outBufferSize; //n�mero de palavras do buffer de sa�da
	int dLZWupdate[100]; //vetor de update do tamanho da palavra - armazena os pontos de aumento de tamanho
};

//Aloca a mem�ria necess�ria para o dicion�rio
void allocate_dictionary(struct encoder* enc, int wordMaxSize);
//Insere uma palavra no dicion�rio
void insert_into_dictionary(struct encoder* enc, const char* str);
//Carrega os c�digos ASCII at� 127 no dicion�rio
void load_ascii (struct encoder* enc);
//Configura o codificador ou decodificador para come�ar
void initialize_codec(struct encoder* enc, int wordMaxSize);

#endif // CODECBASE_H_INCLUDED
