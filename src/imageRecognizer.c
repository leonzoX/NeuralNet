#include"imageRecognizer.h"

int trainImageOnline(ImageRecognizer* imgRec){


	Image** images = getTrainImages(imgRec);
	TrainData trnData = getTrainData(imgRec);
	NeuralNet* net = getNet(imgRec);

	if(getLayers(net) != NULL && images != NULL && getEpochs(trnData) > 0 && getLearnRate(net) > 0){

		unsigned int i = 0;
		for(int k = 0; k < getEpochs(trnData); k++){

			i = k%NUMTRAIN;
			List* expVals = createList(sizeof(double),freePrimitive);

			for(int j = 0; j < 10;j++){
				double value = j == getLabel(images[0]) ?  getLabel(images[0]) : (double)-1;
				add(expVals,&value);
		    }
		    
			if(forwardPass(net,getData(images[0])) == NULL) return -2;;
			if(isPredictionCorrect(getLabel(images[0]),getOutputLayer(net)) == true) updateNCorrect(&trnData,1);

			backpropagate(net,expVals,ONLINE);

			
		

			setTrainErrorsAt(&trnData,getNetError(net),i);
			updateMeanError(&trnData,getNetError(net));

			//printOutput(outLayer);
			printf("\nerror : %lf : ",getNetError(net));

			updateWeights(net);
			freeList(expVals);
		} 
		setMeanError(&trnData,getMeanError(trnData)/getEpochs(trnData));
		setTrainData(imgRec,trnData);

		return 1;
	}
	return -1;
	
}
int testImageRecognizer(ImageRecognizer* imgRec){
	unsigned int retVal = -1;
	if(getLayers(getNet(imgRec)) != NULL){
		Image** images = getTestImages(imgRec);
		TrainData trnData = getTrainData(imgRec);
		if(images != NULL){
			for(int i = 0; i < NUMTEST;i++){
				printf("\n expectedValue : %d ",getLabel(images[i]));
				
				if(forwardPass(getNet(imgRec),getData(images[i])) == NULL) return -3;
				if(isPredictionCorrect(getLabel(images[i]),getOutputLayer(getNet(imgRec))) == true) updateNCorrect(&trnData,1);
					printf(" %d/%d\n",getNCorrect(trnData),NUMTEST);
			}
			retVal = 1;
			setTrainData(imgRec,trnData);

		}else{
			retVal = -2;
		}
	}
	return retVal;
	
}
int trainImageMiniBatch(ImageRecognizer* imgRec){

	Image** images = getTrainImages(imgRec);
	TrainData trnData = getTrainData(imgRec);
	unsigned int epochs = getEpochs(trnData);
	unsigned int batchSize = getBatchSize(imgRec);
	NeuralNet* net = getNet(imgRec);
   	double error = 0.0;
   	Mode mode;
   	unsigned int countErrors = 0;

   	srand(time(0));

   	if(getLayers(net) != NULL && images != NULL && epochs > 0 && batchSize > 0 && getLearnRate(net) > 0){
	   		for(int i = 0; i < epochs; i++){

				error = 0;

				for(int j = 0; j < batchSize;j++){

					List* expVals = createList(sizeof(double),freePrimitive);
					unsigned int r = rand()%NUMTRAIN;
					
					for(int t = 0; t < 10;t++){
						double value = t == getLabel(images[r]) ? getLabel(images[r]) : (double)-1;
						add(expVals,&value);
			    	}

					if(forwardPass(net,getData(images[r])) == NULL) return -2;
					mode = j == 0 ? ONLINE : MINI_BATCH;
					backpropagate(net,expVals,mode);
					error += getNetError(net);
					freeList(expVals);
				}
				error /= batchSize;

				setTrainErrorsAt(&trnData,error,countErrors);

				printf("\n errore : %lf",error);
				
				for(int k = 0; k< getNumberOfNeurons(net);k++){
					
					double* err = getErrorsAt(net,k);

					*err = *err / batchSize;
				}

				updateWeights(net);
				
				countErrors++;
			}
			setTrainData(imgRec,trnData);
			return 1;
   	}
   	return -1;
	
}

int saveImageRecognizer(ImageRecognizer* imgRec,char* filePath){

	unsigned int retFlag = -1;

	NeuralNet* net = getNet(imgRec);

	FILE* file;
	if(getLayers(net) != NULL){
			if((file = fopen(filePath,"wb")) != NULL){

			retFlag = 1;

			NetFunctions functions = getNetFunctions(net);

			char costFunction;
			char actFunction;

			

			//ImageRecognizer header------------
			Mode trainMode = getTrainMode(imgRec);
			boolean customConf = getCustomConf(imgRec);
			unsigned int batchSize = getBatchSize(imgRec);
			unsigned int epochs = getEpochs(getTrainData(imgRec));

			fwrite(&trainMode,sizeof(unsigned int),1,file);
			fwrite(&customConf,sizeof(unsigned int),1,file);
			fwrite(&batchSize,sizeof(unsigned int),1,file);
			fwrite(&epochs,sizeof(unsigned int),1,file);

			//Neural net header-----------------
			unsigned int numberOfNeurons = getNumberOfNeurons(net);
			unsigned int numLayers = getNumLayers(imgRec);
			double learningRate = getLearnRate(net);

			fwrite(&numberOfNeurons,sizeof(unsigned int),1,file);
			fwrite(&numLayers,sizeof(unsigned int),1,file);
			fwrite(getLayerConf(imgRec),sizeof(unsigned int),getNumLayers(imgRec),file);
			fwrite(&learningRate,sizeof(double),1,file);

			CostFunction cF = getCostFunc(functions);
			ActFunction aF = getSquishFunc(functions);

			if(cF == logLoss) costFunction = 'l';
			if(cF == cost) costFunction = 'm';

			if(aF == sigmoid) actFunction = 's';
			if(aF == relu) actFunction = 'r';
			if(aF == linearActv) actFunction = 'l';

			fwrite(&costFunction,sizeof(char),1,file);
			fwrite(&actFunction,sizeof(char),1,file);
			//-----------------------------------

			//body-----------------
			if(saveList(getLayers(net),file) == -1) retFlag = -1;

			fclose(file);
		}
	}else{
		retFlag = -2;
	}
	

	return retFlag;

}
int loadImageRecognizer(ImageRecognizer* imgRec,char* filePath){
	
	unsigned int retFlag = -1;

	FILE* file;
	if((file=fopen(filePath,"r")) != NULL){
		retFlag = 1;	

		NetFunctions functions;
		NeuralNet* net = getNet(imgRec);
		double learningRate;
		char c,a;

		Mode trainMode;
		fread(&trainMode,sizeof(unsigned int),1,file);
		setTrainMode(imgRec,trainMode);

		boolean customConf;
		fread(&customConf,sizeof(unsigned int),1,file);
		setCustomConf(imgRec,customConf);

		unsigned int batchSize;
		fread(&batchSize,sizeof(unsigned int),1,file);
		setBatchSize(imgRec,batchSize);

		unsigned int epochs;
		fread(&epochs,sizeof(unsigned int),1,file);
		setTrainData(imgRec,createTrainData(epochs));
		
		unsigned int n_Neurons;
		fread(&n_Neurons,sizeof(unsigned int),1,file);

		unsigned int n_Layers;
		fread(&n_Layers,sizeof(unsigned int),1,file);
		setNumLayers(imgRec,n_Layers);


		unsigned int* layerConf = (unsigned int*)malloc(sizeof(unsigned int) * n_Layers);
		fread(layerConf,sizeof(unsigned int),n_Layers,file);
		setLayerConf(imgRec,layerConf);

		fread(&learningRate,sizeof(double),1,file);
		setLearnRate(net,learningRate);

		fread(&c,sizeof(char),1,file);
		fread(&a,sizeof(char),1,file);

		if(c == 'l'){
			setCostFunc(&functions,logLoss);
			setCostFuncPrime(&functions,logLossPrime);
		}else if(c == 'm'){
			setCostFunc(&functions,cost);
			setCostFuncPrime(&functions,costPrime);
		}

		if(a == 's'){
			setSquishFunc(&functions,sigmoid);
			setSquishFuncPrime(&functions,sigmoidPrime);
		}else if(a == 'r'){
			setSquishFunc(&functions,relu);
			setSquishFuncPrime(&functions,reluPrime);
		}else if(a == 'l'){
			setSquishFunc(&functions,linearActv);
			setSquishFuncPrime(&functions,linearActvPrime);
		}

		setNetFunctions(net,functions);

		Layers layers = loadList(file,loadLayers,freeLayers);



		fromFileNet(getNet(imgRec),layers,learningRate,n_Neurons,functions);
		fclose(file);


	}
	
	return retFlag;

}



int createImageRecognizer(ImageRecognizer** imgRec){
	NeuralNet* net = NULL;
	if((*imgRec = (ImageRecognizer*)malloc(sizeof(ImageRecognizer))) != NULL && createNet(&net) != -1){
		setNet(*imgRec,net);
		TrainData trnData = {0,0,NULL,0};
		setTrainData(*imgRec,trnData);
		setTrainImages(*imgRec,NULL);
		setTestImages(*imgRec,NULL);
		setBatchSize(*imgRec,0);
		setNumLayers(*imgRec,0);
		setLayerConf(*imgRec,0);
		return 1;
	}
	return -1;

}


int initImageRecognizer(ImageRecognizer* imgRec){
	NeuralNet* net = getNet(imgRec);
	unsigned int retVal = -1;
	if(getCustomConf(imgRec) == true){
	 	retVal = initCustomNet(net,getNumLayers(imgRec),getLayerConf(imgRec),0);
	 }else{
	 	unsigned int* layerConf = getLayerConf(imgRec);
	 	if(layerConf != NULL){
	 		unsigned int numInputs = layerConf[0];
		 	unsigned int sizeHiddens = layerConf[1];
		 	unsigned int numOuts = layerConf[2];
		 	retVal = initDefNet(net,getNumLayers(imgRec),numInputs,numOuts,sizeHiddens,0);	
	 	}
	 }
	 return retVal;
}

boolean isPredictionCorrect(unsigned int expected,List* response){
	if(expected == predict(response)) return true;
	else return false;
}
unsigned int predict(List* response){
	Node* tail = getTail(response);
	
	int conta = 0;
	double max = 0.0000001;
	int maxIndex = -1;
	
	printf("\n");
	while(tail != NULL){
		Neuron* neuron = *(Neuron**)getInfo(tail);
		double value = getActivation(neuron);
		if(value > max){
			max = value;
			maxIndex = conta;
		} 
		tail = getPrev(tail);
		conta++;
	}
	return maxIndex;
}

void freeImageRecognizer(ImageRecognizer* imgRec){

	freeTrainData(&(imgRec->trainData));
	Image** train = getTrainImages(imgRec);
	Image** test = getTestImages(imgRec);

	if(train != NULL){
		for(int i = 0; i < NUMTRAIN;i++)
			freeImage(train[i]);
		free(train);
	}
	if(test != NULL){
		for(int i = 0; i < NUMTEST;i++)
			freeImage(test[i]);
		free(test);
	}
	if(getLayerConf(imgRec) != NULL)
		free(getLayerConf(imgRec));

	deleteNet(getNet(imgRec));
	free(imgRec);

}

TrainData getTrainData(ImageRecognizer* imgRec){
	return imgRec->trainData;
}
void setTrainData(ImageRecognizer* imgRec,TrainData trainData){
	imgRec->trainData = trainData;
}

Image** getTestImages(ImageRecognizer* imgRec){
	return imgRec->testImages;
}
void setTestImages(ImageRecognizer* imgRec,Image** testImages){
	imgRec->testImages = testImages;
}

Image** getTrainImages(ImageRecognizer* imgRec){
	return imgRec->trainImages;
}
void setTrainImages(ImageRecognizer* imgRec,Image** trainImages){
	imgRec->trainImages = trainImages;
}

NeuralNet* getNet(ImageRecognizer* imgRec){
	return imgRec->net;
}
void setNet(ImageRecognizer* imgRec,NeuralNet* net){
	imgRec->net = net;
}

unsigned int getBatchSize(ImageRecognizer* imgRec){
	return imgRec->batchSize;
}
void setBatchSize(ImageRecognizer* imgRec,unsigned int batchSize){
	imgRec->batchSize = batchSize;
}

unsigned int getNumLayers(ImageRecognizer* imgRec){
	return imgRec->numberOfLayers;
}
void setNumLayers(ImageRecognizer* imgRec,unsigned int numLayers){
	imgRec->numberOfLayers = numLayers;
}

boolean getCustomConf(ImageRecognizer* imgRec){
	return imgRec->customConf;
}
void setCustomConf(ImageRecognizer* imgRec,boolean customConf){
	imgRec->customConf = customConf;
}

unsigned int* getLayerConf(ImageRecognizer* imgRec){
	return imgRec->layerConf;
}
void setLayerConf(ImageRecognizer* imgRec,unsigned int* layerConf){
	imgRec->layerConf = layerConf;
}

Mode getTrainMode(ImageRecognizer* imgRec){
	return imgRec->trainingMode;
}
void setTrainMode(ImageRecognizer* imgRec,Mode trainMode){
	imgRec->trainingMode = trainMode;
}