#ifndef LAYERS_H
#define LAYERS_H

#include"layer.h"
#include"functions.h"


//tipo di dato Layers che rappresenta una lista di liste di neuroni 
typedef List* Layers;

//metodo per allocare lo spazio e inizializzare i tutti i layer della rete neurale
Layers createLayers(double** errros,unsigned int nLayers,unsigned int nInputs,unsigned int nOutputs,unsigned int nHiddens,double defValue);
Layers createCustomLayers(double** errors,unsigned int nLayers,unsigned int* layersData,double defValue);

void saveLayers(Node* node,FILE* file);
void loadLayers(List* list,short int dim,FILE* file);
void initLayers(double** errors,Layers layers);//funzione utilizzata per inizializzare i layers dopo il 
//cariamento da file
void freeLayers(Node* node);//dealloca la lista di layer
void printLayers(Layers layers);//stampa la lista di layer



List* getLayersInfo(Node* layer);//ritorna il contenuto informativo di un nodo della lista di layer
void setLayersInfo(Node* layer,List* newInfo);//setta il contenuto informativo di un nodo della lista di layer

void forwardPassLayers(NetFunctions functions,Node* node);

#endif