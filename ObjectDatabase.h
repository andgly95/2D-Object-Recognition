//
//  ObjectDatabase.h
//  
//
//  Created by Andrew Glyadchenko on 9/29/17.
//
//

#ifndef ObjectDatabase_hpp
#define ObjectDatabase_hpp

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <vector>
#include "image.h"

using namespace ComputerVisionProjects;

/**
 * @class Object
 * @brief A class that stores attributes about objects such as size, position and rotation
 *
 */

class Object{
private:
    int objectLabel;
    int center_row_pos;
    int center_col_pos;
    int min_moment_inert;
    double orientation;
    
    int objectSize;
public:
    Object();
    Object(int label);
    
    //Setters
    void setSize(int size);
    void setLabel(int label);
    void setCenter(int row, int collumn);
    void setOrientation(double angle);
    void setMomentumInertia(int moment);
    
    //Getters
    int getLabel();
    int getSize();
    double getOrientation();
    int getMomentInertia();
    int getRowPos();
    int getColPos();
    void listObject();
    
};

/**
 * @class ObjectDatabase
 * @brief A class that stores a vector of Objects that can be added or retrieved
 *
 */

class ObjectDatabase {
private:
    std::vector<Object> database;
public:
    ObjectDatabase();
    
    /**
     * Add object.
     * @param newObject, the object to add to the database
     * @brief Adds an object to the Database vector
     */
    void addObject(Object newObject);
    
    /**
     * Label Used.
     * @param labelCheck, used to see if object already exists in database
     * @return true if object exists, false if object is new
     */
    bool labelUsed(int labelCheck);
    
    /**
     * Full Object.
     * @param index, to check specified object in database
     * @param threshhold checks if object area is larger than specified
     * @return true if object is larger than threshhold, false otherwise
     */
    bool fullObject(int index, int threshhold);
    
    //Getters
    int getSize();
    Object getObject(int index);
    void popLast();
    
};

#endif /* ObjectDatabase_hpp */
