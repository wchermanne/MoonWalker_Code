//
//  Astar_struct_gr1.h
//
//
//  Created by William Chermanne on 4/03/17.
//
//

#include <stdio.h>
#ifndef Wheels_gr1_hpp
#define Wheels_gr1_hpp
#include "MyIncludes_gr1.h"
#include "ctrl_io.h"
#include <stdlib.h>
//NAMESPACE_INIT(ctrlGr1);


typedef struct Node
{
    int index;
    double x;
    double y;
    int walkable;
    int parent_index;

    int isInClosedList;
    int isInOpenList;
    double Hcost;
    double Gcost;
    double Fcost;

    int isOnPath;
    int *adjacent; // SO,S,SE,O,E,NO,N,NE -> 8 au max // Tableau des INDICES des noeuds adjacents
    int isTurnNode;



} Node;

typedef struct Astar
{

    double intervalx;
    double intervaly;

    int *closedList; // Tableau des INDICES des noeuds
    int length_closed;

    int *openList; // Tableau des INDICES des noeuds
    int length_open;

    Node **map; // Tableau de noeuds
    int nbr_nodes;
    int isHardcoded;

    int *path; // Tableau des INDICES des noeuds

    int length_path;
    int count;

    int nTurn;
    int length_path_only_turns; // Without turn
    int* path_only_turns; // Without turn

    //char stringPath[200];


} Astar;

//NAMESPACE_CLOSE();

#endif // end of header guard
