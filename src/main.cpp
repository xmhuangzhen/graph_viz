#include <iostream>
#include "SimpleGraph.h"
#include <fstream>
#include <cmath>
#include <ctime>

using std::cout;	using std::endl;

void Welcome();

const double krepel1 = 7e-4, krepel2 = 5e-8;
const double kattract1 = 6e-4, kattract2 = 5e-6;
const double PI=3.14159265358979323;
double delta = 5e-4;
const int P = 998244353;

SimpleGraph graph;
std::vector <Node> change;
std::vector <Edge> connect;

int main() {
	srand(time( 0 ));

	time_t startTime = time(NULL);

    std::cout << "please input the filename" << endl;

    std::string str;
    std::cin >> str;
    std::fstream f(str);
    
    InitGraphVisualizer (graph);
    
    int n;
	f >> n;

	for(int i = 0; i < n; ++i){
		Node t; t.x = cos(2 * PI * i / n); t.y = sin(2 * PI * i / n);
		graph.nodes.push_back(t);
	}

	size_t x, y;
	while(f >> x >> y){
		Edge t; t.start = x; t.end = y;
		graph.edges.push_back(t);
	}

	for(int i = 0; i < n; ++i){
		Node t; t.x = 0; t.y = 0;
		change.push_back(t);
	}

	//extension 6
	for(unsigned int i = 0; i < graph.edges.size(); ++i){

		Edge t1 = graph.edges[i];
		
		for(unsigned int j = i + 1; j < graph.edges.size(); ++j){

			Edge t2 = graph.edges[j], tmp;

			if(t1.start == t2.start && t1.end != t2.end) {
				tmp.start = t1.end; tmp.end = t2.end;
				connect.push_back(tmp);
			}

			if(t1.end == t2.start && t1.start != t2.end) {
				tmp.start = t1.start; tmp.end = t2.end;
				connect.push_back(tmp);
			}

			if(t1.start == t2.end && t1.end != t2.start) {
				tmp.start = t1.end; tmp.end = t2.start;
				connect.push_back(tmp);
			}

			if(t1.end == t2.end && t1.start != t2.start) {
				tmp.start = t1.start; tmp.end = t2.start;
				connect.push_back(tmp);
			}
		}
	}

	DrawGraph(graph);

	int NUM = 0;

	while(1){
		
		//extension 2
		delta /= (rand() % 10 + 1);
		for(int i = 0; i < n; ++i){
			NUM += rand() % P;
			if( NUM > P) NUM -= P;
            if(NUM % 20 == 1 ) change[i].x -= delta;
            if(NUM % 17 == 12) change[i].y -= delta;
            if(NUM % 20 == 0 ) change[i].x += delta;
            if(NUM %  7 == 1 ) change[i].y += delta;
        }

		//repel
		for(unsigned int i = 0; i < n; ++i)
			for(unsigned int j = i + 1; j < n; ++j) {

                double X0 = graph.nodes[i].x, Y0 = graph.nodes[i].y;
                double X1 = graph.nodes[j].x, Y1 = graph.nodes[j].y;
                double distance = (X0 - X1) * (X0 - X1)	+ (Y0 - Y1) * (Y0 - Y1);

                double Frepel = krepel1 / sqrt(distance);

				double angle = atan2(Y1 - Y0, X1 - X0);

				change[i].x -= Frepel * cos(angle);
				change[i].y -= Frepel * sin(angle);
				change[j].x += Frepel * cos(angle);
				change[j].y += Frepel * sin(angle);
			}

		//attract1
    	for(unsigned int i = 0; i < graph.edges.size(); ++i){
    		
			Edge t; t = graph.edges[i];
			
            double X0 = graph.nodes[t.start].x, Y0 = graph.nodes[t.start].y;
            double X1 = graph.nodes[t.end].x,   Y1 = graph.nodes[t.end].y;
            double distance = (X0 - X1) * (X0 - X1)	+ (Y0 - Y1) * (Y0 - Y1);
			
			double angle = atan2(Y1 - Y0, X1 - X0);
			double Fattract = kattract1 * distance;

			change[t.start].x += Fattract * cos(angle);
			change[t.start].y += Fattract * sin(angle);
			change[t.end].x   -= Fattract * cos(angle);
			change[t.end].y   -= Fattract * sin(angle);

		}

		//extension6 attract2
    	for(unsigned int i = 0;i < connect.size();++i){
    		
			Edge t; t = connect[i];

			double X0 = graph.nodes[t.start].x, Y0 = graph.nodes[t.start].y;
			double X1 = graph.nodes[t.end].x,   Y1 = graph.nodes[t.end].y;
			double distance = (X0 - X1) * (X0 - X1)	+ (Y0 - Y1) * (Y0 - Y1);
			
			double angle = atan2(Y1 - Y0, X1 - X0);
			double Fattract = kattract2 * distance;

			change[t.start].x += Fattract * cos(angle);
			change[t.start].y += Fattract * sin(angle);
			change[t.end].x   -= Fattract * cos(angle);
			change[t.end].y   -= Fattract * sin(angle);
		}

        //extension 4
        if(graph.edges.size() / n < n / 3 ){
        	for(unsigned int i = 0;i < graph.edges.size(); ++i){
            	int C0 = graph.edges[i].start, C1 = graph.edges[i].end;
            	Node A0 = graph.nodes[C0], A1 = graph.nodes[C1], A2;
            	double X0 = (A1.x + A0.x) / 2, Y0 = (A1.y + A0.y) / 2;

            	for(unsigned int j = i + 1;j < graph.edges.size(); ++j){

            		int D0 = graph.edges[j].start, D1 = graph.edges[j].end;
            		Node B0 = graph.nodes[D0], B1 = graph.nodes[D1], B2;
                	double X1 = (B0.x - B1.x) / 3, Y1 = B0.y + (B1.y - B0.y) / 3;

                	double distance = (X0 - X1) * (X0 - X1) + (Y0 - Y1) * (Y0 - Y1);

                	double Frepel = krepel2 / sqrt(distance);
                	//if(distance <= 1e-7)continue;

                	double angle = atan2(Y1 - Y0, X1 - X0);
					change[C0].x -= Frepel * cos(angle); change[C0].y -= Frepel * sin(angle);
					change[C1].x -= Frepel * cos(angle); change[C1].y -= Frepel * sin(angle);

					change[D0].x += Frepel * cos(angle); change[D0].y += Frepel * sin(angle);
					change[D1].x += Frepel * cos(angle); change[D1].y += Frepel * sin(angle);
            	}
        	}
        }

		for(int i = 0; i < n; ++i){
			graph.nodes[i].x += change[i].x;
			graph.nodes[i].y += change[i].y;

			//extension 3
			change[i].x *= 0.1;
			change[i].y *= 0.1;
		}

		DrawGraph(graph);

		double elapsedTime = difftime(time(NULL) , startTime);
		
        if(elapsedTime >= 1000.0)break;
	}

    return 0;
}
