#ifndef LAYER_H
#define LAYER_H

#include"neuron.h"

//tipo di dato Layers che rappresenta una lista di liste di neuroni 
typedef List* Layers;

//metodo per allocare lo spazio e inizializzare i tutti i layer della rete neurale
Layers createLayers(unsigned int nLayers,unsigned int nInputs,unsigned int nOutputs,unsigned int nHiddens,double defValue);

void freeLayers(Node* node);//dealloca la lista di layer
void freeNeurons(Node* node);//dealloca un singolo layer(che contiene neuroni)
void printLayer(Node* node);//stampa un singolo layer
void printLayers(Layers layers);//stampa la lista di layer
//getters e setters per la lista di layer(lista di lista di neuroni)
List* getLayersInfo(Node* layer);//ritorna il contenuto informativo di un nodo della lista di layer
void setLayersInfo(Node* layer,List* newInfo);//setta il contenuto informativo di un nodo della lista di layer
//getters e setters per la lista di neuroni
Neuron* getLayerInfo(Node* layerNode);//ritorna il contenuto informativo di un nodo di un singolo layer
void setLayerInfo(Node* layerNode,Neuron* newInfo);//setta il contenuto informativo di un nodo di un singolo layer
#endif