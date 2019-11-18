/*
 * jsonparser.cpp
 *
 *  Created on: Nov 16, 2019
 *      Author: ping
 */
#include "jsonparser.h"
jsonParser::jsonParser(char* s, double normWidth, double normHeight){
	minX= max_double;
	minY=max_double;
	maxX=min_double;
	maxY=min_double;
	std::ifstream ifs;
	count=0;
	points.clear();
	normPoints.clear();
	fileName = string(s);
	try {
		ifs.open(s, fstream::in);
	  }
	  catch(std::ifstream::failure& e){
	    std::cerr << "Exception opening file: " << std::strerror(errno) << "\n";
	  }
	  jObject= json::parse(ifs);
	  double x,y;
	  for(auto e: jObject){
		  x = double(e["x"]);
		  y = double(e["y"]);
		  if(x > maxX) maxX = x;
		  if(y > maxY) maxY = y;
		  if(x < minX) minX = x;
		  if(y < minY) minY = y;
		  points.push_back(point(x,y));
		  count++;
	  }
	  width = maxX- minX;
	  height = maxY - minY;
	  n_width = normWidth;
	  n_height = normHeight;
	  assert(count == points.size());
	  assert(count == jObject.size());
	  assert(minX < maxX);
	  assert(minY < maxY);
	  debug();
	  print();
	  norm();
}
void jsonParser::print(){
	int i = 0;
	for(auto e: jObject){
		std::cout<< e << std::endl;
		double x = e["x"];
		double  y = e["y"];
		printf("%.9lf\n",x);
		printf("%.9lf\n",y);
		std::cout<<"("<< points[i].first <<","<< points[i].second<< ")" << std::endl;
		i++;
	}
	assert(i == count);
	std::cout<<"min_X: "<<minX<< std::endl;
	std::cout<<"max_X: "<<maxX<< std::endl;
	std::cout<<"min_Y: "<<minY<< std::endl;
	std::cout<<"max_Y: "<<maxY<< std::endl;
};
void jsonParser::debug(){
	  double x,y;
	  for(auto e: jObject){
		  x = double(e["x"]);
		  y = double(e["y"]);
		  assert(!(x > maxX));
		  assert(!(y > maxY));
		  assert(!(x < minX));
		  assert(!(y < minY));
	  }
	  for(auto e: points){
		  x = e.first;
		  y = e.second;
		  assert(!(x > maxX));
		  assert(!(y > maxY));
		  assert(!(x < minX));
		  assert(!(y < minY));
	  }

}
void jsonParser::norm(){
	  double x,y;
	  for(auto e: points){
		  std::cout<< "x:"<< x << endl;
		  std::cout<< "y:"<< y << endl;
		  x = (e.first-minX)/width*n_width;
		  y = (e.second-minY)/height*n_height;
		  std::cout<< "x:"<< x << endl;
		  std::cout<< "y:"<< y << endl;
		  std::cout<< "width:"<< width << endl;
		  std::cout<< "height:"<< height << endl;
		  std::cout<< "n_width:"<< n_width << endl;
		  std::cout<< "_nheight:"<< n_height << endl;
		  assert(!(x <0));
		  assert(x <= n_width);
		  assert(!(y <0));
		  assert(y <= n_height);
		  normPoints.push_back(point(x,y));
	  }
	  assert(points.size() ==  normPoints.size());
}
void jsonParser::output(){    // full file name
	string subString = substr(fileName.find("/json/") + 1); 
	int iend = subString.find("."); //this finds the first occurrence of "."
	//in string thus giving you the index of where it is in the string

	// Now iend can be -1, if lets say the string had no "." at all in it i.e. no "." is found.
	//So check and account for it.
	if (iend != -1)
	{
	    subString= subString.substr(0 , iend); //this will give abc
	}
    subString = "../INPUT/"+substring
	std::ofstream ofs;
	const char * outFile = subString.c_str();
	try {
		ofs.open(outFile, fstream::out);
	}
	catch(std::ifstream::failure& e){
		std::cerr << "Exception opening file: " << std::strerror(errno) << "\n";
	}
	  ofs <<"c "<< fileName <<std::endl ;
	  ofs <<"p UNWEIGHTED "<<count<<" "<<n_width<<" "<< n_height  <<std::endl ;
	  for(auto e: normPoints){
		  ofs<<"e "<< e.first<< " "<< e.second<< std::endl;
	  }
	  ofs.close();
}
