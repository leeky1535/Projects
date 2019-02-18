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
};

void draw_likelihoods(image& Plist, bool yes_or_no){
	if(yes_or_no == true)
		cout << "------likelihoods of yes" << "------" << endl;
	else if(yes_or_no == false)
		cout << "------likelihoods of no" << "------" << endl;

	for(int i = 0; i < 25; i++){
		for(int j = 0; j < 10; j++){
		  if(log(Plist.likelihoods[i][j]) > -1)
				cout << "+";
			else if(log(Plist.likelihoods[i][j]) <= -1 && log(Plist.likelihoods[i][j]) >= -2.5)
				cout << " ";
			else
				cout << "-";
		}
		cout << endl;
	}
}


void likelihoods(image* images, image& Plist, double num){
	double laplace = 1;
	image newimage;
	newimage.likelihoods = new double*[25];
	for(int row = 0; row < 25; row++){
		newimage.likelihoods[row] = new double[10];
		for(int col = 0; col < 10; col++){
			int pixelcount = 0;
			for(int j = 0; j < num; j++){
				if(images[j].pixel[row][col] != '%'){
					pixelcount += 1;
				}
			}
			newimage.likelihoods[row][col] = (pixelcount+laplace)/(num+laplace*2);
		}
		Plist = newimage;
	}
	if(num == 131){

	return;
	}

}

int find_max_index(double* array, int length){
	double max = -999999;
	int idx;
	for(int i = 0; i < length; i++){
		if((length == 50 && array[i] < 0) || length == 2){
			if(array[i] > max){
				idx = i;
				max = array[i];
			}
		}
	}
	return idx;
}

void printDigit(image& testImage){
	for(int row = 0; row < 25; row++){
		for(int col = 0; col < 10; col++){
			cout << testImage.pixel[row][col];
		}
		cout << endl;
	}
}


void testing(image* test, image* Plist, int* TestList, double* Proto, double temp){
	//printDigit(test[1]);
	for(int i = 0; i < 50; i++){
		double MAP[2];
		double total = 271;
		for(int num = 0; num < 2; num++){
			double result = 0;
			result += log(temp/total);
			for(int row = 0; row < 25; row++){
				for(int col = 0; col < 10; col++){
					if(test[i].pixel[row][col] == '%'){
						result += log(Plist[num].likelihoods[row][col]);
					}
					else{
						result += log(1-Plist[num].likelihoods[row][col]);
					}
				}
			}
			MAP[num] = result;
		}
		int maxIdx = find_max_index(MAP, 2);
		Proto[i] = MAP[maxIdx];
		TestList[i] = maxIdx;
	}
	cout << endl;
}




void yesAnalysis(image* testImages, int* testList, double* proto){
	int i;
	double accuracyCount = 0;
	double total = 50;
	double accuracy;
	for(i = 0; i < 50; i++){
		if(testList[i] == 0){
			accuracyCount++;
		}
	}
	accuracy = accuracyCount/total;
	cout << "Overall accuracy of yes: " << accuracy << endl;
}

void noAnalysis(image* testImages, int* testList, double* proto){
	int i;
	double accuracyCount = 0;
	double total = 50;
	double accuracy;
	for(i = 0; i < 50; i++){
		if(testList[i] == 1){
			accuracyCount++;
		}
	}
	accuracy = accuracyCount/total;
	cout << "Overall accuracy of no: " << accuracy << endl;
}

void handle_file(ifstream& file, image* images, int num_lines){
	string currline;
	if(file.is_open()){
		for(int i = 0; i < num_lines; i++){
			image newimage;
			newimage.pixel = new char*[25];
			for(int j = 0; j < 25; j++){
				getline(file, currline);
				newimage.pixel[j] = new char[10];
				for(int z = 0; z < 10; z++){
					newimage.pixel[j][z] = currline[z];
				}
			}
			images[i] = newimage;
			getline(file, currline);
			getline(file, currline);
			getline(file, currline);
		}
	}
}

void draw_confusion_matrix(int* yesTestList, int* noTestList){
	cout << "------Confusion Matrix------" << endl;

	double** confusionMatrix;
	confusionMatrix = new double*[2];
	confusionMatrix[0] = new double[2];
	confusionMatrix[1] = new double[2];
	int i, j;
	double accuracy, accuracyCount;
	for(i = 0; i < 50; i++){
		if(yesTestList[i] == 0){
			accuracyCount++;
		}
	}
	accuracy = accuracyCount/50;
	confusionMatrix[0][0] = accuracy;
	confusionMatrix[0][1] = 1 - accuracy;
	accuracyCount = 0;
	for(i = 0; i < 50; i++){
		if(noTestList[i] == 1){
			accuracyCount++;
		}
	}
	accuracy = accuracyCount/50;
	confusionMatrix[1][1] = accuracy;
	confusionMatrix[1][0] = 1 - accuracy;

	for(i = 0; i < 2; i++){
		for(j = 0; j < 2; j++){
			cout << confusionMatrix[i][j] << " ";
		}
		cout << endl;
	}
}

int main(int argc, char* argv[]){
	if (argc!= 6) {
		cout << "Error, usage: ./mp3 1 [yes_train] [no_train] [yes_test] [no_test] OR" <<endl;
		cout << "Error, usage: ./mp3 2 [training] [yes_test] [no_test] [don't matter]" << endl;
		cout << "mode 1: segmented, mode 2: unsegmented" << endl;
		return 1;
	}

	string yesTrainFile;
	string noTrainFile;
	string yesTestFile;
	string noTestFile;
	string unseg_trainingFile;
	string unseg_yesTestFile;
	string unseg_noTestFile;
	char mode = argv[1][0];
	switch (mode){

		case '1':
			yesTrainFile = argv[2];
			noTrainFile = argv[3];
			yesTestFile = argv[4];
			noTestFile = argv[5];
			break;
		case '2':
			unseg_trainingFile = argv[2];
			unseg_yesTestFile = argv[3];
			unseg_noTestFile = argv[4];
			break;
		default:
			cout << "not valid" << endl;
			return 1;

	}

	image yes_training[140];
	image no_training[131];
	image yes_test[50];
	image no_test[50];


	ifstream yesTrain (yesTrainFile.c_str());

	if(!yesTrain.is_open()) {
		cout << "Error opening file" <<endl;
		return 1;
	}

	handle_file(yesTrain, yes_training, 140);

	ifstream noTrain (noTrainFile.c_str());

	if(!noTrain.is_open()) {
		cout << "Error opening file" <<endl;
		return 1;
	}

	handle_file(noTrain, no_training, 131);

	ifstream yesTest (yesTestFile.c_str());

	if(!yesTest.is_open()) {
		cout << "Error opening file" <<endl;
		return 1;
	}

	handle_file(yesTest, yes_test, 50);

	ifstream noTest (noTestFile.c_str());

	if(!noTest.is_open()) {
		cout << "Error opening file" <<endl;
		return 1;
	}

	handle_file(noTest, no_test, 50);

	image Plist[2];
	likelihoods(yes_training, Plist[0], 140);
	likelihoods(no_training, Plist[1], 131);
	draw_likelihoods(Plist[0], true);
	draw_likelihoods(Plist[1], false);
	int yesTestList[50];
	double yesProto[50];
	int noTestList[50];
	double noProto[50];

	testing(yes_test, Plist, yesTestList, yesProto, 140);
	yesAnalysis(yes_test, yesTestList, yesProto);
	testing(no_test, Plist, noTestList, noProto, 131);
	noAnalysis(no_test, noTestList, noProto);

	draw_confusion_matrix(yesTestList, noTestList);

}
