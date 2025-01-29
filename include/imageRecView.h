#include"imageRecognizer.h"
#include<ctype.h>

#ifndef IMAGE_REC_VIEW_H
#define IMAGE_REC_VIEW_H

#define LIMIT 200//numero massimo di caratteri in input

#define FCOMMANDS 7//numero di comandi iniziali
#define SCOMMANDS 4//numero di comandi secondari
#define TCOMMANDS 5//numero di comandi terziari

#define NCOST 2//numero di cost functions disponibili
#define NACT 3//numero di activation funcions disponibili
#define NFUNCKEYS 2

//enumeratore per classificare ciascun comando disponibile
typedef enum {
	LOAD_N_CONFIG = 111,
	LOAD_TE_DATA = 231,
	LOAD_TR_DATA = 221,
	LOAD_N_DATA = 211,
	SAVE_N_DATA = 212,
	CONFIG_N_FUNC = 313,
	SHOW_N_FUNC = 314,
	CONFIG_N_LAYERS = 413,
	CONFIG_N_TRAIN = 513,
	LOAD_C_CONFIG = 141,
	TRAIN = 5,
	TEST = 6,
	ESC = 7
}Command;



void startView();//funzione principale da richiamare nel main

List* tokenize(char* input);//prende in input un array che rappresenta il comando e ritorna una lista di token 
Command parse(List* tokens);//prende in input una lista di token e la analizza cercando di classificarla come uno dei comandi disponibili nell'enum
void executeCommand(Command command,ImageRecognizer* imgRec,List* tokens,boolean* esc);//esegue un comando

//------funzioni che vengono chiamate all'esecuzione di un determinato comando------------------

boolean loadTestData(List* tokens,ImageRecognizer* imgRec);
boolean loadTrainData(List* tokens,ImageRecognizer* imgRec);

boolean configFunctions(List* tokens,ImageRecognizer* imgRec);
void showFunctions();//ok

boolean configNetLayers(List* tokens,ImageRecognizer* imgRec);
boolean configNetTraining(List* tokens,ImageRecognizer* imgRec);

void loadCurrentConfig(ImageRecognizer* imgRec);

void train(ImageRecognizer* imgRec);
void test(ImageRecognizer* imgRec);

void escProgram(ImageRecognizer* imgRec);

void loadNetworkConfig(List* tokens,ImageRecognizer* imgRec,boolean* esc);

boolean saveNetwork(List* tokens,ImageRecognizer* imgRec);

boolean loadNetwork(List* tokens,ImageRecognizer* imgRec);

//utility
int isNumber(char* string);//ritorna 1 se è un numero , -1 altrimenti
List* obtainAdvancedConfig(char* configString);/*prende in input una stringa per configurare in advanced : {1,2,3}, controlla se 
	è formattata correttamente, ritornando NULL se non lo è , o una lista di interi degli argomenti della stringa

	inoltre controlla se gli argomenti sono dei numeri o meno*/
#endif