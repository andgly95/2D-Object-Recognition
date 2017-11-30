//
//  EquivalenceTable.hpp
//  
//
//  Created by Andrew Glyadchenko on 9/25/17.
//
//

#ifndef EquivalenceTable_hpp
#define EquivalenceTable_hpp

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <vector>
#include "image.h"

using namespace ComputerVisionProjects;

/**
 * @class Node
 * @brief a node used to store equivalencies in disjoint sets
 * @details  each node stores the label of an object, it's rank, and a pointer to the label it is equivalent to
 **/

class Node {
public:
    Node();
    Node(int n);
    int data;
    int rank;
    Node* parent;
};

/**
 * @class Equivalence Table
 * @brief Creates a dynamically allocated array of equivalence nodes that allows for adding equivalent
 *        labels and finding the parent label for each labeled pixel
 * @details
 *        Uses a disjoint set to store every label used on pixels, and then merges equivalent nodes to point
 *        to a single parent
 **/

class EquivalenceTable {
private:
    Node* equivalenceArray;
    int size;
protected:
    
    /**
    * Find Node.
    * @param *Node n, reference to node to find the parent of
    * @return pointer to parent node
    * @brief Recursively calls itself until it finds the parent node
    */
    
    Node* find(Node* n);
    
    /**
     * Merge Nodes
     * @param *Node x & y, reference to nodes that are equivalent
     * @brief Finds parent nodes of x and y, and sets equivalence based on rank
     */
    
    void merge(Node* x, Node* y);
    
public:
    EquivalenceTable();
    
    /**
     * Parametrized Constructor.
     * @param size; number of nodes to create in the table
     * @post An array of node objects is created, equal in size to the specified parameter. Each position
     *       in the array corresponds to a label.
     */
    EquivalenceTable(int size);
    
    /**
     * Add Node.
     * @param a; The label number of the added label
     * @post gives the node at position a the data value of it's label (a)
     */
    
    void addNode(int a);
    
    /**
     * Add Equivalence.
     * @params a & b; The labels of the equivalent labels
     * @post the nodes belonging to labels a and b are merged, and both point to the parent
     */
    
    void addEquivalence(int a, int b);
    
    /**
     * Get Equivalence.
     * @param key; The labels of node to find the parent of
     * @return finds the original label of the parent node
     */
    
    int getEquivalence(int key);
    
    /**
     * Resolve Equivalencies.
     * @details counts number of original objects in array and stores them in new array to relabel objects
     * @return the number of distinct objects in the table
     */
    
    int resolveEquivalencies();
    
    void resizeArray(int newSize);
    int getSize();
    void printTable();
    
};



#endif /* EquivalenceTable_hpp */
