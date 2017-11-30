RRRR EEEE  AA  DDD    M   M EEEE
R  R E    A  A D  D   MM MM E
RRRR EEEE AAAA D  D   M M M EEEE
R R  E    A  A D  D   M   M E
R  R EEEE A  A DDD    M   M EEEE

Andrew Glyadchenko
Assignment 2

For this assignment I managed to complete all four parts, which work almost perfectly. I created three classes to do most of the work, an ImageEditor class to execute most of the tasks, an EquivalenceTable class to handle resolving equivalencies in p2, and an ObjectDatabase to help store object attributes for p3 and p4.

The first part of the assignment, p1, I did not have much difficulty at all with completing, although when I tried setting the value of foreground objects equal to 1, they did not show up when viewing the image through most image editors, even with the number of gray levels set to 1. However once I tried it on Gimp they did become visible, so I kept the values at 1.

Part 2 was the part I had the most difficulty with. The labeling algorithm was relatively easy to figure out, but I needed a way to store equivalent values. The first several methods I used seemed to cause unexplainable bugs. I tried using a vector, then a map, but each one seemed to cause bugs in the rest of the program that appeared completely unrelated. Afterwards I tried using an array of integers to store equivalencies and then used a recursive algorithm to find the parents but that also caused some bugs that cut the objects into too many segments so I had to scrap that. Finally I implemented a simple disjointed set using nodes and that seemed to fix 99% of the problems. However there still appear to be some lines that the algorithm doesn’t seem to notice, but since they don’t register as separate objects, they don’t really interfere with the rest of the program.

For part 3 I had less difficulty getting it to work, it just took a while to figure out how to implement the formulas to get the area, center, orientation and moment of least inertia, but the end result seems pretty accurate. My version outputs the image with black crosshairs on the center of each object and with a white line pointing in the direction of the axis of the moment of least inertia. All of the attributes, including area, are written to the text file which can be read by part 4.

Part 4 wasn’t too hard either, first I had to write a method to read the object attributes from the file and put it into a database array for found objects. I realized that the most accurate way to compare objects was by measuring the ratio of the area to the moment of inertia, so I stored a separate array of ratios whose indices correspond to those in the found object database. Afterwards, I just read in all the objects from the input image, calculated the ratios and compared to the known images. For that were a match within 7%, I marked the center and orientation on the outputted image.

The programs should compile with the make all command.

For p1 the command to run is ./p1 inputfile.pgm threshhold outputfile_clean.pgm 

For p2 the command to run is ./p2 inputfile_clean.pgm outputfile_segmented.pgm

For p3 the command to run is ./p3 inputfile_segmented database.txt outputfile_centered

For p4 the command to run is ./p4 inputfile_segmented database.txt outputfile_found