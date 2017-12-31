#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include "Perceptron.h"


#define window_width  1000 
#define window_height 1000 
#define FPS 10
using namespace std;


//utilizes a 2-dimenstional Euclidean space

class line{
	public:
		float get_y1(){return this->y1;}
		float get_x1(){return this->x1;}
		void set_x1(float x1){this->x1 = x1;}
		void set_y1(float y1){this->y1 = y1;}
		float get_y2(){return this->y2;}
		float get_x2(){return this->x2;}
		void set_x2(float x2){this->x2 = x2;}
		void set_y2(float y2){this->y2 = y2;}
	private:
		float x1;
		float y1;
		float x2;
		float y2;


};

line decision_boundary;



class point{
	public:
		point(){};
		float get_y(){return this->y;}
		float get_x(){return this->x;}
		void set_x(float x){this->x = x;}
		void set_y(float y){this->y = y;}

		float get_upper_label(){return this->upper_label;}
		void set_upper_label(float label){ this->upper_label = label;}
		

		float get_color(){return this->color;}
		void set_color(float color){this->color = color;}

		
		
		

	private:
		float x;
		float y;
		float upper_label;
	
		float color;
		
};


Perceptron1 learning_machine;


vector<point> points(500);

void train();
void draw_upper_bound(float , float ,float , float );
void draw_point();
void timer_callback(int);
void display_callback();		
void reshape_callback(int, int);
void draw_boundaries();
void update_boundary_lines();
void drawScale();
void initialize();


int main(int argc, char* argv[])
{
	srand(time(NULL));
	
	float x,y;	

	for (int i=0;i<500;i++){
		x = rand() % 2000 ;
		x = x/100;
		y = rand() % 2000 ;
		y = y/100;
		
		
		if ( x +y >= 8 )
		{
			points[i].set_upper_label(1);
			points[i].set_color(1);		
		}		
		else
		{
			points[i].set_upper_label(-1);
			points[i].set_color(0);
		}	
			
		points[i].set_x(x);
		points[i].set_y(y);
	}	
	
		
	decision_boundary.set_x1(-1);
	decision_boundary.set_x2(9);
	

   	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(600,10);
   	glutCreateWindow("2-FEATURE-PERCEPTRON VISUALIZATION");
	glutDisplayFunc(display_callback);
	glutReshapeFunc(reshape_callback);
	glutTimerFunc(0, timer_callback, 1000);
	initialize();
    	glutMainLoop();
	
    return 0;
}

void initialize()
{
	glClearColor(0.0,0.0,0.0,1.0);
}

void draw_upper_bound(float x1, float y1,float x2, float y2)
{
	glBegin(GL_LINES);
       	glVertex2f(x1*100, y1*100);
    	glVertex2f(x2*100, y2*100);
	glEnd();
}

void draw_boundaries()
{
	glColor3f(1.0,0.0,0.4);
	update_boundary_lines();
	draw_upper_bound(decision_boundary.get_x1(),decision_boundary.get_y1(),decision_boundary.get_x2(),decision_boundary.get_y2());
	
}

void update_boundary_lines()
{
	vector<float> w(2);
	
	w = learning_machine.get_weights(); 	
	
	decision_boundary.set_y1(    (8-(w[0]*decision_boundary.get_x1()))  /  w[1]   );
	decision_boundary.set_y2(    (8-(w[0]*decision_boundary.get_x2()))  /  w[1]   );	 
	
}


void drawScale() 
{  
	char s[4];

	//draw horizontal scale
	for(int i=0;i<9;i++)
	{
		s[0] = '0';
		s[1] = '0';
		s[2] = '0';		
		s[3] = '0'+i;
		glRasterPos2f(i*100, -50);
		for (int i=0;i<4; i++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
		}
	}
		
	
	
	//draw vertical scale
	for(int i=0;i<9;i++)
	{
		s[0] = '0';
		s[1] = '0';
		s[2] = '0';		
		s[3] = '0'+i;
		glRasterPos2f(-50, i*100);
		for (int i=0;i<4; i++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
		}
	}		
}

void draw_point(){

	int lineAmount = 50; 
	int radius = 5;
	
	float _2pi = 2.0 * 3.14159;
	
	for(int j=0;j<500;j++){
		points[j].set_x(points[j].get_x()*100);//multiply 100 to scale for appropriate viewing
		points[j].set_y(points[j].get_y()*100);
		if (points[j].get_color() == 1)
			glColor3f(1.0,0.0,0.4);
		else
			glColor3f(0.1,0.5,0.0);
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < lineAmount;i++) { 
			glVertex2f(
			    points[j].get_x() + (radius * cos(i *  _2pi / lineAmount)), 
			    points[j].get_y() + (radius* sin(i * _2pi / lineAmount))
			);
			
		}
		glEnd();
		points[j].set_x(points[j].get_x()/100);
		points[j].set_y(points[j].get_y()/100);
	}
	
}



void train(){
	vector<float> trainee(2);
	vector<float> w(2);
	double score_total=0.0;
	for (int i=0;i<500;i++)
	{
		trainee= {points[i].get_x(),points[i].get_y()};	
 		 
		score_total += learning_machine.train(trainee,points[i].get_upper_label());
	}
		
    	cout<<(score_total/500)*100<<"%"<<" complete"<<endl;
	if (score_total == 500)
	{
		w = learning_machine.get_weights();
		cout<<"AI can now make decisions based on 2 features (x1,x2) using the equation: ";
		cout<<endl<<"x1("<<w[0]<<") + x2("<<w[1]<<") "<<endl;
	}
}

void display_callback() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawScale();
	draw_point();
	draw_boundaries();//visualize the training
	train();
	glutSwapBuffers(); 
}

void reshape_callback(int w, int h)
{
	glViewport(0,0,window_width, window_height);
	glLoadIdentity();
	glOrtho(-(window_width/4), (window_width/(4/3)),-(window_width/4), (window_width/(4/3)),-1.0,1.0);
    			
	
}

void timer_callback(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000/FPS, timer_callback,0);
}
