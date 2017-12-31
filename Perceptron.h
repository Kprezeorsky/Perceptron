#ifndef Perceptron1_h
#define Perceptron1_h

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <string>

using namespace std;



class Perceptron1{

	public:
	
		Perceptron1();
		vector<float> get_weights();
		int guess(vector<float>,float);
		int sign(float );
		double train(vector<float> , int );
	

	private:
		vector<float> weights;
		float learning_rate;
		
		
};


Perceptron1::Perceptron1(){
	learning_rate = .0001;
	srand(time(NULL));
	float number;		
	for (int i=0;i<2;i++){
		number = (float)(((rand() %10) - 5)   );		
		weights.push_back(number);
		
	}
	
}

vector<float> Perceptron1::get_weights(){
	return weights;
}


int Perceptron1::guess(vector<float> feature, float target){
	
	float sum=0;
	for (int i=0;i<feature.size();i++){
		sum+= feature[i] * weights[i];
	}
	
	
	return sign(sum);
}

double Perceptron1::train(vector<float> feature, int target){
	
	int guess_value = guess(feature, target);
	double correct_decision=0.0;
	
	if (guess_value != target)
	{	
		cout<<"Still making some incorrect decisions..."<<endl;
		for (int i=0;i<weights.size();i++)
		{
			weights[i] = weights[i] + (target * feature[i] * learning_rate) ;
						
		}
		
		
	}
	else
	{
		correct_decision=1;
	}
	return correct_decision;


}

int Perceptron1::sign(float sum){
	if (sum>=8)
		return 1;
	else
		return -1;
}


#endif

