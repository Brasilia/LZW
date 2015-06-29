#include "codecbase.h"

//Aloca mem�ria para o dicion�rio
void allocate_dictionary(struct encoder* enc, int wordMaxSize){
    enc->dictionary = (char**) malloc( sizeof(char*) * pow(2,wordMaxSize) );
    //Aloca mem�ria para cada palavra do dicion�rio
    int i;
    for (i=0; i < pow(2, wordMaxSize); i++){
        enc->dictionary[i] = (char*) malloc(sizeof(char*));
    }
}

//Insere uma palavra no dicion�rio
void insert_into_dictionary(struct encoder* enc, const char* str){
    sprintf(enc->dictionary[enc->nextCode],"%s",str);
    enc->nextCode++;
}

//Inicia o dicion�rio nos primeiros 128 �ndices - ASCII
void load_ascii (struct encoder* enc){
    //Inicia o dicion�rio nos primeiros 128 �ndices - ASCII
    while (enc->nextCode < 251){
        sprintf(enc->strBuffer,"%c",enc->nextCode);
        insert_into_dictionary(enc, enc->strBuffer);
    }
    //Limpa o buffer
    *(enc->strBuffer) = '\0';
}

//Prepara o codificador ou decodificador para iniciar codifica��o ou decodifica��o
void initialize_codec(struct encoder* enc, int wordMaxSize){
    enc->nextCode = 0;
    enc->outBufferSize = 0;
    enc->outBuffer = NULL;
    allocate_dictionary(enc, wordMaxSize);
    load_ascii(enc);
}
