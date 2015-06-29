#include "decoder.h"

void decode(FILE* inFile, FILE* outFile, struct encoder* dec){
    printf("\nDecoding...\n");
    unsigned short int wordSize;
    unsigned int codeBuffer32; //32 bit code buffer
    const int bufferSize = 32;
    unsigned int curCode = 0;
    unsigned int lastCode = 0;
    int shifted = 0;
    short int codeReadyFlag = 0; //o primeiro c�digo ainda n�o est� pronto para uso
    //Adquire o tamanho das palavras do cabe�alho
    //fread(&wordSize, HEADER_BYTES, 1, inFile); //Apenas TRAB 1
    //Prepara o decodificador
    //initialize_codec(dec, wordSize); //Apenas TRAB 1
	initialize_codec(dec, G_MAX_indexBits); //TRAB 2
	
    //Enquanto n�o terminou de ler o arquivo
	wordSize = 8; //TRAB 2
    while (!feof(inFile)){
		
		
        //Se � preciso renovar o buffer de c�digos
        if(shifted <= 0){
            if (shifted < 0) shifted += wordSize; //previne deslocamento duplo
            shifted = bufferSize + shifted; //renova contagem
            fread(&codeBuffer32, 4, 1, inFile); //recebe 32 bits no buffer
            if (feof(inFile)) break;
        }
        //Verifica o n�mero de bits a serem deslocados e atualiza o c�digo atual
        shifted -= wordSize;
        if(shifted >= 0){
            curCode |= (codeBuffer32 >> shifted) & (unsigned int)(pow(2, wordSize)-1); //desloca e aplica m�scara
            codeReadyFlag = 1; //c�digo pronto para uso
        }
        else{
            curCode = (codeBuffer32 << (-shifted) ) & (unsigned int)(pow(2, wordSize)-1); //desloca e aplica m�scara
        }
        //Se o c�digo estiver completo em seus bits e pronto para uso
        if(codeReadyFlag){
            //Imprime a string do c�digo atual no arquivo descompactado
            if (lastCode == 0){ //primeira leitura
                fprintf(outFile, "%s", dec->dictionary[curCode]);
            }
            else{
                //Se o c�digo atual ainda n�o foi usado de �ndice no dicion�rio
                if(curCode < dec->nextCode){
                    char newWord[MAX_STR_SIZE];
                    strcpy(newWord, dec->dictionary[lastCode]);
                    strncat(newWord, dec->dictionary[curCode], 1);
                    insert_into_dictionary(dec, newWord);
					
					printf("No if: \"%s\" inserido no dicionario com codigo %d\n", newWord, dec->nextCode -1);
					
                    fprintf(outFile, "%s", dec->dictionary[curCode]);
                }
                else{ //Se o c�digo atual j� est� dicionarizado
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
            //Limpa c�digo e zera flag para deixar pronto para leitura do pr�ximo c�digo
            lastCode = curCode; //atualiza �ltimo c�digo
            curCode = 0; //limpa c�digo atual
            codeReadyFlag = 0; //c�digo incompleto
        }
    }
    printf("Decoded!\n");
}
