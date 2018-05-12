#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

struct TrainingData
{
	double x1;
	double x2;
	int 	t;
};

class Neuron
{
public:
	Neuron(double learningRate,std::vector<double> weights ,double (*activation)(double))
	{
		this->learningRate = learningRate;
		this->weights = weights;
		this->activation = activation;
		this->bias = 1.0;
	}

	void run(std::vector<TrainingData>,int epochs);

private:
	double (*activation)(double);
	double learningRate;
	double bias;
	double a;
	std::vector<double> weights;
};

void Neuron::run(std::vector<TrainingData> td,int epochs)
{
	if (epochs <= 0)
	{
		std::cout << "need to run for at least 1 epochs!" << std::endl;
		return;
	}

	std::stringstream boundaryFunction;
	
	double delta = 0.0;

	std::cout << "\nLearning Rate: " << learningRate << std::endl;
	std::cout << "Initial Weights: " << "(" << weights[0] << "," << weights[1] << "," << weights[2] << ")" << std::endl;
	std::cout << "Epochs: " << epochs << "\n" << std::endl;

	for (int i = 0; i < epochs; i++)
	{
		for (int j = 0; j < td.size(); j++)
		{
			double net = weights[0]*td[j].x1 + weights[1]*td[j].x2 +weights[2]*bias;
			a = activation(net);
			if (a != td[j].t)
			{
				delta = learningRate * (td[j].t - a);
				weights[0] += delta * td[j].x1;
				weights[1] += delta * td[j].x2;
				weights[2] += delta * bias;
			}

			if (i == epochs-1)
			{
				std::cout << "r" << j+1 << " Final Updated Weights: (" << weights[0] << "," << weights[1] << "," << weights[2] << ") | A==T?: " <<  a << " == " << td[j].t << " (" << ((a == td[j].t)==1?"True":"False") << ")" << std::endl; 
			}
		}
	}

	std::cout << "\nBoundary Function = " << weights[0] << "x1 + " << weights[1] << "x2 + " << weights[2] << std::endl; 
}

void ReadData(std::vector<TrainingData> &td)
{
	std::ifstream data("training.csv");
	std::string line;
	int counter = 0;

	while(std::getline(data,line))
	{
		std::stringstream lineStream(line);
		std::string cell;
		counter = 0;
		TrainingData input;
		while(std::getline(lineStream,cell,','))
		{
			if (counter == 0)
				input.x1 = std::stod(cell);
			else if (counter == 1)
				input.x2 = std::stod(cell);
			else if (counter == 2)
				input.t = std::stoi(cell);
			counter++;
		}
		td.push_back(input);
	}
}

double ActivationFucntion(double net)
{	
	return net >= 0 ? 1.0 : -1.0;
}

double RandomRange(double lo, double hi)
{
	return lo + static_cast<double> (rand()) / (static_cast<double> (RAND_MAX / (hi - lo)));	
}

int main()
{
	
	std::vector<TrainingData> trainingData;
	ReadData(trainingData);

	//seed for random numbers
	srand(static_cast <unsigned> (time(0)));

	Neuron n = Neuron(
		RandomRange(0.1,0.4),
		{
			RandomRange(-1.0,1.0),
			RandomRange(-1.0,1.0),
			RandomRange(-1.0,1.0)
		},
		ActivationFucntion);

	std::cout << "\t\t\tTRAINING DATA FROM CSV: " << std::endl;
	std::cout << "|\tID\t|\tx1\t|\tx2\t|\tt\t|" << std::endl;
	for (int i = 0; i < trainingData.size(); i ++)
	{
		std::cout << "|\tr" << i+1 << "\t"
		<< "|\t" << trainingData[i].x1 << "\t"
		<< "|\t" << trainingData[i].x2 << "\t"
		<< "|\t"  << trainingData[i].t << "\t|"
		<< std::endl;
	}

	n.run(trainingData,10);
	return 0;
}