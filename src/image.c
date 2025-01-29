#include"image.h"

Image* createImage(){
	Image* newImage = (Image*)malloc(sizeof(Image));
	newImage->label = 0;
	newImage->data = createList(sizeof(double),freePrimitive);
	return newImage;
}

void freeImage(Image* img){
	freeList(img->data);
	free(img);
	img = NULL;
}
unsigned int getLabel(Image* image){
	return image->label;
}
void setLabel(Image* image,unsigned int newLabel){
	image->label = newLabel;
}

List* getData(Image* image){
	return image->data;
}
void setData(Image* image,List* newData){
	image->data = newData;
}

Image** loadImages(const char* filePath){
	Image** images = (Image**) malloc(sizeof(Image*) * MAXIMGS);

	FILE* file = fopen(filePath,"r");
	if(file == NULL){
		return NULL;
	} 
	int nImages = 0;
	int row = 4500;
	char line[row];
	fgets(line,row,file);//skippo la prima linea
	while(fgets(line,row,file)){
		
		Image* newImage = createImage();

		char* token = strtok(line,",");
		int conta = 0;
		while(token != NULL){
			
			double tok = (double)atoi(token);
			//printf("%lf ",tok);
			if(conta == 0){
				setLabel(newImage,tok);
			}else{
				tok = tok/255;
			 	add(getData(newImage),&tok);
			}
			token = strtok(NULL,",");
			conta++;
		}
		images[nImages] = newImage;
		
		nImages++;
	}
	fclose(file);
	printf("numero immagini : %d \n",nImages);
	return images;
}