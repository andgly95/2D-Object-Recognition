//
//  ObjectDatabase.cpp
//  
//
//  Created by Andrew Glyadchenko on 9/29/17.
//
//

#include "ObjectDatabase.h"
#include <iostream>
using namespace ComputerVisionProjects;
using namespace std;

Object::Object(){
    
}

Object::Object(int label){
    objectLabel = label;
}

void Object::setSize(int size){
    objectSize = size;

}

void Object::setLabel(int label){
    objectLabel = label;
}

void Object::setMomentumInertia(int moment){
    min_moment_inert = moment;
}


int Object::getLabel(){
    return objectLabel;
}

int Object::getSize(){
    return objectSize;
}
double Object::getOrientation(){
    return orientation;
}
int Object::getMomentInertia(){
    return min_moment_inert;
}
int Object::getRowPos(){
    return center_row_pos;
}
int Object::getColPos(){
    return center_col_pos;
}

void Object::listObject(){
    cout << "Label: "<< objectLabel << endl;
    cout << "Area: "<< objectSize << endl;
    cout << "Center: " << center_row_pos << " : " << center_col_pos << endl;
    cout << "Orientation: " << orientation << endl;
    cout << "Moment: " << min_moment_inert << endl;
}

void Object::setCenter(int row, int collumn){
    center_row_pos = row;
    center_col_pos = collumn;
}

void Object::setOrientation(double angle){
    orientation = angle;
}

ObjectDatabase::ObjectDatabase(){
    
}

void ObjectDatabase::addObject(Object newObject){
    database.push_back(newObject);
}

bool ObjectDatabase::labelUsed(int labelCheck){
    for (int i=0;i<database.size();i++){
        if (database[i].getLabel() == labelCheck){
            return 1;
        }
    }
    return 0;
}

int ObjectDatabase::getSize(){
    return database.size();
}


bool ObjectDatabase::fullObject(int index, int threshhold){
    if (database[index].getSize() > threshhold) return 1;
    else return 0;
}
Object ObjectDatabase::getObject(int index){
    return database[index];
}

void ObjectDatabase::popLast(){
    database.pop_back();
}
