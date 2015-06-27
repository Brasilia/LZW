#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

#include "codecbase.h"

//Verifica se o difionário do codificador contém a palavra especificada
int enc_dictionary_contains(struct encoder* enc, char* word);
//Adiciona à memória os códigos de output
void add_output (struct encoder* enc, int output);
//Imprime os códigos no arquivo de saída
void write_to_output(FILE* outFile, struct encoder* enc);
//Codifica um arquivo de texto em binário, comprimido
void encode(FILE* inFile, FILE* outFile, struct encoder* enc);

#endif // CODER_H_INCLUDED
