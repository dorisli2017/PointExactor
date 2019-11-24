#!/bin/bash
for file in json/*; 
do
./Debug/PointExacter $file 1080 720	
done


