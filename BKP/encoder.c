#include "encoder.h"

//Busca no dicion�rio pela palavra
int enc_dictionary_contains(struct encoder* enc, char* word){
    int i;
    for (i = enc->nextCode-1; i >= 0; i--){
        if ( !strcmp(enc->dictionary[i], word) ){
            return i;
        }
    }
    return -1; //n�o encontrou
}

//Adiciona uma sa�da � lista de sa�das do codificador
void add_output (struct encoder* enc, int output){
    enc->outBufferSize++;
    enc->outBuffer = (unsigned int*) realloc(enc->outBuffer, enc->outBufferSize*sizeof(unsigned int));
    enc->outBuffer[enc->outBufferSize-1] = output;
}

//Imprime sa�da comprimida em arquivo, em pacotes de 32 bits
void write_to_output(FILE* outFile, struct encoder* enc){
    long int i;
    unsigned int output32 = 0; //32 bit output
    short int freeBits = 32; //32 bits livres inicialmente - unsigned int output32
    unsigned short int wordBits = enc->indexBits; // Apenas TRAB 1: no 2 come�a em 8
	wordBits = 8; //TRAB 2
	int updateIndex = 0; //TRAB 2
    short int shiftCount;
    //Escreve no cabe�alho o n�mero de bits das palavras
    //fwrite(&wordBits, HEADER_BYTES, 1, outFile); apenas no TRAB1
    for (i = 0; i < enc->outBufferSize; i++){
        unsigned int word = enc->outBuffer[i];
		//TRAB 2 - Tamanho Vari�vel:
		//printf("IF.. wordBits = %d\n", wordBits);
		printf("i: %ld - wordBits: %d\n", i, wordBits);
		if ( enc->dLZWupdate[updateIndex] == i ){
			//o c�digo precisa de mais bits pra ser representado
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
            i--; //repetir a palavra, que n�o foi inserida completamente
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
    //Imprime a �ltima sequ�ncia de bits, caso o output32 n�o tenha sido preenchido totalmente
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
        //L� pr�ximo caracter
        fscanf(inFile, "%c", &c);
        //Copia o caracter para o buffer local
        sprintf(cString, "%c", c);
        strcat(localBuffer, cString);
        //Se a cadeia atual existir no dicion�rio
        foundCode = enc_dictionary_contains(enc, localBuffer);
        if (strlen(enc->strBuffer) == 0 || foundCode >=0){
            //Buffer do codificador � acrescido do �ltimo caracter lido
            strcpy(enc->strBuffer, localBuffer);
        }
        else{
            //a maior cadeia dicionarizada j� foi encontrada
            //Insere o c�digo no buffer de sa�da
            add_output(enc, enc_dictionary_contains(enc, enc->strBuffer));
            //Encerra em fim de arquivo, para n�o inserir desnecessariamente ao dicion�rio
            if (feof(inFile)) break;
			if (floor(log2(enc->nextCode)) == 8+updateIndex){ //8 bits + o n�mero de incrementos - TRAB 2
				enc->dLZWupdate[updateIndex] = enc->outBufferSize;
				updateIndex++;
			}
            //Adiciona a nova cadeia ao dicion�rio
            insert_into_dictionary(enc, localBuffer);
            strcpy(enc->strBuffer, cString);
        }
    }
    //Determina o tamanho das palavras no dicion�rio
    //enc->indexBits = ceil(log2(enc->nextCode-1)); //Apenas TRAB 1
	//printf("nextCode: %d", enc->nextCode);
	//printf(" enc->indexBits: %d ", enc->indexBits);
    //Escreve para o arquivo de sa�da
    write_to_output(outFile, enc);
    printf("Encoded!\n");
}
