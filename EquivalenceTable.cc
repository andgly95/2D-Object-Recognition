//
//  EquivalenceTable.cpp
//  
//
//  Created by Andrew Glyadchenko on 9/25/17.
//
//

#include "EquivalenceTable.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

Node::Node() {
    data = 0;
    rank = 0;
    parent = this;
}

Node::Node(int n) {
    data = n;
    rank = 0;
    parent = this;
}

Node* EquivalenceTable::find(Node* n)
{
    if (n != n->parent) {
        n->parent = find(n->parent);
    }
    return n->parent;
}

void EquivalenceTable::merge(Node* x, Node* y)
{
    x = find(x);
    y = find(y);
    
    if (x->rank > y->rank) {
        y->parent = x;
    } else {
        x->parent = y;
        if (x->rank == y->rank) {
            ++(y->rank);
        }
    }
}


EquivalenceTable::EquivalenceTable(){
    equivalenceArray = nullptr;
    size = 0;
}

EquivalenceTable::EquivalenceTable(int arraySize){
    equivalenceArray = new Node[arraySize];
    size = arraySize;
}

void EquivalenceTable::addNode(int a){
    //Node* newNode = new Node(a);
    equivalenceArray[a].data = a;
}

void EquivalenceTable::addEquivalence(int a, int b){
    if(a != 0 && b!= 0){
        Node* nodeA = &equivalenceArray[a];
        Node* nodeB = &equivalenceArray[b];
        //cout << "Position "<< b <<" is equivalent to "<< a << endl;
        //cout << "Size of array = " << size << endl;
        merge(nodeA, nodeB);
    }
}

int EquivalenceTable::getEquivalence(int a){
    Node* findNode = &equivalenceArray[a];
    Node* parentNode = find(findNode);
    return parentNode->data;
}

void EquivalenceTable::resizeArray(int newSize){
    Node *newArray = new Node[newSize+1];
    for (int i=0;i<newSize;i++){
        newArray[i] = equivalenceArray[i];
    }
    size = newSize;
    Node *deleteArray = equivalenceArray;
    equivalenceArray = newArray;
    delete[] deleteArray;
}

int EquivalenceTable::getSize(){
    return size;
}

void EquivalenceTable::printTable(){
    for (int i=0;i<size;i++){
        cout << i << " : " << getEquivalence(i) << endl;
    }
}

int EquivalenceTable::resolveEquivalencies(){
    
    int trueObjectCount = 0;
    for(int i=1;i<size;i++){
        if(equivalenceArray[i].parent->data == i){
            trueObjectCount++;
        }
    }
    //cout << "number of objects" << trueObjectCount<< endl;
    Node *foundObjects = new Node[trueObjectCount+1];
    int newObjectNo = 0;
    for(int i=1;i<=size;i++){
        if(i == equivalenceArray[i].parent->data){
            newObjectNo++;
            foundObjects[newObjectNo].data = i;
        }
    }
    
     for(int i=1;i<=size;i++){
         for(int j=1;j<=newObjectNo;j++){
             if(equivalenceArray[i].parent->data == equivalenceArray[i].data){
                 cout << "Match found at " << i << endl;
                if(foundObjects[j].data == equivalenceArray[i].data){
                    cout << "Comparing against " << foundObjects[j].data << endl;
                    cout << "Replacing " << equivalenceArray[i].data << " with " << j << endl;
                    equivalenceArray[i].parent->data = j;
                }
             }
         }
     }
     printTable();
     return trueObjectCount;
}
