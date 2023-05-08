#ifndef NEURON_H
#define NEURON_H

#include"list.h"
#include<stdio.h>

//adt Neuron
typedef struct Neuron{
	double activation;//valore di attivazione del neurone
	double bias;//bias del neurone
	List* weights;//lista di pesi associata al neurone
}Neuron;

//crea un nuvo neurone assegnando bias e attivazione e creando una lista di pesi vuota
Neuron* createNeuron(double activation,double bias);
void deleteNeuron(Neuron* neuron);//libera lo spazio allocato da un neurone
void freeWeights(Node* node);//libera lo spazio allocato dalla lista di pesi
void printNeuron(Node* node);//stampa un neurone
int initWeights(Neuron* neuron,int prevLayerSize,double weightDef);/*inizializza la lista di pesi ad un valore scelto,(nota che il numero di pesi associati al neurone è uguale al numero di neuroni nel layer precendte)
inoltre ritona 1 se tutto è andato a buon fine, altrimenti ritorna -1*/

//getters e setters
double getActivation(Neuron* neuron);//ritorna il valore di attivazione
void setActivation(Neuron* neuron,double newActivation);//setta l'attivazione
double getBias(Neuron* neuron);//ritorna il bias
void setBias(Neuron* neuron,double newBias);//setta il bias 
List* getWeights(Neuron* neuron);//ritorna la lista di pesi
void setWeights(Neuron* neuron,List* newWeights);//setta i pesi
//---------------------------------------------
double getWeightValue(Node* weight);//ritorna il contenuto informativo di un nodo nella lista di pesi(double)
void setWeightValue(Node* weight,double newValue);//setta il contenuto informativo di un nodo nella lista di pesi
#endif