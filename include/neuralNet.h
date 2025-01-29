#ifndef NEURALNET_H
#define NEURALNET_H

#include"layers.h"
#include"functions.h"

#include<stdlib.h>



typedef enum{//enumeratore per identificare le due modalità di training (mini batch ed online)
	MINI_BATCH,
	ONLINE
}Mode;

typedef void(*ChangeError)(Neuron* neuron,double newValue);/*variabile utilizzata per
cambiare l'operazione da effettuare sull'errore di ciascun neurone ovvero update o set, viene effettuato
un update se si sta utilizzando lo stochastic-gradient descent che prevede l'utilizzo di un mini batch,
altrimenti se si utilizza il metodo classico online,allora si setta un nuovo errore volta per volta*/


//tipo di dato che definisce la rete neurale
typedef struct NeuralNet{

	unsigned int numberOfNeurons;//numero totale di neuroni

	Layers layers; //lista di layer

	NetFunctions functions; //funzioni della rete neurale

	double learnRate; //learning rate per implementare l'algoritmo Gradient Descent

	double error;//risultato di della cost function,nonchè l'errore tra il risultato aspettato e quello ottenuto

	double** errors;/*memorizza tutti gli errori di ogni neurone, in modo da aggiornarli 
					facilmente quando viene implementato il mini-batch stochastic gradient descent
					*/
}NeuralNet;



/*crea una rete neurale dove ogni bias è inizializzato con un 
valore di default(defValue)*/
int createNet(NeuralNet** net);//ritorna 1 se va a buon fine altrimenti , -1
int initDefNet(NeuralNet* net,unsigned int nLayers,unsigned int nInputs,unsigned int nOutputs,unsigned int nHiddens,double defValue);
int initCustomNet(NeuralNet* net,unsigned int nLayers,unsigned int* layersData,double defValue);
int fromFileNet(NeuralNet* net,Layers layers,double learnRate,unsigned int numberOfNeurons,NetFunctions functions);


void deleteNet(NeuralNet* net);//libera lo spazio allocato dalla rete neurale
List* getInputLayer(NeuralNet* net);
List* getOutputLayer(NeuralNet* net);
void printNet(NeuralNet* net);/*stampa il contenuto della rete neurale nel formato :
ogni layer : {
				a1.50(Attivazione del neurone) b1.59(Bias del neurone) {.....(lista di pesi)...}
				a...
				a..
          }
*/
List* forwardPass(NeuralNet* net,List* input);/*utilizzando lo stato corrente della rete neurale,
(quindi mantenendo attivazioni,pesi e bias), viene "dato in pasto" un input alla rete neurale e 
attraverso l'algoritmo di attivazione viene ritornato l'handle 
all'output layer della rete neurale.
*/
void calcHiddenLayerError(Node* scanNeurons,Node* prevLayer,NetFunctions functions,ChangeError errorFunc);
void calcOutLayerError(Node* scanNeurons,Node* scanValues,NetFunctions functions,ChangeError errorFunc);

void backpropagate(NeuralNet* net,List* expValues,Mode mode);
void updateWeights(NeuralNet* net);

void printOutputLayer(NeuralNet* net);
//funzioni helper che non fanno parte dell'interfaccia vera e propria , utilizzate per rendere il codice più modulare
double calcError(NetFunctions functions,List* layer,List* expValue);//calcola la media degli errori, expValue deve essere una lista di double

//getters e setters---------------

unsigned int getNumberOfNeurons(NeuralNet* net);
void setNumberOfNeurons(NeuralNet* net,unsigned int nn);

Layers getLayers(NeuralNet* net);
void setLayers(NeuralNet* net,Layers layers);

NetFunctions getNetFunctions(NeuralNet* net);
void setNetFunctions(NeuralNet* net,NetFunctions functions);

double getLearnRate(NeuralNet* net);
void setLearnRate(NeuralNet* net,double learnRate);

double getNetError(NeuralNet* net);
void setNetError(NeuralNet* net,double error);

double** getErrors(NeuralNet* net);
double* getErrorsAt(NeuralNet* net,unsigned int index);
void setErrors(NeuralNet* net,double** errors);


#endif 