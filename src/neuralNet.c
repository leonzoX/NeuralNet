#include"neuralNet.h"

int createNet(NeuralNet** net){
	if((*net = (NeuralNet*)malloc(sizeof(NeuralNet))) != NULL){
		setNumberOfNeurons(*net,0);
		setLearnRate(*net,0);
		setLayers(*net,NULL);
		setNetError(*net,0);
		setErrors(*net,NULL);
		return 1;
	}
	return -1;
}

int initDefNet(NeuralNet* net,unsigned int nLayers,unsigned int nInputs,unsigned int nOutputs,unsigned int nHiddens,double defValue){

	if(net != NULL){
		setErrors(net,malloc(sizeof(double*) * (nInputs+nOutputs+((nLayers-2)*(nHiddens)))));
		setNumberOfNeurons(net,nInputs+nOutputs+((nLayers-2)*(nHiddens)));

		//alloco lo spazio per la rete neurale vera e propria , alloco una lista di layer , una lista di neuroni ed una lista di pesi
		Layers layers = createLayers(getErrors(net),nLayers,nInputs,nOutputs,nHiddens,defValue);
		if(layers == NULL) return -1;
		setLayers(net,layers);
		setSaveFunc(getLayers(net),saveLayers);

		return 1;
	}
	return -1;	

}

int initCustomNet(NeuralNet* net,unsigned int nLayers,unsigned int* layersData,double defValue){
	
	if(net != NULL && layersData != NULL){
		unsigned int numberOfNeurons = 0;


		for(int i = 0; i < nLayers;i++)
			numberOfNeurons += layersData[i];
		setNumberOfNeurons(net,numberOfNeurons);


		
		setErrors(net,(double**)malloc(sizeof(double*) * (getNumberOfNeurons(net))));
		
		Layers layers = createCustomLayers(getErrors(net),nLayers,layersData,defValue);
		if(layers == NULL) return -1;
		setLayers(net,layers);
		setSaveFunc(getLayers(net),saveLayers);

		return 1;
	}
	return -1;
}

int fromFileNet(NeuralNet* net,Layers layers,double learnRate,unsigned int numberOfNeurons,NetFunctions functions){
	if(net != NULL){
		setLearnRate(net,learnRate);
		setNetFunctions(net,functions);
		setNumberOfNeurons(net,numberOfNeurons);

		setErrors(net,(double**)malloc(sizeof(double*) * (getNumberOfNeurons(net))));

		initLayers(getErrors(net),layers);
		setLayers(net,layers);
		setSaveFunc(getLayers(net),saveLayers);
		
		return 1;
	}
	return -1;

}
void deleteNet(NeuralNet* net){
	free(getErrors(net));
	if(getLayers(net) != NULL)
		freeList(getLayers(net));
	free(net);
}

void printNet(NeuralNet* net){
	printf("\nlearning rate : %lf\n",getLearnRate(net));
	printLayers(getLayers(net));
}
List* forwardPass(NeuralNet* net,List* input){
	Layers layers = getLayers(net);//handle della lista di layer
	
	List* inputLayer = getLayersInfo(getTail(layers));//handle del layer di input

	if(getSize(inputLayer) == getSize(input)){//controllo se la size dell'input della rete neurale e' uguale a quella dell'input passato da parametro
		//controllo se lo spazio occupato dagli elementi di input e' lo stesso di un double (attivazione neurone)
		if(getTypeSize(input) == sizeof(double)){

			//prima copio l'input passato da parametro nell'input della rete neurale 

			Node* oldInputHandle = getTail(inputLayer);
			Node* newInputHandle = getTail(input);
			while(oldInputHandle != NULL){
				Neuron* oldNeuron = getLayerInfo(oldInputHandle);
				setActivation(oldNeuron,getDouble(newInputHandle));
				oldInputHandle = getPrev(oldInputHandle);
				newInputHandle = getPrev(newInputHandle);
			}
			//printList(inputLayer,printNeuron);
			//adesso scorro per ogni layer ed effettuo il forward pass

			Node* layersTail = getTail(layers);//handle alla coda della lista di layer
			while(layersTail != NULL){
				forwardPassLayers(getNetFunctions(net),layersTail);
				layersTail = getPrev(layersTail);
			}

		}else{
			return NULL;
		}
	}else{
	     	return NULL;
	}
	List* outLayer = getLayersInfo(getHead(layers));
	return outLayer;
}

double calcError(NetFunctions functions,List* layer,List* expValue){
	//prima controllo se può essere una lista di double (expValue) e le due liste hanno la stessa size
	if(getTypeSize(expValue) == sizeof(double) && getSize(layer) == getSize(expValue)){
		if(getTypeSize(layer) == sizeof(Neuron*)){//controllo se viene effettivamente passato un layer della rete
			double totalError = 0;
			Node* layerScan = getTail(layer);
			Node* expScan = getTail(expValue);
			while(layerScan != NULL){
				Neuron* layerNeuron = getLayerInfo(layerScan);
				totalError += functions.costFunc(getActivation(layerNeuron),getDouble(expScan));

				expScan = getPrev(expScan);
				layerScan = getPrev(layerScan);
			}
			return (totalError/getSize(layer));
		}else{
			return -1;
		}

	}
	return -1;
}

List* getInputLayer(NeuralNet* net){
	Layers layers = getLayers(net);
	return getLayersInfo(getTail(layers));
}
List* getOutputLayer(NeuralNet* net){
	Layers layers = getLayers(net);
	
	return getLayersInfo(getHead(layers));
}

void calcOutLayerError(Node* scanNeurons,Node* scanValues,NetFunctions functions,ChangeError errorFunc){
	functions.squishFuncPrime = sigmoidPrime;
	while(scanNeurons != NULL){//scannerizzo tra i neuroni
		Neuron* currNeuron = getLayerInfo(scanNeurons);
		errorFunc(currNeuron,functions.costFuncPrime(getActivation(currNeuron),getDouble(scanValues) * 
		functions.squishFuncPrime(getZ(currNeuron))));
		//printf("\n errore output : %lf\n",getError(currNeuron));
		scanNeurons = getPrev(scanNeurons); 
		scanValues = getPrev(scanValues);
	}
}

void calcHiddenLayerError(Node* scanNeurons,Node* prevLayer,NetFunctions functions,ChangeError errorFunc){
	double error = 0;
	while(scanNeurons != NULL){
		Neuron* currNeuron = getLayerInfo(scanNeurons);//neurone corrente
		List* outWeights = getOutputWeights(currNeuron);//pesi di output del neurone corrente
		Node* prevNeurons = getTail(getLayersInfo(prevLayer));//nodo coda del layer di neuroni precedente
		Node* nodeOutWeights = getTail(outWeights);//coda alla lista di pesi di output del neurone corrente
		while(nodeOutWeights != NULL){
			error += getOutWeightValue(nodeOutWeights)* getError(getLayerInfo(prevNeurons));
			nodeOutWeights = getPrev(nodeOutWeights);
			prevNeurons = getPrev(prevNeurons);
		}
		errorFunc(currNeuron,error * functions.squishFuncPrime(getZ(currNeuron)));
		scanNeurons = getPrev(scanNeurons); 
	}
}

void updateWeights(NeuralNet* net){
	Node* scanLayers = getTail(getLayers(net));
	//nota: scanLayers parte dalla coda dei layers ovvero scorre dagli input agli output
	double error = 0;
	double weightValue = 0;
	double currZ = 0;
	while(scanLayers != NULL){//scorro tra i vari layer
		Node* scanCurrNeurons = getTail(getLayersInfo(scanLayers));//coda lista neuroni layer corrente
		Node* scanPrevNeurons = NULL;
		if(getPrev(scanLayers)!=NULL)
			scanPrevNeurons = getTail(getLayersInfo(getPrev(scanLayers)));//coda lista neuroni prossimo layer
		while(scanCurrNeurons != NULL && scanPrevNeurons != NULL){
			Node* outWeights = getTail(getOutputWeights(getLayerInfo(scanCurrNeurons)));
			currZ = getZ(getLayerInfo(scanCurrNeurons));
			while(outWeights != NULL){//1 : scorro per ogni peso
				error = getError(getLayerInfo(scanPrevNeurons));//3 : ottengo l'errore del neurone
				weightValue = getOutWeightValue(outWeights);
				//4 : aggiorno il peso corrente
				setOutWeightValue(outWeights,weightValue - (getLearnRate(net) *( error * currZ)));
				outWeights = getPrev(outWeights);
				scanPrevNeurons = getPrev(scanPrevNeurons);//2 : contemporaneamente scorro per ogni neurone del layer precedente 
			}
			Neuron* currN = getLayerInfo(scanCurrNeurons);
			setBias(currN,getBias(currN)- (getLearnRate(net) * getError(currN)));
			scanCurrNeurons = getPrev(scanCurrNeurons);
		}
		scanLayers = getPrev(scanLayers);
	}
}
void backpropagate(NeuralNet* net,List* expValues,Mode mode){
	NetFunctions functions = getNetFunctions(net);
	ChangeError errorFunc = mode == MINI_BATCH ? updateError : setError;

	Node* expValTail = getTail(expValues);

	Node* scanLayers = getHead(getLayers(net));

	net->error = calcError(functions,getLayersInfo(scanLayers),expValues);//calcolo l'errore

	while(scanLayers != NULL){
		Node* currLayerTail = getTail(getLayersInfo(scanLayers));//per scannerizzare i neuroni
		
		if(scanLayers == getHead(getLayers(net))){//se mi trovo nel layer di output
			
			calcOutLayerError(currLayerTail,expValTail,functions,errorFunc);

		}else{//se mi trovo negli hidden layers
			calcHiddenLayerError(currLayerTail,getPrev(scanLayers),functions,errorFunc);
		}
		scanLayers = getNext(scanLayers);
	}
}

void printOutputLayer(NeuralNet* net){
	List* outLayer = getOutputLayer(net);
	Node* tail = getTail(outLayer);
	int conta = 0;
	printf("\n");
	while(tail != NULL){
		Neuron* neuron = *(Neuron**)getInfo(tail);
		printf("%d : %lf \n",conta,getActivation(neuron));
		tail = getPrev(tail);
		conta++;
	}
}

unsigned int getNumberOfNeurons(NeuralNet* net){
	return net->numberOfNeurons;
}
void setNumberOfNeurons(NeuralNet* net,unsigned int nn){
	net->numberOfNeurons = nn;
}

Layers getLayers(NeuralNet* net){
	return net->layers;
}
void setLayers(NeuralNet* net,Layers layers){
	net->layers = layers;
}

NetFunctions getNetFunctions(NeuralNet* net){
	return net->functions;
}
void setNetFunctions(NeuralNet* net,NetFunctions functions){
	net->functions = functions;
}

double getLearnRate(NeuralNet* net){
	return net->learnRate;
}
void setLearnRate(NeuralNet* net,double learnRate){
	net->learnRate = learnRate;
}

double getNetError(NeuralNet* net){
	return net->error;
}
void setNetError(NeuralNet* net,double error){
	net->error = error;
}

double** getErrors(NeuralNet* net){
	return net->errors;
}
double* getErrorsAt(NeuralNet* net,unsigned int index){
	return (net->errors)[index];
}
void setErrors(NeuralNet* net,double** errors){
	net->errors = errors;
}
