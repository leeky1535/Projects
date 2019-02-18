#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <cmath>
#include <unistd.h> //for sleep
#include <stack>
#include <vector>
#include <math.h>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

struct image{
	public:
		char** pixel;
		double** likelihoods;
		int label;
};

void prior(image* images, double* priorDict){
	for(int i = 0; i < 5000; i++){
		int num = images[i].label;
		priorDict[num]++;
	}
	for(int j = 0; j < 10; j++){
		//cout << priorDict[j] << endl;
	}
}

void draw_likelihoods(image* Plist, int digit){
	cout << "------likelihoods of digit " << digit << "------" << endl;
	for(int i = 0; i < 28; i++){
		for(int j = 0; j < 28; j++){
		  if(log(Plist[digit].likelihoods[i][j]) > -1)
				cout << "+";
			else if(log(Plist[digit].likelihoods[i][j]) <= -1 && log(Plist[digit].likelihoods[i][j]) >= -2.5)
				cout << " ";
			else
				cout << "-";
		}
		cout << endl;
	}
}


void likelihoods(image* images, double* priorDict, image* Plist){
	double laplace = 1;
	for(int i = 0; i < 10; i++){
		image newimage;
		newimage.likelihoods = new double*[28];
		for(int row = 0; row < 28; row++){
			newimage.likelihoods[row] = new double[28];
			for(int col = 0; col < 28; col++){
				int pixelcount = 0;
				for(int j = 0; j < 5000; j++){
					if(images[j].pixel[row][col] != ' ' && images[j].label == i){
						pixelcount += 1;
					}
					//if(images[j].pixel[row][col] == '#' && images[j].label == i){
					//	pixelcount += 0.5;
					//}
					//if(images[j].pixel[row][col] != '+' && images[j].label == i){
					//	pixelcount += 0.25;
					//}
				}
				newimage.likelihoods[row][col] = (pixelcount+laplace)/(priorDict[i]+laplace*10);
			}
		}
		Plist[i] = newimage;
	}
}


int find_max_index(double* array, int length){
	double max = -999999;
	int idx;
	for(int i = 0; i < length; i++){
		if((length == 1000 && array[i] < 0) || length == 10){
			if(array[i] > max){
				idx = i;
				max = array[i];
			}
		}
	}
	return idx;
}

int find_min_index(double* array, int length){
	double min = 999999;
	int idx;
	for(int i = 0; i < length; i++){
		if((length == 1000 && array[i] < 0) || length == 10){
			if(array[i] < min){
				idx = i;
				min = array[i];
			}
		}
	}
	return idx;
}

void testing(image* testImages, double* priorDict, image* Plist, int* testList, int* proto){
	for(int i = 0; i < 1000; i++){
		double MAP[10];
		double temp = 5000;
		for(int num = 0; num < 10; num++){
			double result = 0;
			result += log(priorDict[num]/temp);
			for(int row = 0; row < 28; row++){
				for(int col = 0; col < 28; col++){
					if(testImages[i].pixel[row][col] != ' '){
						result += log(Plist[num].likelihoods[row][col]);
					}
					else{
						result += log(1-(Plist[num].likelihoods[row][col]));
					}
				}
			}
			MAP[num] = result;
		}

		int maxIdx = find_max_index(MAP, 10);
		proto[i] = MAP[maxIdx];
		testList[i] = maxIdx;
	}
}

void printDigit(image& testImage){
	for(int row = 0; row < 28; row++){
		for(int col = 0; col < 28; col++){
			cout << testImage.pixel[row][col];
		}
		cout << endl;
	}
}

void drawOddRatio(image* Plist, int first, int second){
	cout << "------Odd ratios: Digit " << first << " over Digit " << second << "------" << endl;
	cout << "First Digit" << first << endl;
	draw_likelihoods(Plist, first);
	cout << "Second Digit" << second << endl;
	draw_likelihoods(Plist, second);
	image interpolation;
	interpolation.likelihoods = new double*[28];
	for(int i = 0; i < 28; i++){
		interpolation.likelihoods[i] = new double[28];
		for(int j = 0; j < 28; j++){
			interpolation.likelihoods[i][j] = log(Plist[first].likelihoods[i][j] / Plist[second].likelihoods[i][j]);
		}
	}
	cout << "------Final------" << endl;
	for(int i = 0; i < 28; i++){
		for(int j = 0; j < 28; j++){
		  if(interpolation.likelihoods[i][j] > 0.2)
				cout << "+";
			else if(interpolation.likelihoods[i][j] <= 0.2 && interpolation.likelihoods[i][j] >= -0.2)
				cout << " ";
			else
				cout << "-";
		}
		cout << endl;
	}
}


void analysis(image* testImages, int* testList, int* proto){
	int i, j, z;
	int accuracyCount = 0;
	for(i = 0; i < 1000; i++){
		if(testList[i] == testImages[i].label){
			accuracyCount++;
		}
	}
	cout << "Overall :" << accuracyCount/1000.0 << endl;

	//double classificationList[10];

	for(i = 0; i < 10; i++){
		double classificationRate = 0;
		int classcount = 0;
		for(j = 0; j < 1000; j++){
			if(i == testImages[j].label){
				classcount++;
				if(i == testList[j]){
					classificationRate++;
				}
			}
		}
		cout << "Classification rate for digit " << i << ": " << classificationRate/classcount << endl;
		//classificationList[i] = classificationRate;
	}

	cout << "------Confusion Matrix------" << endl;

	double** confusionMatrix;
	confusionMatrix = new double*[10];
	for(i = 0; i < 10; i++){
		confusionMatrix[i] = new double[10];
		for(j = 0; j < 10; j++){
			double classcount = 0;
			double confusion = 0;
			for(z = 0; z < 1000; z++){
				if(i == testImages[z].label){
					classcount++;
				}
				if(testList[z] == j && testImages[z].label == i){
					confusion++;
				}
			}
			double temp = confusion/classcount*1000.0;
			confusionMatrix[i][j] = round(temp)/1000.0;
		}
	}

	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
			cout << confusionMatrix[i][j] << " ";
		}
		cout << endl;
	}

	double** DigitInstance;
	DigitInstance = new double*[10];
	for(i = 0; i < 10; i++){
		DigitInstance[i] = new double[1000];
		for(j = 0; j < 1000; j++){
			if(testImages[j].label == i && testList[j] == i){
				DigitInstance[i][j] = proto[j];
			}
		}
		int largestIndex = find_max_index(DigitInstance[i], 1000);
		int smallestIndex = find_min_index(DigitInstance[i], 1000);
		cout << "------Prototypical instance for digit " << i << "------" << endl;
		cout << "largest: " << largestIndex << endl;
		printDigit(testImages[largestIndex]);
		cout << "smallest: " << smallestIndex << endl;
		printDigit(testImages[smallestIndex]);
	}
}


int main(int argc, char* argv[]){
	if (argc!= 5) {
		cout<< "Error, usage: ./mp3 [training images] [training labels] [test images] [test labels]" <<endl;
		return 1;
	}
	//char mode = argv[5][0];

	string trainingImagesFile = argv[1];
	string trainingLabelsFile = argv[2];
	string testImagesFile = argv[3];
	string testLabelsFile = argv[4];

	image images_training[5000];
	image images_test[1000];

	ifstream trainingImages (trainingImagesFile.c_str());

	if(!trainingImages.is_open()) {
		cout << "Error opening file" <<endl;
		return 1;
	}

	if(trainingImages.is_open()){
		for(int i = 0; i < 5000; i++){
			image newimage;
			newimage.pixel = new char*[28];
			for(int j = 0; j < 28; j++){
				string currline;
				getline(trainingImages, currline);
				newimage.pixel[j] = new char[28];
				for(int z = 0; z < 28; z++){
					//cout<< i << " " <<  j << " " << z << endl;
					newimage.pixel[j][z] = currline[z];
				}
			}
			images_training[i] = newimage;

		}
	}

	ifstream trainingLabels (trainingLabelsFile.c_str());

	if(!trainingLabels.is_open()) {
		cout << "Error opening file" <<endl;
		return 1;
	}

	if(trainingLabels.is_open()){
		for(int i = 0; i < 5000; i++){
			string currline;
			getline(trainingLabels, currline);
			images_training[i].label = currline[0] - 48;
		}
		for(int j = 0; j < 5000; j++){
			//cout << images_training[j].label << endl;
		}
	}


	ifstream testImages (testImagesFile.c_str());

	if(!testImages.is_open()) {
		cout << "Error opening file" <<endl;
		return 1;
	}

	if(testImages.is_open()){
		for(int i = 0; i < 1000; i++){
			image newimage;
			newimage.pixel = new char*[28];
			for(int j = 0; j < 28; j++){
				string currline;
				getline(testImages, currline);
				newimage.pixel[j] = new char[28];
				for(int z = 0; z < 28; z++){
					newimage.pixel[j][z] = currline[z];
				}
			}
			images_test[i] = newimage;
		}
	}


	ifstream testLabels (testLabelsFile.c_str());
	if(!testLabels.is_open()) {
		cout << "Error opening file" <<endl;
		return 1;
	}

	if(testLabels.is_open()){
		for(int i = 0; i < 5000; i++){
			string currline;
			getline(testLabels, currline);
			images_test[i].label = currline[0] - 48;
		}

	}




	double priorDict[10] = {0,0,0,0,0,0,0,0,0,0};
	prior(images_training, priorDict);
	image Plist[10];
	likelihoods(images_training, priorDict, Plist);

	int testList[1000];
	int proto[1000];
	testing(images_test, priorDict, Plist, testList, proto);
	analysis(images_test, testList, proto);

	drawOddRatio(Plist, 4, 9);
	drawOddRatio(Plist, 5, 3);
	drawOddRatio(Plist, 7, 9);
	drawOddRatio(Plist, 8, 3);

}
