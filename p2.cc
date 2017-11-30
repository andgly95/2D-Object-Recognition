//
//  p2.cc
//
// segments a binary image into several connected regions
//
//  Created by Andrew Glyadchenko on 9/20/17.
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
    
    if (argc!=3) {
        printf("Usage: %s file1 file2\n", argv[0]);
        return 0;
    }
    const string input_file(argv[1]);
    const string output_file(argv[2]);
        
    Image an_image;
    if (!ReadImage(input_file, &an_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }
    
    Image *imageReference = &an_image;
    
    ImageEditor an_image_editor(imageReference, 2);
    
    an_image_editor.objectSegmenter();
    
    if (!WriteImage(output_file, an_image)){
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }
}
