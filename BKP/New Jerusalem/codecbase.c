#include "codecbase.h"

//Aloca memória para o dicionário
void allocate_dictionary(struct encoder* enc, int wordMaxSize){
    enc->dictionary = (char**) malloc( sizeof(char*) * pow(2,wordMaxSize) );
    //Aloca memória para cada palavra do dicionário
    int i;
    for (i=0; i < pow(2, wordMaxSize); i++){
        enc->dictionary[i] = (char*) malloc(sizeof(char*));
    }
}

//Insere uma palavra no dicionário
void insert_into_dictionary(struct encoder* enc, const char* str){
    sprintf(enc->dictionary[enc->nextCode],"%s",str);
    enc->nextCode++;
}

//Inicia o dicionário nos primeiros 128 índices - ASCII
void load_ascii (struct encoder* enc){
    //Inicia o dicionário nos primeiros 128 índices - ASCII
    while (enc->nextCode < 251){
        sprintf(enc->strBuffer,"%c",enc->nextCode);
        insert_into_dictionary(enc, enc->strBuffer);
    }
    //Limpa o buffer
    *(enc->strBuffer) = '\0';
}

//Prepara o codificador ou decodificador para iniciar codificação ou decodificação
void initialize_codec(struct encoder* enc, int wordMaxSize){
    enc->nextCode = 0;
    enc->outBufferSize = 0;
    enc->outBuffer = NULL;
    allocate_dictionary(enc, wordMaxSize);
    load_ascii(enc);
}
