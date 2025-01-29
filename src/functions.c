#include"functions.h"

NetFunctions getFunctions(CostFunction costFunc,CostFunction costFuncPrime,ActFunction squishFunc,ActFunction squishFuncPrime){
	NetFunctions functions;
	functions.costFunc = costFunc;
	functions.costFuncPrime = costFuncPrime;
	functions.squishFunc = squishFunc;
	functions.squishFuncPrime = squishFuncPrime;
	return functions;
}
double linearActv(double value){
	return value;
}
double linearActvPrime(double value){
	return 1;
}
double cost(double currValue,double expValue){
	return ((expValue-currValue)*(expValue-currValue))/2;
}
double costPrime(double currValue,double expValue){
	return -(expValue-currValue);
	
}
double sigmoid(double value){
	return 1/(1+exp(-value));
}
double relu(double value){
	if(value > 0) return value;
	else return 0;
}
double sigmoidPrime(double value){
	return sigmoid(value) * (1-sigmoid(value));
}
double logLoss(double pY,double y){
	return -((y*log(pY))+((1-y)*log(1-pY)));
}
double logLossPrime(double pY,double y){
	return ((y/pY)+((1-y)/(1-pY)));
}
double reluPrime(double value){
	if(value > 0) return 1;
	else return 0;
}

CostFunction getCostFunc(NetFunctions functions){
	return functions.costFunc;
}
void setCostFunc(NetFunctions* functions,CostFunction costFunc){
	functions->costFunc = costFunc;
}

CostFunction getCostFuncPrime(NetFunctions functions){
	return functions.costFuncPrime;
}
void setCostFuncPrime(NetFunctions* functions,CostFunction costFuncPrime){
	functions->costFuncPrime = costFuncPrime;
}

ActFunction getSquishFunc(NetFunctions functions){
	return functions.squishFunc;
}
void setSquishFunc(NetFunctions* functions,ActFunction squishFunc){
	functions->squishFunc = squishFunc;
}

ActFunction getSquishFuncPrime(NetFunctions functions){
	return functions.squishFuncPrime;
}
void setSquishFuncPrime(NetFunctions* functions,ActFunction squishFuncPrime){
	functions->squishFuncPrime = squishFuncPrime;
}

double softMax(List* outLayer,double z){
	double sum = 0;

	Node* scan = getTail(outLayer); 
	while(scan != NULL){
		Neuron* currNeuron = getLayerInfo(scan);
		sum += exp(getZ(currNeuron));
		scan = getPrev(scan);
	}

	//calcolo la softMax effettiva

	return (exp(z))/sum;
}

double softMaxPrime(List* outLayer,double z){
	return softMax(outLayer,z) * (1 - softMax(outLayer,z));
}
