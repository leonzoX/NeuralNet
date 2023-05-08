#ifndef NEURALNET_H
#define NEURALNET_H

#include"layer.h"
#include<stdlib.h>

/*tipo di dato che contiene i puntatori a funzioni cost e squish, con le loro rispettive derivate prime*/
typedef struct NetFunctions{
	double(*costFunc)(double currValue,double expValue);//cost function
	double(*costFuncPrime)(double currValue,double expValue);//derivata prima della cost function
	double(*squishFunc)(double value);//squishification function
	double(*squishFuncPrime)(double value);//derivata prima della squishification function
}NetFunctions;

//tipo di dato che definisce la rete neurale
typedef struct NeuralNet{
	Layers layers; //lista di layer
	NetFunctions functions; //funzioni della rete neurale
	double learnRate; //learning rate per implementare l'algoritmo Gradient Descent
	double error;//risultato di della cost function,nonchè l'errore tra il risultato aspettato e quello ottenuto
}NeuralNet;


/*crea una rete neurale dove ogni valore (attivazione neuroni,pesi e bias) è inizializzato con un 
valore di default(defValue)*/
NeuralNet* createDefNet(unsigned int nLayers,unsigned int nInputs,unsigned int nOutputs,unsigned int nHiddens,double defValue,double defLearnRate,NetFunctions functions);
void deleteNet(NeuralNet* net);//libera lo spazio allocato dalla rete neurale
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

//funzioni helper che non fanno parte dell'interfaccia vera e propria , utilizzate per rendere il codice più modulare

void forwardPassLayer(NetFunctions functions,Node* node);
double calcError(NetFunctions functions,List* layer,List* expValue);//calcola la media degli errori, expValue deve essere una lista di double

#endif 