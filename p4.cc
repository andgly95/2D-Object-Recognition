//
//  p4.cc
//
// recognizes objects from the database
//
//  Created by Andrew Glyadchenko on 9/26/17.
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
    const string databaseFile(argv[2]);
    const string output_file(argv[3]);
    
    Image an_image;
    if (!ReadImage(input_file, &an_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }
    
    Image *imageReference = &an_image;
    
    ImageEditor an_image_editor(imageReference, 4);
    
    an_image_editor.readDatabase(databaseFile);
    an_image_editor.scanForObjects();
    
    if (!WriteImage(output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }
}
