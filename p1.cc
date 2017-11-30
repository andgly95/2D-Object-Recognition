//
//  p1.cpp
//
// converts a gray–level image to a binary one using a threshold value:
//
//  Created by Andrew Glyadchenko on 9/18/17.
//
//

#include "ImageEditor.h"
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv){
    
    if (argc!=4) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 0;
    }
    const string input_file(argv[1]);
    const string threshold(argv[2]);
    const string output_file(argv[3]);
    
    int thresholdValue = stoi(threshold,nullptr,10);
    
    Image an_image;
    if (!ReadImage(input_file, &an_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }
    
    Image *imageReference = &an_image;
    
    ImageEditor an_image_editor(imageReference,1);
    
    an_image_editor.binaryConverter(thresholdValue);
    
    if (!WriteImage(output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }
}
