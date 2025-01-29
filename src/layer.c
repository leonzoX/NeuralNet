#include"layer.h"

void freeLayer(Node* node){
	Neuron* neuron = *(Neuron**)getInfo(node);
	deleteNeuron(neuron);
	free(getInfo(node));
	free(node);
}
void printLayer(Node* node){
	printf("\n{\n");
	List* listHandle = *(List**)getInfo(node);
	loopT(listHandle,printNeuron);
	printf("\n}\n");
}

Neuron* getLayerInfo(Node* layerNode){
	return *(Neuron**)(getInfo(layerNode));
}
void setLayerInfo(Node* layerNode,Neuron* newInfo){
	*(Neuron**)(getInfo(layerNode)) = newInfo;
}

void saveLayer(Node* node,FILE* file){
	Neuron* neuron = getLayerInfo(node);
	double activation = getActivation(neuron);
	double z = getZ(neuron);
	double error = getError(neuron);
	double bias = getBias(neuron);

	fwrite(&activation,sizeof(double),1,file);
	fwrite(&z,sizeof(double),1,file);
	fwrite(&error,sizeof(double),1,file);
	fwrite(&bias,sizeof(double),1,file);

	List* inputWeights = getInputWeights(neuron);
	saveList(inputWeights,file);
}

void loadLayer(List* list,short int dim,FILE* file){
	for(int i = 0; i < dim;i++){
		Neuron* neuron = (Neuron*)malloc(sizeof(Neuron));
		double activation,z,error,bias;

		fread(&activation,sizeof(double),1,file);
		fread(&z,sizeof(double),1,file);
		fread(&error,sizeof(double),1,file);
		fread(&bias,sizeof(double),1,file);

		setActivation(neuron,activation);
		setZ(neuron,z);
		setError(neuron,error);
		setBias(neuron,bias);

		List* inputWeights = loadList(file,loadPrimitive,freePrimitive);
		List* outputWeights = createList(sizeof(Node*),freeOutputWeights);

		setSaveFunc(inputWeights,saveDouble);
		setInputWeights(neuron,inputWeights);
		setOutputWeights(neuron,outputWeights);
		

		add(list,&neuron);
	}

}