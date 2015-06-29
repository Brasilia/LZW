/*
    Codificador e Decodificador LZW para Compressão de Texto
    Trabalho 1 - Multimídia - Professor Dr. Marcelo Manzato
    Autor: Rafael Miranda Lopes - nºUSP 6520554
*/

#include "encoder.h"
#include "decoder.h"

int main (int argc, char** argv){
    FILE* inputFile = NULL;
    FILE* outputFile = NULL;
    short int badArguments = 0;
    short int codecFlag = 0; // 0 não foi escolhido, 1 codifica e -1 decodifica
	short int dynamic = 0; // Flag do codec dinamico - Trabalho 2
    struct encoder codec;

    //Verifica número de argumentos
    if(argc >= 6){
        //Verifica argumentos e determina função do coder-decoder
        if(!strcmp(argv[2], "-i") && !strcmp(argv[4], "-o")){
            if(!strcmp(argv[1], "encode")){
                //Encode
                codecFlag = 1;
                inputFile = fopen(argv[3] ,"r");
                outputFile = fopen(argv[5], "wb");
            }
            else if(!strcmp(argv[1], "decode")){
                //Decode
                codecFlag = -1;
                inputFile = fopen(argv[3] ,"rb");
                outputFile = fopen(argv[5], "w");
            }
            else{
                badArguments = 1;
            }
        }
        else{
            badArguments = 1;
        }
		//Ajuste do flag de codec dinâmico - Trabalho 2
		if(argc == 7){
			if(!strcmp(argv[6], "-d") dynamic = 1; // flag dynamic é verdadeira
		}
    }
    else{
        badArguments = 1;
    }
    //Erro: Argumentos incorretos:
    if (badArguments || !codecFlag){
        printf("Lista de argumentos invalida. Os argumentos devem seguir o formato:");
        printf("\n<modo: encode ou decode> -i <arq de entrada> -o <arq de saida> -d(este opcional, para LZW dinamico)\n");
        return -1;
    }
    //Erro: nome do input inválido
    if(inputFile == NULL){
        printf("Nome de arquivo de entrada invalido.\n");
        return -2;
    }

    //Se os parâmetros foram escolhidos corretamente:
    if(codecFlag == 1){
        encode(inputFile, outputFile, &codec, dynamic);
    }
    else{
        decode(inputFile, outputFile, &codec, dynamic);
    }
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}
