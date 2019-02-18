import math
import numpy
import random
from matplotlib import pyplot

decayRate = 100
numEpoch = 50
bias = False
randomweight = False
randomOrder = True

def drawAccyVsEpoch(accuracy, epochSet, numEpoch):
	pyplot.plot(epochSet, accuracy)
	pyplot.xlabel('Epoch')
	pyplot.ylabel('Accuracy')
	pyplot.axis([0, numEpoch, 0, 1])
	pyplot.show()

def training(trainingImage, trainingLabel, numEpoch, bias, randomweight, randomOrder, decayRate):
	num_feature = 784

	if bias:
		num_feature += 1
		trainingImage = numpy.insert(trainingImage, 0, 1)

	if randomweight:
		weight = numpy.random.rand(numEpoch, 10, num_feature)
	else:
		weight = numpy.zeros((numEpoch, 10, num_feature))

	accuracy = []
	epochSet = []

	for epoch in range(1, numEpoch):
		wrong = numpy.ones(5000)
		weight[epoch] = weight[epoch-1]
		if randomOrder:
			temp = list(range(5000))
			random.shuffle(temp)
			for i in temp:
				result = []
				for digit in range(10):
					result.append(numpy.sum(numpy.multiply(weight[epoch, digit], trainingImage[i])))
			
				testResult = result.index(max(result))
				truth = int(trainingLabel[i])

				if truth == testResult:
					wrong[i] = 0
				else:
					mult = numpy.multiply(trainingImage[i], decayRate/(decayRate+ float(epoch)))
					weight[epoch, testResult] = numpy.subtract(weight[epoch, testResult], mult)
					weight[epoch, truth] = numpy.add(weight[epoch, truth], mult)

			print('Epoch:', epoch, ' Accuracy: ',  1 - numpy.sum(wrong)/5000)
			accuracy.append(1 - numpy.sum(wrong)/5000)
			epochSet.append(epoch)
		else:
			for i in range(5000):
				result = []
				for digit in range(10):
					result.append(numpy.sum(numpy.multiply(weight[epoch, digit], trainingImage[i])))
			
				testResult = result.index(max(result))
				truth = int(trainingLabel[i])

				if truth == testResult:
					wrong[i] = 0
				else:
					mult= numpy.multiply(trainingImage[i], decayRate/(decayRate+ float(epoch)))
					weight[epoch, testResult] = numpy.subtract(weight[epoch, testResult], mult)
					weight[epoch, truth] = numpy.add(weight[epoch, truth], mult)

			print('Epoch:', epoch, ' accuracy: ',  1 - numpy.sum(wrong)/5000)
			
			accuracy.append(1 - numpy.sum(wrong)/5000)
			epochSet.append(epoch)

	drawAccyVsEpoch(accuracy, epochSet, numEpoch)

	return weight[numEpoch - 1]


def analyze(testResult, testingLabel):
	accuracyCount = 0
	for i in range(0, 1000):
		if testResult[i] == int(testingLabel[i]):
			accuracyCount += 1
	print('Overall Accuracy: '+ str(accuracyCount/1000.0))

	classificationList = []
	for i in range(0, 10):
		classificationRate = 0.0
		classCount = 0
		for img in range(0, 1000):
			if i == int(testingLabel[img]):
				classCount += 1
				if i == testResult[img]:
					classificationRate += 1
		print('Classification rate for digit '+str(i)+': '+str(round(classificationRate/classCount, 3)))
		classificationList.append(classificationRate)
	
	print('\n')
	print('------Confusion Matrix------')
	confusionMatrix=[]
	for i in range(0, 10):
		colList = []
		for j in range(0, 10):
			classCount = 0
			confusion = 0
			for img in range(0, 1000):
				if i == int(testingLabel[img]):
					classCount += 1
				if testResult[img] == j and int(testingLabel[img]) == i:
					confusion += 1
			colList.append(round(confusion/float(classCount), 3))
		confusionMatrix.append(colList)
	
	for row in confusionMatrix:
		string = ''
		for col in row:
			string += str(round(col, 2)) + ' '
		print(string)

	return accuracyCount/1000.0

def handleImages(imagefile):
	images = []
	lineNum = 0
	image = []
	with open(imagefile, 'r') as fileRead:
		for line in fileRead:
			if lineNum == 28:
				lineNum = 0
				images.append(image)
				image = []
			lineNum += 1
			charNum = 0
			for c in line:
				if charNum < 28:
					if c == ' ':
						image.append(0)
						charNum += 1
					else:
						image.append(1)
						charNum += 1
				else:
					charNum = 0		
	images.append(image)
	return numpy.array(images)


def handleLabels(labelfile):
	labels = []
	with open(labelfile, 'r') as fileRead:
		for line in fileRead:
			labels.append(list(line)[0])
	return numpy.array(labels)

def drawWeights(weight):
	print('\n')
	pyplot.xlabel('x position')
	pyplot.ylabel('y position')
	pyplot.imshow(numpy.reshape(weight[8], (28, 28)), interpolation = None)
	pyplot.show()

def testing(testingImage, testingLabel, weight, bias):
	if bias:
		testingImage = numpy.insert(testingImage, 0, 1,)

	testResult = []
	for i in range(1000):
		result = []
		for digit in range(10):
			result.append(numpy.sum(numpy.multiply(weight[digit], testingImage[i])))
		testResult.append(result.index(max(result)))

	analyze(testResult, testingLabel)

def main():
	testingImage = handleImages('testimages')
	testingLabel = handleLabels('testlabels')
	trainingImage = handleImages('trainingimages')
	trainingLabel = handleLabels('traininglabels')
	weight = training(trainingImage, trainingLabel, numEpoch, bias, randomweight, randomOrder, decayRate)
	testing(testingImage, testingLabel, weight, bias)
	if bias == False:
		drawWeights(weight)
		
main()
input()


