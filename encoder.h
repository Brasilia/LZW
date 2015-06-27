#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

#include "codecbase.h"

//Verifica se o difion�rio do codificador cont�m a palavra especificada
int enc_dictionary_contains(struct encoder* enc, char* word);
//Adiciona � mem�ria os c�digos de output
void add_output (struct encoder* enc, int output);
//Imprime os c�digos no arquivo de sa�da
void write_to_output(FILE* outFile, struct encoder* enc);
//Codifica um arquivo de texto em bin�rio, comprimido
void encode(FILE* inFile, FILE* outFile, struct encoder* enc);

#endif // CODER_H_INCLUDED
