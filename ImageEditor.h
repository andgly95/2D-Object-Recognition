//
//  ImageEditor.h
//
//
//  Created by Andrew Glyadchenko on 9/18/17.
//
//

#ifndef ImageEditor_hpp
#define ImageEditor_hpp

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <math.h>
#include "image.h"
#include "EquivalenceTable.h"
#include "ObjectDatabase.h"

using namespace ComputerVisionProjects;

/**
 * @class ImageEditor
 * @brief A class that points to an Image object and supports operations for manipulating the image
 * @details
 * This object contains a pointer to an image object and contains methods that allow for converting
 * an image to a binary image, segmenting a binary image into seperately labeled objects, and generating a
 * database of objects in an image containing the area, position of center, orientation, and moment of
 * least inerta. The object stores an equivalence table for resolving labeling conflicts, and a database
 * for storing details about each object. These objects are only used when needed.
 */

class ImageEditor {
private:
    Image *activeImage;
    unsigned int currentObject;
    int numberOfObjects;
    EquivalenceTable* equivalenceMap;
    ObjectDatabase* objectArray;
    ObjectDatabase* knownObjects;
    double* knownRatios;
protected:
    
    /**
     *  First Pass Labeler
     * @call called by Object Segmenter
     * Goes through each pixel and calls Labeling Algorithm (labelPixel) on each one
     **/
    void firstPassLabeler();
    
    /**
     * Equivalency Resolver
     * @call by Object Segmenter
     * Counts number of individual objects and sets number of gray levels of image equal to number of objects
     **/
    void equivalencyResolver();
    
    /**
     * Second Pass Labeler
     * @call called by Object Segmenter
     * Goes through each pixel and relabels each according to the object it belongs to
     **/
    void secondPassLabeler();
 
    /**
     * Pixel Labeler
     * @params x and y are the position of the pixel being labeled
     * @call Called by First Pass Labeler
     * @algorithm Labels each pixel based on neighbors; if D is not a background, then A is labeled D
     *    |-D-|-C-| if D is a 0, then A is set to C or B. If C or B is different, then an equivalence is
     *    |-B-|-A-| added which is resolved by the Equivalency Table
     **/
    int labelPixel(int x, int y);
    
public:
    
    ImageEditor();
    
    /**
     *  Parameterized Constructor
     * @param addImage, takes a pointer to an image and sets the image as the active image
     * @param mode, creates database and equivalence tables only if needed by calling program
     * @post creates the ImageEditor object with the referenced Image loaded
     **/
    
    ImageEditor(Image *addImage, int mode);
    
    /**
     *  Binary Converter
     * @param threshhold the minimum value a pixel must be to register as an object
     * @post the image is converted into a binary image where background pixels are a 0 and objects are a 1
     **/
    
    void binaryConverter(int threshhold);
    
    /**
     * Object Segmenter
     * @pre the image must be a binary image file
     * @post each pixel is labeled according to the image it belongs to, the number of gray levels
     *       is set to the number of objects in the file
     **/
    void objectSegmenter();
    
    /**
     * Generate Database
     * @pre all objects must be labeled seperately
     * @post each object's label, area, orientation, center position and moment of least inertia is logged
     *       into a text file. The center is marked on each object in the image and a line is drawn through
     *       through the axis of least inertia
     **/
    void generateDatabase(std::string databaseFile);
    
    /**
     * Read Database
     * @pre a database file must exist with all the needed information for each object
     * @post each object in the database is read and stored into a seperate database, a ratio is calculated based on area and
     *       moment of minimum inertia to compare against other objects
     **/
    
    void readDatabase(std::string databaseFile);
    
    /**
     * Scan For Objects
     * @pre the database of known objects must be populated with calculated ratios
     * @post each object in the image is read and stored into a database, a ratio is calculated based on area and
     *       moment of minimum inertia, and is compared against known objects. If there is a match, the position and
     *       orientation of matched objects are displayed on the image
     **/
    
    void scanForObjects();
};

#endif /* ImageEditor_hpp */
