#include"layers.h"


Layers createLayers(double** errors,unsigned int nLayers,unsigned int nInputs,unsigned int nOutputs,unsigned int nHiddens,double defValue){
	srand(time(0));//inizializzo la srand per generare i pesi casuali
	Layers layers = createList(sizeof(List*),freeLayers);
	//per riempio la lista di layer di liste di neuroni 
	unsigned int countErrors = 0;
	for(int i = 0; i < nLayers;i++){
		List* currLayer = createList(sizeof(Neuron*),freeLayer);
		setSaveFunc(currLayer,saveLayer);

		//unsigned int condition = i == nLayers-1 ;
		unsigned int check = i == 0 ? nInputs  : (i == nLayers-1 ? nOutputs : nHiddens);
		for(int j = 0; j < check; j++){
			Neuron* neuron = createNeuron(defValue,defValue);//creo il nuovo neurone da inserire
			if(neuron == NULL) return NULL;
			errors[countErrors] = &(neuron->error);
			//se non mi trovo nel primo layer allora ogni neurona avrà una lista di pesi, altrimenti la lista sarà nulla
			if(i != 0){
				//riempio ogni neurone di pesi
				Node* head = getHead(layers);
				
				List* prevLayer = *(List **)getInfo(head);//dato che add inserisce in testa la testa corrisponde all'ultimo elemento inserito , perciò per ottenere il precendete devo fare ->next
				
				if(initInputWeights(neuron,getSize(prevLayer)) == -1)
					return NULL;
			}
			add(currLayer,&neuron);
			countErrors++;
		}
		add(layers,&currLayer);
	}
	initOutputWeights(layers);
	return layers;
}
Layers createCustomLayers(double** errors,unsigned int nLayers,unsigned int* layersData,double defValue){
	srand(time(0));//inizializzo la srand per generare i pesi casuali
	Layers layers = createList(sizeof(List*),freeLayers);
	//per riempio la lista di layer di liste di neuroni 
	unsigned int countErrors = 0;
	for(int i = 0; i < nLayers;i++){
		List* currLayer = createList(sizeof(Neuron*),freeLayer);
		setSaveFunc(currLayer,saveLayer);

		for(int j = 0; j < layersData[i]; j++){
			Neuron* neuron = createNeuron(defValue,defValue);//creo il nuovo neurone da inserire
			if(neuron == NULL) return NULL;
			errors[countErrors] = &(neuron->error);
			//se non mi trovo nel primo layer allora ogni neurona avrà una lista di pesi, altrimenti la lista sarà nulla
			if(i != 0){
				//riempio ogni neurone di pesi
				Node* head = (getHead(layers));
				
				List* prevLayer = *(List **)getInfo(head);//dato che add inserisce in testa la testa corrisponde all'ultimo elemento inserito , perciò per ottenere il precendete devo fare ->next
				
				if(initInputWeights(neuron,getSize(prevLayer)) == -1)
					return NULL;
			}
			add(currLayer,&neuron);
			countErrors++;
		}
		add(layers,&currLayer);
	}
	initOutputWeights(layers);
	return layers;
}
void initLayers(double** errors,Layers layers){
	Node* scanLayers = getTail(layers);
	unsigned int countErrors = 0;
	while(scanLayers != NULL){
		Node* scanLayer = getTail(getLayersInfo(scanLayers));
		
		while(scanLayer != NULL){
			Neuron* neuron = getLayerInfo(scanLayer);
			errors[countErrors] = &(neuron->error);

			scanLayer = getPrev(scanLayer);
			countErrors++;
		}
		scanLayers = getPrev(scanLayers);
	}
	initOutputWeights(layers);
}
void freeLayers(Node* node){
	List* info = *(List**)getInfo(node);//informazione puntata da dal valore puntato da void *
	freeList(info);
	free(getInfo(node));//void *
	free(node);
}

void printLayers(Layers layers){
	loopT(layers,printLayer);
}

List* getLayersInfo(Node* layer){
	return *(List**)(getInfo(layer));
}
void setLayersInfo(Node* layer,List* newInfo){
	*(List**)(getInfo(layer)) = newInfo;
}



void forwardPassLayers(NetFunctions functions,Node* node){
	//se mi trovo nel primo  layer non faccio niente (ricorda che scorro dalla coda verso la testa)
	if(getPrev(node) == NULL) functions.squishFunc = sigmoid;
	if(node->next != NULL){
		double activationSum = 0;
		//handle rispettivamente al layer del nodo ed al layer del nodo precedente
		List* currLayer = getLayersInfo(node);
		List* prevLayer = getLayersInfo(getNext(node));

		//handle della coda del layer corrente
		Node* currTail = getTail(currLayer);
		//copia della coda del layer precendte
		Node* prevTailHandle = getTail(prevLayer);

		//variabili che engono riassegnate ogni ciclo
		Node* scanPrev = NULL;//handle che mi permette di scorrere il layer precedente
		Neuron* currNeuron = NULL;
		List* currWeights = NULL;//lista di pesi del Neurone corrente
		Node* currWeightsTail = NULL;//coda della lista di pesi del neurone corrente
		Neuron* prevNeuron = NULL;//handle al neurone precedente

		double tempVal = 0;
		while(currTail != NULL){//scorro per ogni neurone

			//aggiorno gli handle
			scanPrev = prevTailHandle;
			currNeuron = getLayerInfo(currTail);
			currWeights = getInputWeights(currNeuron);
			currWeightsTail = getTail(currWeights);

			if(getSize(currWeights) != getSize(prevLayer)){//controllo se la size dei pesi e' uguale a quella del layer precendte	
				return;
			}
		
			while(scanPrev != NULL){//scorro tra neuroni del layer precedente e pesi del neurone del layer corrente
				prevNeuron = getLayerInfo(scanPrev);//ottengo l'handle del neurone 
				
				tempVal = getInWeightValue(currWeightsTail);//ottengo il valore del peso corrente

				activationSum += (getActivation(prevNeuron)) * (tempVal);//calcolo la combinazione lineare tra pesi e neuroni 

				scanPrev = getPrev(scanPrev);
				currWeightsTail = getPrev(currWeightsTail);

			}
			setZ(currNeuron,activationSum+(getBias(currNeuron)));
			double actVal = 0;
			
			double(*actFunc)(double) = functions.squishFunc;
			actVal = actFunc(getZ(currNeuron));
			setActivation(currNeuron,actVal);//sommo il bias alla fine ed applico la squishification function 
		
			

			activationSum = 0;

			currTail = getPrev(currTail);
		}


	}
}

void saveLayers(Node* node,FILE* file){
	List* list = getLayersInfo(node);

	saveList(list,file);
}

void loadLayers(List* list,short int dim,FILE* file){
	for(int i = 0; i < dim; i++){
		List* l = loadList(file,loadLayer,freeLayer);
		setSaveFunc(l,saveLayer);
		add(list,&l);
	}
}