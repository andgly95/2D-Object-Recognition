//
//  ImageEditor.cpp
//  
//
//  Created by Andrew Glyadchenko on 9/18/17.
//
//

#include "ImageEditor.h"
#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace ComputerVisionProjects;

ImageEditor::ImageEditor(){
    activeImage = nullptr;
    currentObject = 0;
    numberOfObjects = 0;
    equivalenceMap = nullptr;
    objectArray = nullptr;
    knownObjects = nullptr;
}

// Modes:
// 1) - BinaryConverter
// 2) - Object Labeler
// 3) - Database Generator

ImageEditor::ImageEditor(Image *addImage, int mode){
    activeImage = addImage;
    currentObject = 0;
    numberOfObjects = 0;
    equivalenceMap = nullptr;
    objectArray = nullptr;
    knownObjects = nullptr;
    if (mode == 2) {
        currentObject = 10;
        equivalenceMap = new EquivalenceTable(activeImage->num_rows() * activeImage->num_columns());
    }
    else if (mode == 3) {
        objectArray = new ObjectDatabase;
        numberOfObjects = addImage->num_gray_levels();
    }
    else if (mode == 4) {
        objectArray = new ObjectDatabase;
        numberOfObjects = addImage->num_gray_levels();
        knownObjects = new ObjectDatabase;
    }
    
    
}

void ImageEditor::binaryConverter(int threshhold){
    for (int i=0;i<activeImage->num_rows();i++){
        for (int j=0;j<activeImage->num_columns();j++){
            if (activeImage->GetPixel(i,j) < threshhold){
                activeImage->SetPixel(i,j,0);
            }
            else if (activeImage->GetPixel(i,j) >= threshhold){
                activeImage->SetPixel(i,j,1);
            }
        }
    }
    activeImage->SetNumberGrayLevels(1);
    return;
}

void ImageEditor::objectSegmenter(){

    firstPassLabeler();
    equivalencyResolver();
    secondPassLabeler();
}

void ImageEditor::firstPassLabeler(){
    int pixelsLabeled = 0;
    for(int i=0;i<activeImage->num_rows();i++){
        for(int j=0;j<activeImage->num_columns();j++){
            if(labelPixel(i,j)) {
                pixelsLabeled++;
            }
            else cout << "pixel not found\n";
        }
    }
    
    cout << "Pixels Labeled: " << pixelsLabeled << endl;
    cout << "Segments Found: " << currentObject << endl;
    equivalenceMap->resizeArray(currentObject);
    activeImage->SetNumberGrayLevels(currentObject);
    //equivalenceMap->printTable();
    //return equivalenceMap;
}

void ImageEditor::secondPassLabeler(){
    for(int i=0;i<activeImage->num_rows();i++){
        for(int j=0;j<activeImage->num_columns();j++){
            int currentPixel = activeImage->GetPixel(i,j);
            if(currentPixel != 0) {
                if(equivalenceMap->getEquivalence(currentPixel) > 0){
                    activeImage->SetPixel(i,j,equivalenceMap->getEquivalence(currentPixel));
                }
                else {
                    if(activeImage->GetPixel(i,j-1) > 1)
                        activeImage->SetPixel(i,j,activeImage->GetPixel(i,j-1));
                    if(activeImage->GetPixel(i,j+1) > 1)
                        activeImage->SetPixel(i,j,activeImage->GetPixel(i,j+1));
                }
            }
        }
    }
}

void ImageEditor::equivalencyResolver(){
    
    activeImage->SetNumberGrayLevels(equivalenceMap->resolveEquivalencies());
    //equivalenceMap->printTable();
}

int ImageEditor::labelPixel(int x, int y){
    //cout << "new pixel: "<< x << y << endl;
    if(x < 0 || x > activeImage->num_rows() || y < 0 || y > activeImage->num_columns()){
        //cout << "invalid\n";
        
        return 0;
    }
    if (activeImage->GetPixel(x,y) == 0){
        //cout << "background\n";
        return 1;
    }
    else {
        unsigned A = activeImage->GetPixel(x,y);
        unsigned B = activeImage->GetPixel(x,y-1);
        unsigned C = activeImage->GetPixel(x-1,y);
        unsigned D = activeImage->GetPixel(x-1,y-1);
        
        //   |-D-|-C-|
        //   |-B-|-A-|
        
        if(D != 0 && (B!= 0 || C!= 0)){
            if (C != B) equivalenceMap->addEquivalence((int)B,(int)C);
            activeImage->SetPixel(x,y,D);
            return 1;
        }
        else if(C != 0){
            activeImage->SetPixel(x,y,C);
            if (C != B && B != 0){
                equivalenceMap->addEquivalence((int)B,(int)C);
                return 1;
            }
            return 1;
        }
        
        else if(B != 0){
            activeImage->SetPixel(x,y,B);
            return 1;
        }
        else{
            activeImage->SetPixel(x,y,++currentObject);
            cout << currentObject << " is new\n";
            equivalenceMap->addNode(currentObject);
            return 1;
        }
    }
}

void ImageEditor::generateDatabase(string databaseFile){
    objectArray = new ObjectDatabase();
    for(int h=0;h<numberOfObjects+1;h++){
        Object newObject;
        int currentLabel = -1;
        int area = 0;

        
        int centerX = 0;
        int centerY = 0;
        
        double valA = 0;
        double valB = 0;
        double valC = 0;
        
        //Calculate Area and Center
        
        for(int i=0;i<activeImage->num_rows();i++){
            for(int j=0;j<activeImage->num_columns();j++){
                if(activeImage->GetPixel(i,j) != 0){
                    if (currentLabel == -1 && !objectArray->labelUsed(activeImage->GetPixel(i,j))) {
                        currentLabel = activeImage->GetPixel(i,j);
                        newObject.setLabel(currentLabel);

                        area++;
                        
                        centerX += i;
                        centerY += j;
                    }
                    else if(currentLabel == activeImage->GetPixel(i,j)){
                        
                        area++;
                        
                        centerX += i * 1;
                        centerY += j * 1;
                        
                        
                    }
                }
            }
        }
        
        centerX = centerX / area;
        centerY = centerY / area;
        
        //Calculate Moment of Least Inertia
        
        for(int i=0;i<activeImage->num_rows();i++){
            for(int j=0;j<activeImage->num_columns();j++){
                if(activeImage->GetPixel(i,j) != 0){
                    if(currentLabel == activeImage->GetPixel(i,j)){
                        
                        //cout << i-centerX << "," << j-centerY << endl;
                        valA += (double(i-centerX) * double(i-centerX));
                        valB += (double(i-centerX) * double(j-centerY));
                        valC += (double(j-centerY) * double(j-centerY));
                        
                        
                    }
                }
            }
        }
        valB = valB * 2;
        newObject.setSize(area);
        newObject.setCenter(centerX,centerY);
        double theta = atan2(valB, valA-valC)/2;
        double momentOfInertia = ((valA * pow((sin(theta)),2) - (valB*(sin(theta)*cos(theta)))+(valC*pow(cos(theta),2))));
        newObject.setOrientation(theta);
        newObject.setMomentumInertia(momentOfInertia);
        if(area > 200){
            
            cout << "\nLabel: " << currentLabel << endl;
            cout << "Area: " << area << endl;
            cout << "Center = (" << centerX << "," << centerY << ")\n";
            cout << "Moment of Least Intertia: " << int(momentOfInertia) << endl;
            cout << "Theta: " << theta << endl;
            DrawLine(centerX-3, centerY, centerX+3, centerY, 0, activeImage);
            DrawLine(centerX, centerY-3, centerX, centerY+3, 0, activeImage);
            DrawLine(centerX, centerY, 50*cos(theta)+centerX, 50*sin(theta)+centerY, 210, activeImage);
        }
        objectArray->addObject(newObject);
    }
    
    //Output Database
    
    ofstream dbOutput;
    dbOutput.open (databaseFile);
    for(int h=0;h<objectArray->getSize();h++){
        if(objectArray->fullObject(h,300)){
            dbOutput << objectArray->getObject(h).getLabel() << " " << objectArray->getObject(h).getRowPos() << " " << objectArray->getObject(h).getColPos() << " " << objectArray->getObject(h).getMomentInertia() << " " << objectArray->getObject(h).getOrientation() << " " <<objectArray->getObject(h).getSize() << endl;
        }
    }
    dbOutput.close();
}

void ImageEditor::readDatabase(std::string databaseFile){
    ifstream dbInput;
    dbInput.open (databaseFile);
    cout << "opened" << endl;
    
    while (!dbInput.eof()) {
        Object newObject;
        int size, label, row, collumn, moment;
        double angle;
        dbInput >> label >> row >> collumn >> moment >> angle >> size;
        //getline(label,row, collumn, moment, angle, size);
        newObject.setSize(size);
        newObject.setLabel(label);
        newObject.setCenter(row, collumn);
        newObject.setOrientation(angle);
        newObject.setMomentumInertia(moment);
        knownObjects->addObject(newObject);
    }
    knownObjects->popLast();
    dbInput.close();
    /*cout << "Objects: " << knownObjects->getSize() <<endl;
    for (int i=0; i<knownObjects->getSize();i++){
        knownObjects->getObject(i).listObject();
    }*/
    knownRatios = new double[knownObjects->getSize()];
    for (int i=0; i<knownObjects->getSize();i++){
        knownRatios[i] = double(knownObjects->getObject(i).getSize()) / double(knownObjects->getObject(i).getMomentInertia());
    }
}

void ImageEditor::scanForObjects(){
    objectArray = new ObjectDatabase();
    for(int h=0;h<numberOfObjects+1;h++){
        Object newObject;
        int currentLabel = -1;
        int area = 0;
        double ratio;
        
        
        int centerX = 0;
        int centerY = 0;
        
        double valA = 0;
        double valB = 0;
        double valC = 0;
        
        //Calculate Area and Center
        
        for(int i=0;i<activeImage->num_rows();i++){
            for(int j=0;j<activeImage->num_columns();j++){
                if(activeImage->GetPixel(i,j) != 0){
                    if (currentLabel == -1 && !objectArray->labelUsed(activeImage->GetPixel(i,j))) {
                        currentLabel = activeImage->GetPixel(i,j);
                        newObject.setLabel(currentLabel);
                        
                        area++;
                        
                        centerX += i;
                        centerY += j;
                    }
                    else if(currentLabel == activeImage->GetPixel(i,j)){
                        
                        area++;
                        
                        centerX += i * 1;
                        centerY += j * 1;
                        
                        
                    }
                }
            }
        }
        
        centerX = centerX / area;
        centerY = centerY / area;
        
        //Calculate Moment of Least Inertia
        
        for(int i=0;i<activeImage->num_rows();i++){
            for(int j=0;j<activeImage->num_columns();j++){
                if(activeImage->GetPixel(i,j) != 0){
                    if(currentLabel == activeImage->GetPixel(i,j)){
                        
                        //cout << i-centerX << "," << j-centerY << endl;
                        valA += (double(i-centerX) * double(i-centerX));
                        valB += (double(i-centerX) * double(j-centerY));
                        valC += (double(j-centerY) * double(j-centerY));
                        
                        
                    }
                }
            }
        }
        valB = valB * 2;
        newObject.setSize(area);
        newObject.setCenter(centerX,centerY);
        double theta = atan2(valB, valA-valC)/2;
        double momentOfInertia = ((valA * pow((sin(theta)),2) - (valB*(sin(theta)*cos(theta)))+(valC*pow(cos(theta),2))));
        newObject.setOrientation(theta);
        newObject.setMomentumInertia(momentOfInertia);
        
        ratio = double(newObject.getSize()) / double(newObject.getMomentInertia());
        
        for (int i=0; i<knownObjects->getSize();i++){
            if((ratio / knownRatios[i] * 100 > 93) &&(ratio / knownRatios[i] * 100 < 107)){
                cout << "Object " << i+1 << " detected\n";
                /*cout << "\nLabel: " << currentLabel << endl;
                cout << "Area: " << area << endl;
                cout << "Center = (" << centerX << "," << centerY << ")\n";
                cout << "Moment of Least Intertia: " << int(momentOfInertia) << endl;
                cout << "Theta: " << theta << endl;*/
                DrawLine(centerX-3, centerY, centerX+3, centerY, 210, activeImage);
                DrawLine(centerX, centerY-3, centerX, centerY+3, 210, activeImage);
                DrawLine(centerX, centerY, 50*cos(theta)+centerX, 50*sin(theta)+centerY, 210, activeImage);
            }
        }
        objectArray->addObject(newObject);
    }
}
