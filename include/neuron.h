#include"list.h"
#include<stdio.h>
#include"layers.h"
#include<time.h>
#ifndef NEURON_H
#define NEURON_H


//adt Neuron
typedef struct Neuron{

	double activation;//valore di attivazione del neurone/
	double z;//valore del neurone prima della funzione di attivazione
	double error;//valore di errore del neurone o gradiente generale del neurone(per la backpropagation)
	double bias;//bias del neurone
	List* inputWeights;//lista di pesi in input del neurone
	List* outputWeights;//lista di pesi in output del neurone

}Neuron;

//crea un nuvo neurone assegnando bias e attivazione e creando una lista di pesi vuota
Neuron* createNeuron(double activation,double bias);
void deleteNeuron(Neuron* neuron);//libera lo spazio allocato da un neurone

int initInputWeights(Neuron* neuron,int prevLayerSize);/*inizializza la lista di pesi ad un valore scelto,(nota che il numero di pesi associati al neurone è uguale al numero di neuroni nel layer precendte)
inoltre ritona 1 se tutto è andato a buon fine, altrimenti ritorna -1*/
void initOutputWeights(List* layers);

void freeInputWeights(Node* node);//libera lo spazio allocato dalle liste di pesi
void freeOutputWeights(Node* node);

void printNeuron(Node* node);//stampa un neurone

//getters e setters
double getActivation(Neuron* neuron);//ritorna il valore di attivazione
void setActivation(Neuron* neuron,double newActivation);//setta l'attivazione

double getBias(Neuron* neuron);//ritorna il bias
void setBias(Neuron* neuron,double newBias);//setta il bias 

List* getInputWeights(Neuron* neuron);//ritorna la lista di pesi
List* getOutputWeights(Neuron* neuron);

void setOutputWeights(Neuron* neuron,List* newWeights);
void setInputWeights(Neuron* neuron,List* newWeights);//setta i pesi

double getZ(Neuron* neuron);
void setZ(Neuron* neuron,double newZ);

double getError(Neuron* neuron);
void setError(Neuron* neuron,double newError);

void updateError(Neuron* neuron,double newError);

double getInWeightValue(Node* weight);//ritorna il contenuto informativo di un nodo nella lista di pesi(double)
void setInWeightValue(Node* weight,double newValue);//setta il contenuto informativo di un nodo nella lista di pesi

double getOutWeightValue(Node* weight);
void setOutWeightValue(Node* weight,double newValue);

void printOutWeight(Node* node);
void printInWeight(Node* node);

#endif