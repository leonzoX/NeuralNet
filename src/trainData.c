#include"trainData.h"

TrainData createTrainData(unsigned int epochs){
	TrainData trainData;
	trainData.epochs = epochs;
	trainData.errors = malloc(sizeof(double) * epochs);
	trainData.nCorrect = 0;
	trainData.meanError = 0.0;
	return trainData;
}

void freeTrainData(TrainData* trainData){
	if(trainData->errors != NULL)
		free(trainData->errors);
}
unsigned int getNCorrect(TrainData trainData){
	return trainData.nCorrect;
}
void setNCorrect(TrainData* trainData,unsigned int nCorrect){
	trainData->nCorrect = nCorrect;
}
void updateNCorrect(TrainData* trainData,unsigned int nUp){
	trainData->nCorrect += nUp;
}
unsigned int getEpochs(TrainData trainData){
	return trainData.epochs;
}
void setEpochs(TrainData* trainData, unsigned int epochs){
	trainData->epochs = epochs;
}

double* getTrainErrors(TrainData trainData){
	return trainData.errors;
}
void setTrainErrors(TrainData* trainData,double* errors){
	trainData->errors = errors;
}
void setTrainErrorsAt(TrainData* trainData,double value,unsigned int index){
	(trainData->errors)[index] = value;
}
double getMeanError(TrainData trainData){
	return trainData.meanError;
}
void setMeanError(TrainData* trainData,double meanError){
	trainData->meanError = meanError;
}
void updateMeanError(TrainData* trainData,double meanUp){
	trainData->meanError += meanUp;
}

void saveTrainData(TrainData trainData){
	const char* fileName = "train_data.csv";
	FILE* fHandle = fopen(fileName,"w");
	printf(" epochs : %d",getEpochs(trainData));
	fprintf(fHandle,"epochs,errors,\n");
	for(int i = 0; i< getEpochs(trainData);i++){
		fprintf(fHandle,"%d,%lf\n",i,(getTrainErrors(trainData))[i]);
	}
	fclose(fHandle);
}

double getAccuracy(TrainData trainData){
	return (double)getNCorrect(trainData) / (double)getEpochs(trainData);
}
