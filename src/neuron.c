#include"neuron.h"

Neuron* createNeuron(double activation,double bias){
	Neuron* neuron = (Neuron*)malloc(sizeof(Neuron));
	if(neuron == NULL){
		return neuron;
	}
	neuron->bias = bias;
	neuron->activation = activation;
	neuron->z = 0;
	neuron->error = 0;
	neuron->inputWeights = createList(sizeof(double),freeInputWeights);
	neuron->outputWeights = createList(sizeof(Node*),freeOutputWeights);
	return neuron;
}
void deleteNeuron(Neuron* neuron){
	//elimino prima ogni peso relativo al neurone
	/*parto dalla coda della lista , ogni volta considero il Nodo precendete , ed elimino il corrente */
	freeList(neuron->outputWeights);
	freeList(neuron->inputWeights);
	free(neuron);//elimino il neurone
}

void freeInputWeights(Node* node){
	double* info = (double*)getInfo(node);
	free(info);
	free(node);
}
void freeOutputWeights(Node* node){
	
	free(getInfo(node));
	free(node);
}

int initInputWeights(Neuron* neuron,int prevLayerSize){
	if(neuron == NULL)
		return -1;
	setSaveFunc(neuron->inputWeights,saveDouble);
	double variance = sqrt(1.0/(double)prevLayerSize);
	for(int k = 0; k < prevLayerSize;k++){//il numero di pesi deve essere uguale al numero di neuroni del layer precedente
		double rnd = (double)(rand())/RAND_MAX;
		double weight = rnd* variance;
		add(neuron->inputWeights,&weight);	

	}
		
	return 1;
	
}
void initOutputWeights(List* layers){
	//scorro i layers al contrario
	Node* layersHead = getHead(layers);
	Node* nextLayer = NULL;
	while(layersHead != NULL){
		if(getNext(layersHead) != NULL){
			nextLayer = getNext(layersHead);
			Node* nextLayerTail = getTail(getLayersInfo(nextLayer));
			Node* scanNextLayer = nextLayerTail;
			Neuron* nextNeuron = NULL;
			while(scanNextLayer != NULL){
				nextNeuron = getLayerInfo(scanNextLayer);
				if(scanNextLayer == nextLayerTail){
					//scorro per ogni neurone del layer precedente
					Node* currentTail = getTail(getLayersInfo(layersHead));
					Neuron* currNeuron = NULL;
					while(currentTail != NULL){
						currNeuron = getLayerInfo(currentTail);
						Node* addNode = getTail(getInputWeights(currNeuron));
						add(nextNeuron->outputWeights,&addNode);
						currentTail = getPrev(currentTail);
					}
				}else{
					Neuron* prevNeuron = getLayerInfo(getNext(scanNextLayer));
					List* prevOutputWeights = getOutputWeights(prevNeuron);
					Node* scanW = getTail(prevOutputWeights);
					while(scanW != NULL){
						Node* n = *(Node**)getInfo(scanW);
						n = getPrev(n);
						add(nextNeuron->outputWeights,&n);
						scanW = getPrev(scanW);
					}
				}
				scanNextLayer = getPrev(scanNextLayer);
			}
		}
		layersHead = getNext(layersHead);
	}
}

void printInWeight(Node* node){
	double* weight = (double*)getInfo(node);
	printf("%lf ",*weight);
}

void printOutWeight(Node* node){
	printf("%lf ",getOutWeightValue(node));
}

void printNeuron(Node* node){
	Neuron* neuron = *(Neuron**)getInfo(node);
	printf("	a%lf z%lf b%lf e%lf Iw{",neuron->activation,neuron->z,neuron->bias,neuron->error);
	loopT(neuron->inputWeights,printInWeight);
	printf("}");
	printf("  Ow{");
	loopT(neuron->outputWeights,printOutWeight);
	printf("}\n");

}

double getActivation(Neuron* neuron){
	if(neuron==NULL)printf("il neurone e' NULL!");
	return neuron->activation;
}
double getZ(Neuron* neuron){
	return neuron->z;
}
void setZ(Neuron* neuron,double newZ){
	neuron->z = newZ;
}
double getError(Neuron* neuron){
	return neuron->error;
}
void setError(Neuron* neuron,double newError){
	neuron->error = newError;
}
void setActivation(Neuron* neuron,double newActivation){
	neuron->activation = newActivation;
}
double getBias(Neuron* neuron){
	return neuron->bias;
}
void setBias(Neuron* neuron,double newBias){
	neuron->bias = newBias;
}
List* getInputWeights(Neuron* neuron){
	return neuron->inputWeights;
}
void setInputWeights(Neuron* neuron,List* newWeights){
	neuron->inputWeights = newWeights;
}
double getInWeightValue(Node* weight){
	return *(double*)(getInfo(weight));
}
double getOutWeightValue(Node* weight){
	Node* nodeValue = *(Node**)getInfo(weight);
	return *(double*)getInfo(nodeValue);
}
void setInWeightValue(Node* weight,double newValue){
	*(double*)(getInfo(weight)) = newValue;
}
List* getOutputWeights(Neuron* neuron){
	return neuron->outputWeights;
}
void setOutputWeights(Neuron* neuron,List* newWeights){
	neuron->outputWeights = newWeights;
}
void setOutWeightValue(Node* weight,double newValue){
	*(double*)(getInfo((*(Node**)getInfo(weight)))) = newValue;
}

void updateError(Neuron* neuron,double newError){
	neuron->error += newError;
}


