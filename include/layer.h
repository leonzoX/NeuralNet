#ifndef LAYER_H
#define LAYER_H

#include"list.h"
#include"neuron.h"

typedef List* Layer;

void saveLayer(Node* node,FILE* file);
void loadLayer(List* list,short int dim,FILE* file);
void freeLayer(Node* node);//dealloca un singolo layer(che contiene neuroni)

void printLayer(Node* node);//stampa un singolo layer

Neuron* getLayerInfo(Node* layerNode);//ritorna il contenuto informativo di un nodo di un singolo layer
void setLayerInfo(Node* layerNode,Neuron* newInfo);//setta il contenuto informativo di un nodo di un singolo layer

#endif