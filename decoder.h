#ifndef DECODER_H_INCLUDED
#define DECODER_H_INCLUDED

#include "codecbase.h"

//Decodifica um arquivo binário para texto
void decode(FILE* inFile, FILE* outFile, struct encoder* dec);

#endif // DECODER_H_INCLUDED
