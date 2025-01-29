#include"imageRecView.h"
#include"functions.h"

const char* firstCommands[] = {"load","save","config","show","train","test","esc"};
const char* secondCommands[] = {"network","training","test","current"};
const char* thirdCommands[] = {"config","data","functions","layers","training"};
const char* functionKeywords[] = {"Cost","Act"};
const char* costFunctions[] = {"logLoss","mse"};
const char* actFunctions[] = {"sigmoid","relu","linear"};
const char* confNetLayers[] = {"simple","advanced"};
const char* simpleNetConf[] = {"numInputs","numHiddens","hiddenSize","numOutputs"};
const char* trainingTags[] = {"epochs","learningRate","trainMode"};
const char* trainingModes[] = {"online","miniBatch"};

const char* syntaxError = "\n...c'e' stato un errore nell'inserimento del comando, riprova...\n";



void startView(){

	boolean esc = false;
	char* input = malloc(sizeof(char)*LIMIT);
	Command command;

	ImageRecognizer* imgRec = NULL;

	if(createImageRecognizer(&imgRec) == -1) esc = error;//alloco lo spazio per imageRecognizer

	while(esc == false){

		printf("-->");

		fgets(input,LIMIT,stdin);
		input[strlen(input)-1] = '\0';//l'ultimo carattere letto da fgets è \n che deve essere sostituito con \0

		List* tokens = tokenize(input);

		if(isEmpty(tokens) == -1){
			command = parse(tokens);
			executeCommand(command,imgRec,tokens,&esc);
		}
		freeList(tokens);	
	}
	free(input);
	if(esc == error) printf("c'e' stato un errore con la rete neurale, verra' effettuato lo shutdown dell'app");


}

void executeCommand(Command command,ImageRecognizer* imgRec,List* tokens,boolean* esc){
	switch(command){
			case LOAD_N_CONFIG:
				loadNetworkConfig(tokens,imgRec,esc);
			break;
			case LOAD_TE_DATA:
				*esc = loadTestData(tokens,imgRec);
			break;
			case LOAD_TR_DATA:
				*esc = loadTrainData(tokens,imgRec);
			break;
			case LOAD_N_DATA:
				printf("\n...caricando i dati della rete neurale...\n");
				*esc = loadNetwork(tokens,imgRec);
			break;	
			case SAVE_N_DATA:
				printf("\n...salvando la rete neurale....\n");
				*esc = saveNetwork(tokens,imgRec);
			break;
			case CONFIG_N_FUNC:
				*esc = configFunctions(tokens,imgRec);
			break;
			case SHOW_N_FUNC:
				printf("\n----funzioni disponibili----\n");
				showFunctions();
			break;
			case CONFIG_N_LAYERS:
				*esc = configNetLayers(tokens,imgRec);
			break;
			case CONFIG_N_TRAIN:
				*esc = configNetTraining(tokens,imgRec);
			break;

			case TRAIN:
				printf("\n...iniziando la fase di training...\n");
				train(imgRec);
			break;

			case TEST:
				printf("\n...iniziando la fase di testing....\n");
				test(imgRec);
			break;

			case LOAD_C_CONFIG:
				loadCurrentConfig(imgRec);
			break;

			case ESC:
				printf("\n...uscita dal programma...\n");
				escProgram(imgRec);
				*esc = true;
			break;

			default:
				printf("...errore di sintassi , riprova...\n");

			break;

		}
}
List* tokenize(char* input){
	List* tokens = createList(sizeof(char*),freePrimitive);

	char* token = strtok(input," -=");
	while(token){
		add(tokens,&token);
		token = strtok(NULL," -=");
	}
	return tokens;
}

Command parse(List* tokens){
	Command command;
	Node* scan = getTail(tokens);
	unsigned int conta = 0;
	while(scan != NULL && conta<3){
		char* elem = getString(scan);
		if(conta == 0){
			for(int i = 0; i < FCOMMANDS;i++){
				if(strcmp(elem,firstCommands[i]) == 0) command = i+1;
			}
		}else if(conta == 1){
			for(int i = 0; i < SCOMMANDS;i++){
				if(strcmp(elem,secondCommands[i]) == 0) command +=((i+1)*10);
			}
		}else if(conta == 2){
			for(int i = 0; i < TCOMMANDS;i++){
				if(strcmp(elem,thirdCommands[i]) == 0) command +=((i+1)*100);
			}
		}
		conta++;
		scan = getPrev(scan);
	}
	return command;
}

boolean loadTestData(List* tokens,ImageRecognizer* imgRec){
	char* testPath;

	if(getSize(tokens) != 4){
		printf("%s",syntaxError);
		return false;
	}

	testPath = getStringAt(tokens,3);//ricordo che getStringAt utilizza l'indicizzazione degli array, 0-1-,..... quindi 3 = 2
	
	setTestImages(imgRec,loadImages(testPath));
	if(getTestImages(imgRec) == NULL)printf("\n...non e' stato possibile caricare le immagini percorso non valido...\n");
	return false;
}

boolean loadTrainData(List* tokens,ImageRecognizer* imgRec){
	char* trainPath;

	if(getSize(tokens) > 4){
		printf("%s",syntaxError);
		return false;
	}

	trainPath = getStringAt(tokens,3);
	setTrainImages(imgRec,loadImages(trainPath));
	if(getTrainImages(imgRec) == NULL)printf("\n...non e' stato possibile caricare le immagini percorso non valido...\n");
	return false;
}
boolean configFunctions(List* tokens,ImageRecognizer* imgRec){
	if(getSize(tokens) != 7){
		printf("%s",syntaxError);
		return false;
	}
	char* costKey;
	char* costValue;
	char* actKey;
	char* actValue;

	costKey = getStringAt(tokens,3);
	costValue = getStringAt(tokens,4);

	actKey = getStringAt(tokens,5);
	actValue = getStringAt(tokens,6);
	boolean costFound = false;
	boolean actFound = false; 

	CostFunction costF,costFPrime;
	ActFunction actF,actFPrime;
	//verifico se i comandi sono corretti
	if(strcmp(costKey,functionKeywords[0]) == 0 && strcmp(actKey,functionKeywords[1]) == 0){
		for(int i = 0; i < NCOST;i++){
			if(strcmp(costValue,costFunctions[i]) == 0 && isNumber(costValue) != 1){//controllo anche se inserisco lettere e non numeri
				if(i == 0){
					costF = logLoss;
					costFPrime = logLossPrime;
				}
				if(i == 1){
					costF = cost;
					costFPrime = costPrime;
				}
				costFound = true;
			}
		}

		for(int i = 0; i < NACT;i++){
			if(strcmp(actValue,actFunctions[i]) == 0 && isNumber(actValue) != 1){
				if(i == 0){
					actF = sigmoid;
					actFPrime = sigmoidPrime;
				}
				if(i == 1){
					actF = relu;
					actFPrime = reluPrime;
				}
				if(i == 2){
					actF = linearActv;
					actFPrime = linearActvPrime;
				}
				actFound = true;
			}
		}
		if(actFound == false || costFound == false)
			printf("\n...le funzioni inserite non corrispondono a nessuna di quelle disponibili...\n");
		else setNetFunctions(getNet(imgRec),getFunctions(costF,costFPrime,actF,actFPrime));
		return false;
	}

	printf("%s",syntaxError);



	return false;
}
void showFunctions(){
	printf("\n cost functions : ");
	for(int i = 0; i < NCOST;i++)
		printf("%s ",costFunctions[i]);
	printf("\n activation functions : ");
	for(int j = 0; j < NACT; j++)
		printf("%s ",actFunctions[j]);
	printf("\n");
}

boolean configNetLayers(List* tokens,ImageRecognizer* imgRec){
	if(getSize(tokens) > 4){
		//estraggo il tag che si trova nel 3 , token, ovvero nell'indice 4 della lista
		char* tag = getStringAt(tokens,3);
		unsigned int simple = 1,advanced = 1;
		//controllo
		if((simple = strcmp(tag,confNetLayers[0]))==0 || (advanced=strcmp(tag,confNetLayers[1]))==0){
			//adesso risolvo i due casi diversi 
			if(simple == 0 && getSize(tokens) == 12){//se voglio configurare la net nel modo semplice
				
					char* inTag = getStringAt(tokens,4);
					char* numIn = getStringAt(tokens,5);
					char* hidTag = getStringAt(tokens,6);
					char* numHid = getStringAt(tokens,7);
					char* sizeTag = getStringAt(tokens,8);
					char* sizeHid = getStringAt(tokens,9);
					char* outTag = getStringAt(tokens,10);
					char* numOuts = getStringAt(tokens,11);

					if(isNumber(numIn) == 1 && isNumber(numHid) == 1 && isNumber(sizeHid) == 1 && isNumber(numOuts) == 1){
						setCustomConf(imgRec,false);

						if(strcmp(inTag,simpleNetConf[0]) == 0 && strcmp(hidTag,simpleNetConf[1]) == 0
							&& strcmp(sizeTag,simpleNetConf[2]) == 0 && strcmp(outTag,simpleNetConf[3]) == 0){

							setLayerConf(imgRec,(unsigned int*)malloc(sizeof(unsigned int ) * 3));

							setNumLayers(imgRec,2+(atoi(numHid) * atoi(sizeHid)));
							getLayerConf(imgRec)[0] = atoi(numIn);
							getLayerConf(imgRec)[1] = atoi(sizeHid);
							getLayerConf(imgRec)[2] = atoi(numOuts);

							return false;
						}else{
							printf("...tag non validi , i tag da utilizzare devono essere : numInputs=value numHiddens=value hiddenSize=value numOutputs=value ...\n");
						}
					}else{
						printf("\n...i parametri inseriti devono essere necessariamente dei numeri...\n");
					}

					
				
			}
			if(advanced == 0 && getSize(tokens) == 5){//se voglio configurare la net nel modo advanced 
				setCustomConf(imgRec,true);
				
				List* layerConfig = obtainAdvancedConfig(getStringAt(tokens,4));
				if(layerConfig != NULL){
					setNumLayers(imgRec,getSize(layerConfig));
					setLayerConf(imgRec,(unsigned int*)toIntArray(layerConfig));
					freeList(layerConfig);

				}else{
					printf("\n...la sintassi utilizzata non è corretta, o sono state inserite delle lettere al posto dei  numeri nei parametri...\n");
				}

				return false;
			}
		}
	}
	printf("%s",syntaxError);
	return false;
}

List* obtainAdvancedConfig(char* configString){
	List* layerConfig = NULL;

	unsigned int charPresent = -1;//flag utilizzato per controllare se sono presenti delle lettere al posto dei numeri nei parametri

	//controllo se la stringa è formattata correttamente ie: {1,2,3,..}
	if(configString[0] == '{' && configString[strlen(configString)-1] == '}'){

		layerConfig = createList(sizeof(int),freePrimitive);

		char* lInfo = strtok(configString,"{,}");

		while(lInfo != NULL && charPresent == -1){
			if(isNumber(lInfo) == 1){
				unsigned int value = atoi(lInfo);
				add(layerConfig,&value);
				lInfo = strtok(NULL,"{,}");
			}else{
				charPresent = 1;
				freeList(layerConfig);
				layerConfig = NULL;
			}
		}
	}

	return layerConfig;
}


boolean configNetTraining(List* tokens,ImageRecognizer* imgRec){
	if(getSize(tokens) == 9){

		char* epochs = getStringAt(tokens,3);
		char* epochsValue = getStringAt(tokens,4);
		char* learn = getStringAt(tokens,5);
		char* learnValue = getStringAt(tokens,6);
		char* trainM = getStringAt(tokens,7);
		char* trainMValue = getStringAt(tokens,8);

		unsigned int sError = 1;

		//controllo la correttezza dei tag, altrimenti lancio un errore di sintassi
		if(strcmp(epochs,trainingTags[0]) == 0 && strcmp(learn,trainingTags[1]) == 0 &&
			strcmp(trainM,trainingTags[2]) == 0){

			//controllo se l'argomento di epochs e learningRate è un numero

			if(isNumber(epochsValue) == 1 && isNumber(learnValue) == 1){

			    char* realTag = strtok(trainMValue,"()");

				unsigned int online = strcmp(realTag,trainingModes[0]);
				unsigned int mini_batch = strcmp(realTag,trainingModes[1]);
			
				if(online == 0 || mini_batch == 0){

					setTrainData(imgRec,createTrainData(atoi(epochsValue)));
					double learnRate;
					sscanf(learnValue,"%lf",&learnRate);

					setLearnRate(getNet(imgRec),learnRate);

					if(online == 0){
						setTrainMode(imgRec,ONLINE);
						sError = -1;//la sintassi è corretta pertanto il flag può essere settato a false( nessun errore "-1")
					}
					if(mini_batch == 0){
						char* batchSz = strtok(NULL,"()");
						if(isNumber(batchSz) == 1){
							setTrainMode(imgRec,MINI_BATCH);
							setBatchSize(imgRec,atoi(batchSz)); 
							sError = -1;
						}
					}
				}
			}

			if(sError == 1 ) printf("%s",syntaxError);

			
			return false;
		}

	}
	printf("\n hai inserito i tag errati, riprova \n");
	return false;
}
void loadCurrentConfig(ImageRecognizer* imgRec){
	 if(initImageRecognizer(imgRec) != 1)
	 	printf("\n...errore,non e' possibile caricare la configurazione poiche' ancora non e' la rete neurale non e' stata configurata del tutto...\n");
}
void train(ImageRecognizer* imgRec){
	unsigned int err = -1;

	if(getTrainMode(imgRec) == MINI_BATCH){
		err = trainImageMiniBatch(imgRec);
	}else{
		err = trainImageOnline(imgRec);
	}
	if(err == -1) printf("\n...errore, non e' stato possibile iniziare il training poiche' la configurazione non e' corretta o non e' completa...\n");
	if(err == -2) printf("\n...error, non è stato possibile iniziare il training poichè il numero di neuroni di input non corrisponde con l'input scelto...\n");
}
void test(ImageRecognizer* imgRec){
	unsigned int err = testImageRecognizer(imgRec);
	if(err == -1)printf("\nerrore, non e' stato possibile iniziare il testing poiche' la rete neurale non e' stata ancora configurata...\n");
	if(err == -2)printf("\nerrore, non e' stato possibile iniziare il testing poiche' non sono state caricate immagini per il testing...\n");
	if(err == -3)printf("\nerrore, non e' stato possibile iniziare il testing poiche' il numero di neuroni di input non corrisponde all'input scelto...\n");
}
void escProgram(ImageRecognizer* imgRec){
	freeImageRecognizer(imgRec);
}
void loadNetworkConfig(List* tokens,ImageRecognizer* imgRec,boolean* esc){
	unsigned int sError = 1;

	if(getSize(tokens) == 4){
		char* path = getStringAt(tokens,3);
		
		FILE* fHandle = fopen(path,"r");

		
		char* chunk = (char*)malloc(sizeof(char)*LIMIT);

		Command command;
		if(chunk != NULL){
			if(fHandle != NULL){

				

				while(fgets(chunk,sizeof(char) * LIMIT,fHandle) != NULL){
					chunk[strlen(chunk)-1] = '\0';
					printf("\n%s\n",chunk);
					List* tokns = tokenize(chunk);
					command = parse(tokns);
					executeCommand(command,imgRec,tokns,esc);
					freeList(tokns);	
				}

				sError = -1;
				fclose(fHandle);
				free(chunk);

			}
		}else{
			*esc = error;//in questo caso mi ritorna error perchè l'errore è fatale (dovuto ad una corruzione della memoria)
		}
	}

	if(sError == 1)	printf("%s",syntaxError);
	
	
}

boolean saveNetwork(List* tokens,ImageRecognizer* imgRec){
	unsigned int sError = 0;
	if(getSize(tokens) == 4){
		char* filePath = getStringAt(tokens,3);
		sError = saveImageRecognizer(imgRec,filePath);
	}
	if(sError == 0) printf("%s",syntaxError);
	if(sError == -1) printf("\n...file non trovato , riprovare...\n");
	if(sError == -2) printf("\n...non è stato possibile salvare la rete neurale, poichè essa risulta ancora non inizializzata...\n");

	return false;
}

boolean loadNetwork(List* tokens,ImageRecognizer* imgRec){
	unsigned int sError = 0;
	if(getSize(tokens) == 4){
		char* filePath = getStringAt(tokens,3);
		sError = loadImageRecognizer(imgRec,filePath);
	}
	if(sError == 0) printf("%s",syntaxError);
	if(sError == -1) printf("\n...file non trovato , riprovare...\n");
	if(sError == -2) printf("\n...non è stato possibile salvare la rete neurale, poichè essa risulta ancora non inizializzata...\n");
	
	return false;
}

int isNumber(char* string){
	int numPoints = 0;

	for(int i = 0; i < strlen(string);i++){
		if(string[i] == '.' && numPoints < 1){
			numPoints++;
		}else if(isdigit(string[i]) == 0){
			return -1;
		}
	}
	return 1;
}