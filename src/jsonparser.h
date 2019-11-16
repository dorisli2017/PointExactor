/*
 * jsonparser.h
 *
 *  Created on: Nov 16, 2019
 *      Author: ping
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_
#include<iostream>
#include<vector>
#include <stdlib.h>     /* atof */
#include <utility>
#include <typeinfo>
#include <string>     // std::string, std::stod
#include "json.hpp"
#include "string"
#include <iostream>
#include <limits>
#include <fstream>      // std::ifstream
using namespace std;
using json = nlohmann::json ;
using point= std::pair<double,double>;
constexpr double max_double = std::numeric_limits<double>::max();
constexpr double min_double = std::numeric_limits<double>::lowest();
class jsonParser{
private:
	int count;
	string fileName;
	json jObject;
	std::vector<point> points;
	std::vector<point> normPoints;
	double minX;
	double minY;
	double maxX;
	double maxY;
	double width;
	double height;
	double n_width;
	double n_height;
public:
	jsonParser(char* s,double normWidth, double normHeight);
	void print();
	void debug();
	void norm();
	void output();
};




#endif /* JSONPARSER_H_ */
