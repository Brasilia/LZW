#include "encoder.h"

//Busca no dicionário pela palavra
int enc_dictionary_contains(struct encoder* enc, char* word){
    int i;
    for (i = enc->nextCode-1; i >= 0; i--){
        if ( !strcmp(enc->dictionary[i], word) ){
            return i;
        }
    }
    return -1; //não encontrou
}

//Adiciona uma saída à lista de saídas do codificador
void add_output (struct encoder* enc, int output){
    enc->outBufferSize++;
    enc->outBuffer = (unsigned int*) realloc(enc->outBuffer, enc->outBufferSize*sizeof(unsigned int));
    enc->outBuffer[enc->outBufferSize-1] = output;
}

//Imprime saída comprimida em arquivo, em pacotes de 32 bits
void write_to_output(FILE* outFile, struct encoder* enc){
    long int i;
    unsigned int output32 = 0; //32 bit output
    short int freeBits = 32; //32 bits livres inicialmente - unsigned int output32
    unsigned short int wordBits = enc->indexBits; // Apenas TRAB 1: no 2 começa em 8
	wordBits = 8; //TRAB 2
	int updateIndex = 0; //TRAB 2
    short int shiftCount;
    //Escreve no cabeçalho o número de bits das palavras
    //fwrite(&wordBits, HEADER_BYTES, 1, outFile); apenas no TRAB1
    for (i = 0; i < enc->outBufferSize; i++){
        unsigned int word = enc->outBuffer[i];
		//TRAB 2 - Tamanho Variável:
		//printf("IF.. wordBits = %d\n", wordBits);
		printf("i: %ld - wordBits: %d\n", i, wordBits);
		if ( enc->dLZWupdate[updateIndex] == i ){
			//o código precisa de mais bits pra ser representado
			wordBits++;
			updateIndex++;
			
		}
        shiftCount = freeBits - wordBits;
        //Cabe uma palavra inteira
        if (shiftCount >= 0){
            output32 |= word << (shiftCount);
        }
        else{
            output32 |= word >> (-shiftCount);
            i--; //repetir a palavra, que não foi inserida completamente
        }
        //Atualiza a quantidade de bits livres no output
        freeBits = shiftCount; // <shiftCount> livres

        //Se preencheu completamente o output
        if(freeBits <= 0){
            fwrite(&output32, 4, 1, outFile);
            freeBits = 32 + shiftCount + wordBits;
            if (shiftCount == 0) freeBits = 32;
            output32 = 0;
        }
    }
    //Imprime a última sequência de bits, caso o output32 não tenha sido preenchido totalmente
    if (shiftCount != 0){
        fwrite(&output32, 4, 1, outFile);
		printf("Ultima sequencia de bits impressa (com buffer incompleto)");
    }
}

//Codifica o arquivo
void encode(FILE* inFile, FILE* outFile, struct encoder* enc){
    printf("\nEncoding...\n");
    //Prepara o codificador
    initialize_codec(enc, G_MAX_indexBits);
	int updateIndex = 0; //TRAB 2
    //Codifica
    while (!feof(inFile)){
        char c;
        char cString[2];
        char localBuffer[MAX_STR_SIZE];
        int foundCode;
        strcpy(localBuffer, enc->strBuffer);
        //Lê próximo caracter
        fscanf(inFile, "%c", &c);
        //Copia o caracter para o buffer local
        sprintf(cString, "%c", c);
        strcat(localBuffer, cString);
        //Se a cadeia atual existir no dicionário
        foundCode = enc_dictionary_contains(enc, localBuffer);
        if (strlen(enc->strBuffer) == 0 || foundCode >=0){
            //Buffer do codificador é acrescido do último caracter lido
            strcpy(enc->strBuffer, localBuffer);
        }
        else{
            //a maior cadeia dicionarizada já foi encontrada
            //Insere o código no buffer de saída
            add_output(enc, enc_dictionary_contains(enc, enc->strBuffer));
            //Encerra em fim de arquivo, para não inserir desnecessariamente ao dicionário
            if (feof(inFile)) break;
			if (floor(log2(enc->nextCode)) == 8+updateIndex){ //8 bits + o número de incrementos - TRAB 2
				enc->dLZWupdate[updateIndex] = enc->outBufferSize;
				updateIndex++;
			}
            //Adiciona a nova cadeia ao dicionário
            insert_into_dictionary(enc, localBuffer);
            strcpy(enc->strBuffer, cString);
        }
    }
    //Determina o tamanho das palavras no dicionário
    //enc->indexBits = ceil(log2(enc->nextCode-1)); //Apenas TRAB 1
	//printf("nextCode: %d", enc->nextCode);
	//printf(" enc->indexBits: %d ", enc->indexBits);
    //Escreve para o arquivo de saída
    write_to_output(outFile, enc);
    printf("Encoded!\n");
}
