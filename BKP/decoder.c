#include "decoder.h"

void decode(FILE* inFile, FILE* outFile, struct encoder* dec){
    printf("\nDecoding...\n");
    unsigned short int wordSize;
    unsigned int codeBuffer32; //32 bit code buffer
    const int bufferSize = 32;
    unsigned int curCode = 0;
    unsigned int lastCode = 0;
    int shifted = 0;
    short int codeReadyFlag = 0; //o primeiro código ainda não está pronto para uso
    //Adquire o tamanho das palavras do cabeçalho
    //fread(&wordSize, HEADER_BYTES, 1, inFile); //Apenas TRAB 1
    //Prepara o decodificador
    //initialize_codec(dec, wordSize); //Apenas TRAB 1
	initialize_codec(dec, G_MAX_indexBits); //TRAB 2
	
    //Enquanto não terminou de ler o arquivo
	wordSize = 8; //TRAB 2
    while (!feof(inFile)){
		
		
        //Se é preciso renovar o buffer de códigos
        if(shifted <= 0){
            if (shifted < 0) shifted += wordSize; //previne deslocamento duplo
            shifted = bufferSize + shifted; //renova contagem
            fread(&codeBuffer32, 4, 1, inFile); //recebe 32 bits no buffer
            if (feof(inFile)) break;
        }
        //Verifica o número de bits a serem deslocados e atualiza o código atual
        shifted -= wordSize;
        if(shifted >= 0){
            curCode |= (codeBuffer32 >> shifted) & (unsigned int)(pow(2, wordSize)-1); //desloca e aplica máscara
            codeReadyFlag = 1; //código pronto para uso
        }
        else{
            curCode = (codeBuffer32 << (-shifted) ) & (unsigned int)(pow(2, wordSize)-1); //desloca e aplica máscara
        }
        //Se o código estiver completo em seus bits e pronto para uso
        if(codeReadyFlag){
            //Imprime a string do código atual no arquivo descompactado
            if (lastCode == 0){ //primeira leitura
                fprintf(outFile, "%s", dec->dictionary[curCode]);
            }
            else{
                //Se o código atual ainda não foi usado de índice no dicionário
                if(curCode < dec->nextCode){
                    char newWord[MAX_STR_SIZE];
                    strcpy(newWord, dec->dictionary[lastCode]);
                    strncat(newWord, dec->dictionary[curCode], 1);
                    insert_into_dictionary(dec, newWord);
					
					printf("No if: \"%s\" inserido no dicionario com codigo %d\n", newWord, dec->nextCode -1);
					
                    fprintf(outFile, "%s", dec->dictionary[curCode]);
                }
                else{ //Se o código atual já está dicionarizado
                    char newWord[MAX_STR_SIZE];
                    strcpy(newWord, dec->dictionary[lastCode]);
                    strncat(newWord, dec->dictionary[lastCode], 1);
                    insert_into_dictionary(dec, newWord);
					
					printf("No else: \"%s\" inserido no dicionario com codigo %d\n", newWord, dec->nextCode-1);

                    fprintf(outFile, "%s", newWord);
                }
				printf("wordSize = %d\n", wordSize);
				wordSize = ceil(log2(dec->nextCode+1)); //TRAB 2
            }
            //Limpa código e zera flag para deixar pronto para leitura do próximo código
            lastCode = curCode; //atualiza último código
            curCode = 0; //limpa código atual
            codeReadyFlag = 0; //código incompleto
        }
    }
    printf("Decoded!\n");
}
