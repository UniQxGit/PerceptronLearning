
CPP = g++ -std=c++11 -g -O -frounding-math -fsignaling-nans

all: perceptron test

test: perceptron_test
	./perceptron_test

perceptron: Perceptron.cc
	${CPP} Perceptron.cc -o perceptron_test

headers: TrainingData.hh

clean:
	rm -f perceptron_test
