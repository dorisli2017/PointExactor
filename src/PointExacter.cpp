//============================================================================
// Name        : PointExacter.cpp
// Author      : GPL
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include "jsonparser.h"


int main(int argc, char *argv[]){
	char* s =argv[1];
	std::cout<< s<< std::endl;
	double width = atof(argv[2]);
	double height = atof(argv[3]);
	jsonParser jp(s,width, height);
	jp.print();
	jp.debug();
	jp.output();
	return 0;
}
