//
//  Astar_gr1.cpp
//
//
//  Created by William Chermanne on 4/03/17.
//
//
#include "MyIncludes_Moonwalker.h"
#include "MyIncludes_gr1.h"



//NAMESPACE_INIT(ctrlGr1);

/**********************************
***********************************
***********************************
***********************************
Initialization
***********************************
***********************************
***********************************
**********************************/

/* This function initializes the Astar structure
*
* param[in] : Astar, index of starting node , index of target node
*/
void Astar_init(CtrlStruct *cvs) // Pour initialiser le Astar
{

  Astar *myAstar = (Astar*) calloc(1,sizeof(Astar));
  int lign_x=60;
  int lign_y=60;
  int nbr_nodes = lign_x*lign_y;

  double intervalx=0.0;
  double intervaly=0.0;

  //Create_map(myAstar,lign_x,lign_y);
  // printf(GRN "ASTAR INIT\n" RESET);
  Create_map_hardcoded(myAstar);

  // Define the obstacles after the map is created!


  myAstar->closedList =  (int*) calloc(nbr_nodes,sizeof(int));
  myAstar->openList = (int*) calloc(nbr_nodes,sizeof(int));
  myAstar->length_open=0;
  myAstar->length_closed=0;
  myAstar->length_path=1;
  myAstar->nTurn=0;
  myAstar->length_path_only_turns=0;
  //myAstar->stringPath=0;


  cvs->struct_control->counterNode=0;


  cvs->struct_path_planning->astar=myAstar; // Pour assigner myAstar, que je viens de créer, au Astar de ma structure path planning
}

/**********************************
***********************************
***********************************
***********************************
Map and obstacles
***********************************
***********************************
***********************************
**********************************/

/* This function creates the map of nodes
*
* param[in] : Astar, number of horizontal lines wanted , number of vertical lines wanted
*/

void Create_map(Astar *A, int lign_x,int lign_y)
{

  A->isHardcoded=0;
  int nbr_nodes = lign_y*lign_x;
  A->nbr_nodes = nbr_nodes;

  A->map = (Node**) malloc(nbr_nodes*sizeof(Node));

  double l_x_table=2022.0;
  double l_y_table=3022.0;
  int countx=0;
  int county=0;

  A->intervalx= l_x_table/(lign_y-1);
  A->intervaly= l_y_table/(lign_x-1);

  double intervalx=A->intervalx;
  double intervaly=A->intervaly;
  for (int i = 0; i < nbr_nodes; i++)
  {
    A->map[i] = (Node*) malloc(sizeof(Node));
    A->map[i]->adjacent = (int*) calloc(8,sizeof(int));

    /* Définir les attributs de chaque Node */

    A->map[i]->index= i;
    A->map[i]->Hcost= 0;
    A->map[i]->Gcost= 0;
    A->map[i]->Fcost= 0;
    A->map[i]->isInOpenList= 0;
    A->map[i]->isInClosedList= 0;
    A->map[i]->walkable= 1;
    A->map[i]->parent_index=-1;
    A->map[i]->isOnPath=0;
    A->map[i]->isTurnNode=0;


    // Coordonnées
    A->map[i]->x= (-l_x_table)/2+double(countx)*intervalx;
    A->map[i]->y= (-l_y_table)/2+double(county)*intervaly;

    countx++;

    if(county==lign_x-1 && countx==lign_y)
    {
      // printf("Grid done!\n");

    }
    if(countx==lign_y)
    {
      countx=0;
      county++;
    }


    /* Définir les adjacents */


    ////////////////// Coins //////////////////

    // Coin Sud Ouest
    if(i==0)
    {
      A->map[i]->adjacent[0]=1;
      A->map[i]->adjacent[1]=lign_y;
      A->map[i]->adjacent[2]=1+lign_y;
      A->map[i]->adjacent[3]=-1;
      A->map[i]->adjacent[4]=-1;
      A->map[i]->adjacent[5]=-1;
      A->map[i]->adjacent[6]=-1;
      A->map[i]->adjacent[7]=-1;




    }
    // Coin Sud Est

    else if(i==lign_y-1)
    {
      A->map[i]->adjacent[0]=lign_y-2;
      A->map[i]->adjacent[1]=i+lign_y-1;
      A->map[i]->adjacent[2]=i+lign_y;
      A->map[i]->adjacent[3]=-1;
      A->map[i]->adjacent[4]=-1;
      A->map[i]->adjacent[5]=-1;
      A->map[i]->adjacent[6]=-1;
      A->map[i]->adjacent[7]=-1;
    }

    // Coin Nord Ouest

    else if(i==(lign_y*lign_x)-lign_y)
    {
      A->map[i]->adjacent[0]=i-lign_y;
      A->map[i]->adjacent[1]=i-lign_y+1;
      A->map[i]->adjacent[2]=i+1;
      A->map[i]->adjacent[3]=-1;
      A->map[i]->adjacent[4]=-1;
      A->map[i]->adjacent[5]=-1;
      A->map[i]->adjacent[6]=-1;
      A->map[i]->adjacent[7]=-1;
    }

    // Coin Nord Est

    else if(i==(lign_y*lign_x)-1)
    {
      A->map[i]->adjacent[0]=i-lign_y-1;
      A->map[i]->adjacent[1]=i-lign_y;
      A->map[i]->adjacent[2]=i-1;
      A->map[i]->adjacent[3]=-1;
      A->map[i]->adjacent[4]=-1;
      A->map[i]->adjacent[5]=-1;
      A->map[i]->adjacent[6]=-1;
      A->map[i]->adjacent[7]=-1;
    }

    ////////////////// Cotés //////////////////

    // Coté Sud

    else if(i>0 && i<lign_y-1)
    {
      A->map[i]->adjacent[0]=i-1;
      A->map[i]->adjacent[1]=i+1;
      A->map[i]->adjacent[2]=i+lign_y-1;
      A->map[i]->adjacent[3]=i+lign_y;
      A->map[i]->adjacent[4]=i+lign_y+1;
      A->map[i]->adjacent[5]=-1;
      A->map[i]->adjacent[6]=-1;
      A->map[i]->adjacent[7]=-1;
    }

    // Coté Nord

    else if(i>(lign_y*lign_x)-lign_y && i<(lign_y*lign_x)-1)
    {
      A->map[i]->adjacent[0]=i-lign_y-1;
      A->map[i]->adjacent[1]=i-lign_y;
      A->map[i]->adjacent[2]=i-lign_y+1;
      A->map[i]->adjacent[3]=i-1;
      A->map[i]->adjacent[4]=i+1;
      A->map[i]->adjacent[5]=-1;
      A->map[i]->adjacent[6]=-1;
      A->map[i]->adjacent[7]=-1;
    }

    // Coté Ouest

    else if(i !=(0) && i !=(lign_x*lign_y)-lign_y && i % lign_y ==0)
    {
      A->map[i]->adjacent[0]=i-lign_y;
      A->map[i]->adjacent[1]=i-lign_y+1;
      A->map[i]->adjacent[2]=i+1;
      A->map[i]->adjacent[3]=i+lign_y;
      A->map[i]->adjacent[4]=i+lign_y+1;
      A->map[i]->adjacent[5]=-1;
      A->map[i]->adjacent[6]=-1;
      A->map[i]->adjacent[7]=-1;
    }

    // Coté Est

    else if(i !=((lign_x*lign_y)-1) && i !=(lign_y-1) && ((i % lign_y) == (lign_y-1)))
    {
      A->map[i]->adjacent[0]=i-lign_y-1;
      A->map[i]->adjacent[1]=i-lign_y;
      A->map[i]->adjacent[2]=i-1;
      A->map[i]->adjacent[3]=i+lign_y-1;
      A->map[i]->adjacent[4]=i+lign_y;
      A->map[i]->adjacent[5]=-1;
      A->map[i]->adjacent[6]=-1;
      A->map[i]->adjacent[7]=-1;
    }
    // Pour le cas classique //

    else
    {
      A->map[i]->adjacent[0]= i-lign_y-1;
      A->map[i]->adjacent[1]= i-lign_y;
      A->map[i]->adjacent[2]= (i+1)-lign_y;
      A->map[i]->adjacent[3]= i-1;
      A->map[i]->adjacent[4]= i+1;
      A->map[i]->adjacent[5]= (i -1)+lign_y;
      A->map[i]->adjacent[6]= i+lign_y;
      A->map[i]->adjacent[7]= (i+1)+lign_y;

    }

  }

}

void Create_map_hardcoded(Astar *A)
{
  A->isHardcoded=1;
  int nbr_nodes = NBR_NODES;
  A->nbr_nodes = nbr_nodes;
    printf("nbr_nodes:%d\n",nbr_nodes);
  A->map = (Node**) malloc(nbr_nodes*sizeof(Node));

  for (int i = 0; i < nbr_nodes; i++)
  {
      printf("In the for\n");
    A->map[i] = (Node*) malloc(sizeof(Node));
    A->map[i]->adjacent = (int*) calloc(NBR_ADJ,sizeof(int));

    /* Définir les attributs de chaque Node = initialisation des nodes */
    A->map[i]->index= i;
    printf("i:%d\n",i);
      printf("index:%d\n",A->map[i]->index);
    A->map[i]->Hcost= 0;
    A->map[i]->Gcost= 0;
    A->map[i]->Fcost= 0;
    A->map[i]->isInOpenList= 0;
    A->map[i]->isInClosedList= 0;
    A->map[i]->walkable= 1;
          printf("walkable:%d\n",A->map[i]->walkable);

    A->map[i]->parent_index=-1;
    A->map[i]->isOnPath=0;
    A->map[i]->isTurnNode=0;
    printf("index2:%d\n",A->map[i]->index);
  }

  // Définir les coordonnées de tous les noeuds
  A->map[0]->x=-850;
  A->map[0]->y=-1400;
  A->map[0]->adjacent[0]=5;
  A->map[0]->adjacent[1]=-1;
  A->map[0]->adjacent[2]=-1;
  A->map[0]->adjacent[3]=-1;
  A->map[0]->adjacent[4]=-1;
  A->map[0]->adjacent[5]=-1;
  A->map[0]->adjacent[6]=-1;
  A->map[0]->adjacent[7]=-1;
      //A->map[0]->index= 0;

    printf(GRN "Created node %d %f %f \n" RESET,A->map[0]->index,A->map[0]->x,A->map[0]->y);

  A->map[1]->x=-850;
  A->map[1]->y=-1150;
  A->map[1]->adjacent[0]=5;
  A->map[1]->adjacent[1]=-1;
  A->map[1]->adjacent[2]=-1;
  A->map[1]->adjacent[3]=-1;
  A->map[1]->adjacent[4]=-1;
  A->map[1]->adjacent[5]=-1;
  A->map[1]->adjacent[6]=-1;
  A->map[1]->adjacent[7]=-1;


  A->map[2]->x=-400;
  A->map[2]->y=-1250;
  A->map[2]->adjacent[0]=6;
  A->map[2]->adjacent[1]=7;
  A->map[2]->adjacent[2]=3;
  A->map[2]->adjacent[3]=37;
  A->map[2]->adjacent[4]=-1;
  A->map[2]->adjacent[5]=-1;
  A->map[2]->adjacent[6]=-1;
  A->map[2]->adjacent[7]=-1;



  A->map[3]->x=0;
  A->map[3]->y=-1150;
  A->map[3]->adjacent[0]=2;
  A->map[3]->adjacent[1]=7;
  A->map[3]->adjacent[2]=8;
  A->map[3]->adjacent[3]=4;
  A->map[3]->adjacent[4]=9;
  A->map[3]->adjacent[5]=37;
  A->map[3]->adjacent[6]=39;
  A->map[3]->adjacent[7]=-1;


  A->map[4]->x=800;
  A->map[4]->y=-1300;
  A->map[4]->adjacent[0]=3;
  A->map[4]->adjacent[1]=9;
  A->map[4]->adjacent[2]=8;
  A->map[4]->adjacent[3]=-1;
  A->map[4]->adjacent[4]=-1;
  A->map[4]->adjacent[5]=-1;
  A->map[4]->adjacent[6]=-1;
  A->map[4]->adjacent[7]=-1;

  A->map[5]->x=-800;
  A->map[5]->y=-600;
  A->map[5]->adjacent[0]=6;
  A->map[5]->adjacent[1]=15;
  A->map[5]->adjacent[2]=-1;
  A->map[5]->adjacent[3]=0;
  A->map[5]->adjacent[4]=1;
  A->map[5]->adjacent[5]=32;
  A->map[5]->adjacent[6]=33;
  A->map[5]->adjacent[7]=-1;

  A->map[6]->x=-400;
  A->map[6]->y=-550;
  A->map[6]->adjacent[0]=5;
  A->map[6]->adjacent[1]=12;
  A->map[6]->adjacent[2]=7;
  A->map[6]->adjacent[3]=15;
  A->map[6]->adjacent[4]=2;
  A->map[6]->adjacent[5]=32;
  A->map[6]->adjacent[6]=33;
  A->map[6]->adjacent[7]=-1;

  A->map[7]->x=-200;
  A->map[7]->y=-855;
  A->map[7]->adjacent[0]=2;
  A->map[7]->adjacent[1]=6;
  A->map[7]->adjacent[2]=3;
  A->map[7]->adjacent[3]=8;
  A->map[7]->adjacent[4]=11;
  A->map[7]->adjacent[5]=37;
  A->map[7]->adjacent[6]=-1;
  A->map[7]->adjacent[7]=-1;

  A->map[8]->x=133;
  A->map[8]->y=-855;
  A->map[8]->adjacent[0]=4;
  A->map[8]->adjacent[1]=9;
  A->map[8]->adjacent[2]=11;
  A->map[8]->adjacent[3]=7;
  A->map[8]->adjacent[4]=3;
  A->map[8]->adjacent[5]=30;
  A->map[8]->adjacent[6]=37;
  A->map[8]->adjacent[7]=39;

  A->map[9]->x=530;
  A->map[9]->y=-855;
  A->map[9]->adjacent[0]=8;
  A->map[9]->adjacent[1]=10;
  A->map[9]->adjacent[2]=3;
  A->map[9]->adjacent[3]=4;
  A->map[9]->adjacent[4]=30;
  A->map[9]->adjacent[5]=39;
  A->map[9]->adjacent[6]=-1;
  A->map[9]->adjacent[7]=-1;

  A->map[10]->x=800;
  A->map[10]->y=-550;
  A->map[10]->adjacent[0]=9;
  A->map[10]->adjacent[1]=4;
  A->map[10]->adjacent[2]=-1;
  A->map[10]->adjacent[3]=-1;
  A->map[10]->adjacent[4]=-1;
  A->map[10]->adjacent[5]=-1;
  A->map[10]->adjacent[6]=-1;
  A->map[10]->adjacent[7]=-1;

  A->map[11]->x=0;
  A->map[11]->y=-666;
  A->map[11]->adjacent[0]=13;
  A->map[11]->adjacent[1]=8;
  A->map[11]->adjacent[2]=12;
  A->map[11]->adjacent[3]=7;
  A->map[11]->adjacent[4]=6;
  A->map[11]->adjacent[5]=30;
  A->map[11]->adjacent[6]=37;
  A->map[11]->adjacent[7]=-1;

  A->map[12]->x=-200;
  A->map[12]->y=-465;
  A->map[12]->adjacent[0]=6;
  A->map[12]->adjacent[1]=13;
  A->map[12]->adjacent[2]=11;
  A->map[12]->adjacent[3]=15;
  A->map[12]->adjacent[4]=33;
  A->map[12]->adjacent[5]=-1;
  A->map[12]->adjacent[6]=-1;
  A->map[12]->adjacent[7]=-1;

  A->map[13]->x=0;
  A->map[13]->y=-266;
  A->map[13]->adjacent[0]=12;
  A->map[13]->adjacent[1]=15;
  A->map[13]->adjacent[2]=14;
  A->map[13]->adjacent[3]=11;
  A->map[13]->adjacent[4]=30;
  A->map[13]->adjacent[5]=33;
  A->map[13]->adjacent[6]=-1;
  A->map[13]->adjacent[7]=-1;

  A->map[14]->x=0;
  A->map[14]->y=0;
  A->map[14]->adjacent[0]=5;
  A->map[14]->adjacent[1]=15;
  A->map[14]->adjacent[2]=-1;
  A->map[14]->adjacent[3]=16;
  A->map[14]->adjacent[4]=13;
  A->map[14]->adjacent[5]=33;
  A->map[14]->adjacent[6]=35;
  A->map[14]->adjacent[7]=-1;


  A->map[15]->x=-470;
  A->map[15]->y=0;
  A->map[15]->adjacent[0]=5;
  A->map[15]->adjacent[1]=17;
  A->map[15]->adjacent[2]=18;
  A->map[15]->adjacent[3]=12;
  A->map[15]->adjacent[4]=14;
  A->map[15]->adjacent[5]=32;
  A->map[15]->adjacent[6]=33;
  A->map[15]->adjacent[7]=34;

  A->map[16]->x=0;
  A->map[16]->y=266;
  A->map[16]->adjacent[0]=15;
  A->map[16]->adjacent[1]=18;
  A->map[16]->adjacent[2]=14;
  A->map[16]->adjacent[3]=20;
  A->map[16]->adjacent[4]=31;
  A->map[16]->adjacent[5]=35;
  A->map[16]->adjacent[6]=-1;
  A->map[16]->adjacent[7]=-1;

  A->map[17]->x=-800;
  A->map[17]->y=600;
  A->map[17]->adjacent[0]=25;
  A->map[17]->adjacent[1]=26;
  A->map[17]->adjacent[2]=19;
  A->map[17]->adjacent[3]=15;
  A->map[17]->adjacent[4]=-1;
  A->map[17]->adjacent[5]=34;
  A->map[17]->adjacent[6]=35;
  A->map[17]->adjacent[7]=-1;

  A->map[18]->x=-200;
  A->map[18]->y=465;
  A->map[18]->adjacent[0]=15;
  A->map[18]->adjacent[1]=31;
  A->map[18]->adjacent[2]=16;
  A->map[18]->adjacent[3]=20;
  A->map[18]->adjacent[4]=19;
  A->map[18]->adjacent[5]=35;
  A->map[18]->adjacent[6]=-1;
  A->map[18]->adjacent[7]=-1;

  A->map[19]->x=-400;
  A->map[19]->y=620;
  A->map[19]->adjacent[0]=17;
  A->map[19]->adjacent[1]=18;
  A->map[19]->adjacent[2]=27;
  A->map[19]->adjacent[3]=22;
  A->map[19]->adjacent[4]=15;
  A->map[19]->adjacent[5]=34;
  A->map[19]->adjacent[6]=35;
  A->map[19]->adjacent[7]=-1;

  A->map[20]->x=0;
  A->map[20]->y=666;
  A->map[20]->adjacent[0]=18;
  A->map[20]->adjacent[1]=19;
  A->map[20]->adjacent[2]=22;
  A->map[20]->adjacent[3]=23;
  A->map[20]->adjacent[4]=16;
  A->map[20]->adjacent[5]=31;
  A->map[20]->adjacent[6]=36;
  A->map[20]->adjacent[7]=-1;

  A->map[21]->x=800;
  A->map[21]->y=550;
  A->map[21]->adjacent[0]=24;
  A->map[21]->adjacent[1]=29;
  A->map[21]->adjacent[2]=-1;
  A->map[21]->adjacent[3]=-1;
  A->map[21]->adjacent[4]=-1;
  A->map[21]->adjacent[5]=-1;
  A->map[21]->adjacent[6]=-1;
  A->map[21]->adjacent[7]=-1;

  A->map[22]->x=-200;
  A->map[22]->y=855;
  A->map[22]->adjacent[0]=19;
  A->map[22]->adjacent[1]=20;
  A->map[22]->adjacent[2]=27;
  A->map[22]->adjacent[3]=28;
  A->map[22]->adjacent[4]=23;
  A->map[22]->adjacent[5]=36;
  A->map[22]->adjacent[6]=-1;
  A->map[22]->adjacent[7]=-1;

  A->map[23]->x=133;
  A->map[23]->y=855;
  A->map[23]->adjacent[0]=20;
  A->map[23]->adjacent[1]=29;
  A->map[23]->adjacent[2]=24;
  A->map[23]->adjacent[3]=28;
  A->map[23]->adjacent[4]=22;
  A->map[23]->adjacent[5]=31;
  A->map[23]->adjacent[6]=36;
  A->map[23]->adjacent[7]=38;

  A->map[24]->x=530;
  A->map[24]->y=855;
  A->map[24]->adjacent[0]=21;
  A->map[24]->adjacent[1]=29;
  A->map[24]->adjacent[2]=28;
  A->map[24]->adjacent[3]=23;
  A->map[24]->adjacent[4]=31;
  A->map[24]->adjacent[5]=38;
  A->map[24]->adjacent[6]=-1;
  A->map[24]->adjacent[7]=-1;

  A->map[25]->x=-850;
  A->map[25]->y=1150;
  A->map[25]->adjacent[0]=17;
  A->map[25]->adjacent[1]=-1;
  A->map[25]->adjacent[2]=-1;
  A->map[25]->adjacent[3]=-1;
  A->map[25]->adjacent[4]=-1;
  A->map[25]->adjacent[5]=-1;
  A->map[25]->adjacent[6]=-1;
  A->map[25]->adjacent[7]=-1;

  A->map[26]->x=-850;
  A->map[26]->y=1400;
  A->map[26]->adjacent[0]=17;
  A->map[26]->adjacent[1]=-1;
  A->map[26]->adjacent[2]=-1;
  A->map[26]->adjacent[3]=-1;
  A->map[26]->adjacent[4]=-1;
  A->map[26]->adjacent[5]=-1;
  A->map[26]->adjacent[6]=-1;
  A->map[26]->adjacent[7]=-1;

  A->map[27]->x=-400;
  A->map[27]->y=1250;
  A->map[27]->adjacent[0]=19;
  A->map[27]->adjacent[1]=22;
  A->map[27]->adjacent[2]=28;
  A->map[27]->adjacent[3]=36;
  A->map[27]->adjacent[4]=-1;
  A->map[27]->adjacent[5]=-1;
  A->map[27]->adjacent[6]=-1;
  A->map[27]->adjacent[7]=-1;

  A->map[28]->x=0;
  A->map[28]->y=1150;
  A->map[28]->adjacent[0]=22;
  A->map[28]->adjacent[1]=23;
  A->map[28]->adjacent[2]=24;
  A->map[28]->adjacent[3]=27;
  A->map[28]->adjacent[4]=29;
  A->map[28]->adjacent[5]=36;
  A->map[28]->adjacent[6]=38;
  A->map[28]->adjacent[7]=-1;

  A->map[29]->x=800;
  A->map[29]->y=1300;
  A->map[29]->adjacent[0]=28;
  A->map[29]->adjacent[1]=23;
  A->map[29]->adjacent[2]=21;
  A->map[29]->adjacent[3]=-1;
  A->map[29]->adjacent[4]=-1;
  A->map[29]->adjacent[5]=-1;
  A->map[29]->adjacent[6]=-1;
  A->map[29]->adjacent[7]=-1;

  A->map[30]->x=160;
  A->map[30]->y=-600;
  A->map[30]->adjacent[0]=8;
  A->map[30]->adjacent[1]=9;
  A->map[30]->adjacent[2]=13;
  A->map[30]->adjacent[3]=11;
  A->map[30]->adjacent[4]=-1;
  A->map[30]->adjacent[5]=-1;
  A->map[30]->adjacent[6]=-1;
  A->map[30]->adjacent[7]=-1;

  A->map[31]->x=160;
  A->map[31]->y=600;
  A->map[31]->adjacent[0]=16;
  A->map[31]->adjacent[1]=18;
  A->map[31]->adjacent[2]=23;
  A->map[31]->adjacent[3]=20;
  A->map[31]->adjacent[4]=24;
  A->map[31]->adjacent[5]=-1;
  A->map[31]->adjacent[6]=-1;
  A->map[31]->adjacent[7]=-1;

  A->map[32]->x=-750;
  A->map[32]->y=-200;
  A->map[32]->adjacent[0]=34;
  A->map[32]->adjacent[1]=15;
  A->map[32]->adjacent[2]=33;
  A->map[32]->adjacent[3]=5;
  A->map[32]->adjacent[4]=6;
  A->map[32]->adjacent[5]=-1;
  A->map[32]->adjacent[6]=-1;
  A->map[32]->adjacent[7]=-1;

  A->map[33]->x=-530;
  A->map[33]->y=-300;
  A->map[33]->adjacent[0]=32;
  A->map[33]->adjacent[1]=5;
  A->map[33]->adjacent[2]=15;
  A->map[33]->adjacent[3]=14;
  A->map[33]->adjacent[4]=13;
  A->map[33]->adjacent[5]=12;
  A->map[33]->adjacent[6]=6;
  A->map[33]->adjacent[7]=-1;

  A->map[34]->x=-750;
  A->map[34]->y=200;
  A->map[34]->adjacent[0]=32;
  A->map[34]->adjacent[1]=15;
  A->map[34]->adjacent[2]=19;
  A->map[34]->adjacent[3]=17;
  A->map[34]->adjacent[4]=35;
  A->map[34]->adjacent[5]=-1;
  A->map[34]->adjacent[6]=-1;
  A->map[34]->adjacent[7]=-1;

  A->map[35]->x=180;
  A->map[35]->y=500;
  A->map[35]->adjacent[0]=34;
  A->map[35]->adjacent[1]=18;
  A->map[35]->adjacent[2]=19;
  A->map[35]->adjacent[3]=17;
  A->map[35]->adjacent[4]=16;
  A->map[35]->adjacent[5]=14;
  A->map[35]->adjacent[6]=15;
  A->map[35]->adjacent[7]=-1;

  A->map[36]->x=0;
  A->map[36]->y=900;
  A->map[36]->adjacent[0]=27;
  A->map[36]->adjacent[1]=28;
  A->map[36]->adjacent[2]=-1;
  A->map[36]->adjacent[3]=22;
  A->map[36]->adjacent[4]=23;
  A->map[36]->adjacent[5]=-1;
  A->map[36]->adjacent[6]=20;
  A->map[36]->adjacent[7]=-1;

  A->map[37]->x=0;
  A->map[37]->y=-900;
  A->map[37]->adjacent[0]=3;
  A->map[37]->adjacent[1]=7;
  A->map[37]->adjacent[2]=8;
  A->map[37]->adjacent[3]=11;
  A->map[37]->adjacent[4]=2;
  A->map[37]->adjacent[5]=-1;
  A->map[37]->adjacent[6]=-1;
  A->map[37]->adjacent[7]=-1;

  A->map[37]->x=0;
  A->map[37]->y=-900;
  A->map[37]->adjacent[0]=3;
  A->map[37]->adjacent[1]=7;
  A->map[37]->adjacent[2]=8;
  A->map[37]->adjacent[3]=11;
  A->map[37]->adjacent[4]=2;
  A->map[37]->adjacent[5]=-1;
  A->map[37]->adjacent[6]=-1;
  A->map[37]->adjacent[7]=-1;

  A->map[38]->x=530;
  A->map[38]->y=1100;
  A->map[38]->adjacent[0]=23;
  A->map[38]->adjacent[1]=24;
  A->map[38]->adjacent[2]=28;
  A->map[38]->adjacent[3]=-1;
  A->map[38]->adjacent[4]=-1;
  A->map[38]->adjacent[5]=-1;
  A->map[38]->adjacent[6]=-1;
  A->map[38]->adjacent[7]=-1;

  A->map[39]->x=530;
  A->map[39]->y=-1100;
  A->map[39]->adjacent[0]=3;
  A->map[39]->adjacent[1]=9;
  A->map[39]->adjacent[2]=8;
  A->map[39]->adjacent[3]=-1;
  A->map[39]->adjacent[4]=-1;
  A->map[39]->adjacent[5]=-1;
  A->map[39]->adjacent[6]=-1;
  A->map[39]->adjacent[7]=-1;

  A->map[40]->x=-800;
  A->map[40]->y=350;
  A->map[40]->adjacent[0]=17;
  A->map[40]->adjacent[1]=15;
  A->map[40]->adjacent[2]=14;
  A->map[40]->adjacent[3]=34;
  A->map[40]->adjacent[4]=35;
  A->map[40]->adjacent[5]=-1;
  A->map[40]->adjacent[6]=-1;
  A->map[40]->adjacent[7]=-1;

  A->map[41]->x=-800;
  A->map[41]->y=-350;
  A->map[41]->adjacent[0]=32;
  A->map[41]->adjacent[1]=15;
  A->map[41]->adjacent[2]=33;
  A->map[41]->adjacent[3]=5;
  A->map[41]->adjacent[4]=-1;
  A->map[41]->adjacent[5]=-1;
  A->map[41]->adjacent[6]=-1;
  A->map[41]->adjacent[7]=-1;

for (int i = 0; i < nbr_nodes; i++)
  {
    A->map[i]->index= i;
  }
}


/**********************************
***********************************
***********************************
***********************************
Trajectory(=A*) and paths
***********************************
***********************************
***********************************
**********************************/

/* This function creates the trajectory of the nodes (=A* algorithm!) : sets the parents of the nodes so that the path can be retrieved
*
* param[in] : Astar, index of starting node , index of target node
*/
void Create_path(Astar *A, int indexStartNode, int indexTargetNode)
{

       printf("In create path\n");

  Node *StartNode=A->map[indexStartNode];

  double xNodeStart=StartNode->x;
  double yNodeStart=StartNode->y;
  printf("xNodeStart in create path:%d\n",xNodeStart);
  printf("yNodeStart  in create path:%d\n",yNodeStart);

  double xTargetNode=A->map[indexTargetNode]->x;
  double yTargetNode=A->map[indexTargetNode]->y;

  Node *adjacentNode;
  int adjacent_index;
  double xadj;
  double yadj;


  Node *currentNode;
  int current_index;
  double xcurrent;
  double ycurrent;

  double dir1,dir2;
  double GcostRotation=0;
  //double GcostRotation=0;

  bool found=0; // If the last node is reached, found=0

  add_open_list(A,StartNode);


  while(A->length_open>=0 && found ==0)
  {
       printf("In create path\n");

    current_index = lowest_cost_open_list(A);

    currentNode=A->map[current_index];
    if (A->length_open==0)
    {
       printf("OPEN LIST EMPTY! NO PATH FOUND\n");
      break;
    }

    xcurrent=currentNode->x;
    ycurrent=currentNode->y;


    current_to_closed_list(A,currentNode);




    // Parcourir les adjacents de current
    for (int i = 0; i < NBR_ADJ; i++)
    {
               printf("In The for\n");

              adjacent_index=A->map[current_index]->adjacent[i];

    if (adjacent_index!=-1)
      {

      adjacentNode= A->map[adjacent_index];
      xadj=adjacentNode->x;

      yadj=adjacentNode->y;


        if (current_index==indexTargetNode)
        {
          // On est arrivés au noeud target
           printf(RED "Path computed!: Arrived at node %d\n" RESET,current_index);
           printf(BLU "Path computed!: Arrived at node %d\n" RESET,current_index);
           printf(GRN "Path computed!: Arrived at node %d\n" RESET,current_index);
           printf("Path computed!: Arrived at node %d\n",current_index);

          found=1;
          break;
        }
        else if(adjacentNode->isInClosedList==1)
        {
        }
        else if(adjacentNode->walkable==0)
        {

        }
        else if(adjacentNode->isInOpenList==0) // S'il est pas dans l'open list
        {
          add_open_list(A,A->map[adjacent_index]);
          adjacentNode->parent_index=current_index; // Mettre le parent;


          ////////////////// ADDED FOR ROTATION ////////////////////

          if(current_index !=indexStartNode)
          {

            double slope1=slopeBetweenNodes(A,currentNode->parent_index,current_index);
            double slope2=slopeBetweenNodes(A,current_index,adjacent_index);
            slope1=int(slope1);
            slope2=int(slope2);


            if(slope1==slope2)
            {

              GcostRotation=0.0;
            }
            else
            {

              GcostRotation=100.0;
            }

          }

          adjacentNode->Hcost=EuclidianDistance(xadj,yadj,xTargetNode,yTargetNode);
          adjacentNode->Gcost=currentNode->Gcost+EuclidianDistance(A->map[adjacent_index]->x,A->map[adjacent_index]->y,xcurrent,ycurrent)+GcostRotation; // Gcost = distance entre adj et current
          adjacentNode->Fcost=A->map[adjacent_index]->Gcost+A->map[adjacent_index]->Hcost; // F=G+H


        }

        else if(adjacentNode->isInOpenList==1)
        {
          double slope1=slopeBetweenNodes(A,currentNode->parent_index,current_index);
          double slope2=slopeBetweenNodes(A,current_index,adjacent_index);

          if(slope1==slope2)
          {
            GcostRotation=0.0;
          }
          else
          {
            GcostRotation=100.0;
          }

          double GcostNow= currentNode->Gcost+EuclidianDistance(xadj,yadj,xcurrent,ycurrent)+GcostRotation;

          if(GcostNow< adjacentNode->Gcost)
          {

            adjacentNode->parent_index= current_index; // Mettre à jour le parent;
            adjacentNode->Gcost= GcostNow; // Mettre à jour le parent;

            adjacentNode->Fcost=GcostNow+ adjacentNode->Hcost; // F=G+H
          }
        }
      }
    }
  }
  // Computes the length of the path by starting from the target node and going from parent to parent
  ComputeLengthPath(A,indexTargetNode); // Vient mettre length path dans le path de la structure
  A->path= (int*)calloc(A->length_path,sizeof(int)); // Malloc le path de la structure
  RetrieveReversedPath(A,indexTargetNode,A->path);

  printArray(A->path,A->length_path);
  int *temp;
  temp= (int*)calloc(A->length_path,sizeof(int));
  revertArray(A->path,A->length_path,temp);
  A->path=temp;
  printArray(A->path,A->length_path);
    printf("\n \n");

}

void RefreshPath(Astar *A)
{
  A->path= (int*) malloc(A->length_path*sizeof(int)); // Malloc le path de la structure
}

/* This function computes the length of the path generated
*
* param[in] : Astar, index of target Node(to know where to start looking for the parents)
*/
void ComputeLengthPath(Astar *A,int TargetNode) // Calcule le nombre de noeuds, y compris le target
{


  int myParent = A->map[TargetNode]->parent_index;
  if(myParent>-1) // Pcq le noeud start est le seul qui a un parent =-1
  {
    A->length_path++;
    ComputeLengthPath(A,myParent);
  }
}

/* This function returns a table containing the index of the nodes in the path from the target node to the start node, including target node,
* It also sets the isOnPath property of the node to 1
* param[in] : Astar, index of target Node(to know where to start looking for the parents)
*/
// Renvoit le tableau des noeuds traversés, y compris targetnode (mais pas startnode)
void RetrieveReversedPath(Astar *A,int TargetNode, int* path)
{
  int current = A->map[TargetNode]->parent_index;
  A->map[TargetNode]->isOnPath=1;
  path[0]=TargetNode;
  for (int i = 1; i < A->length_path; i++)
  {
    path[i]= current;
    A->map[current]->isOnPath=1;

    current = A->map[current]->parent_index;
  }

}


/* This function returns the index of the node having the lowest cost in the open list
*
* param[in] : Astar and target node
* param[out] : Array with indexes of the nodes in the path
*/
void RetrievePath(Astar *A,int TargetNode,int *path)
{

  RetrieveReversedPath(A,TargetNode,path);


  int *temp;
  temp= (int*)malloc(A->length_path*sizeof(int));
  revertArray(path,A->length_path,temp); // Temp est le reverted array
  path=temp;
  free(temp);

}


void SelectNodesPath(Astar *A,int *path)
{

  int length=A->length_path;

  Node *parentNode;
  Node *currentNode;
  Node *nextNode;

  // Pour la première computation
  parentNode=A->map[path[0]];
  parentNode->isTurnNode=1; // Le premier noeud du chemin est considéré comme un tournant
  currentNode=A->map[path[1]];
  nextNode=A->map[path[2]];

  int countidem=0;


  for (int i = 2; i < length-1; i++) // Je parcours le path // -1 car on va chercher nextNode
  {
      // printf("Entering the for \n");

      // printf("Current Node index : %d \n ", currentNode->index); // print

      double slope1=slopeBetweenNodes(A,parentNode->index,currentNode->index); // Slope between parent and current
      double slope2=slopeBetweenNodes(A,currentNode->index,nextNode->index);
      slope1=int(slope1);
      slope2=int(slope2);

      if(slope1==slope2) // do not actualise turnNode, and go to the next nextNode
      {
        countidem++;
        // printf(BLU "Count idem: %d\n" RESET,countidem);
        if (countidem==-1)
        {
          // printf(RED "Node %d is a turn node!\n" RESET,currentNode->index);
          // printf(RED"Coordinates : x=%f, y=%f\n" RESET,currentNode->x,currentNode->y);
          currentNode->isTurnNode=1;
          A->nTurn++;
          countidem=0;
        }
      }
      else // Actualise turnNode and go to the next nextNode
      {
        // printf(RED "Node %d is a turn node!\n" RESET,currentNode->index);
        // printf(RED"Coordinates : x=%f, y=%f\n" RESET,currentNode->x,currentNode->y);
        currentNode->isTurnNode=1;
        A->nTurn++;
        countidem=0;
      }
      parentNode=A->map[path[i-1]];
      currentNode=A->map[path[i]];
      nextNode=A->map[path[i+1]]; // Switch to the next node
  }

    A->map[path[length-1]]->isTurnNode=1;;
    // printf(RED "Final Node %d is also a turn node!\n" RESET,path[length-1]);
    // printf(GRN "Number of turns : %d\n" RESET,A->nTurn);
    A->length_path_only_turns=A->nTurn+2; // Because the final path is also composed of the last node and the first node
    // printf(GRN "length_path_only_turn : %d\n" RESET,A->length_path_only_turns);

}

void CreatePathOnlyTurns(Astar *A)
{
  // printf(CYN "Creating Path Only Turns \n" RESET);
  A->path_only_turns= (int*)malloc(A->length_path_only_turns*sizeof(int)); // Malloc le path de la structure
  int counterTurnPath=0;
  for (int i = 0; i < A->length_path; i++)
  {
    if((A->map[(A->path[i])]->isTurnNode))
    {
      // printf("%d! It is a turn node! It is a turn node ! \n",(A->map[(A->path[i])]->index));
      A->path_only_turns[counterTurnPath]=A->path[i];
      counterTurnPath++;
    }
  }
  // printf(CYN "Creating Path Only Turns FINISHED\n" RESET);
  // printf(CYN "Path Only Turns:\n" RESET);
  printArray(A->path_only_turns,A->length_path_only_turns);
}
/**********************************
***********************************
***********************************
***********************************
Operations on lists
***********************************
***********************************
***********************************
**********************************/


/* This function adds a node to the open list
*
* param[in] : Astar, Node N
*/
void add_open_list(Astar *A,Node *N)
{
  N->isInOpenList = 1;
  A->openList[A->length_open]=N->index;
  A->length_open ++;
}

/* This function removes a node from the open list and adds it to the closed list
*
* param[in] : Astar, Node N
*/
void current_to_closed_list(Astar *A,Node *N)
{
  N->isInOpenList = 0;
  N->isInClosedList = 1;
  A->closedList[A->length_closed]=N->index;

  remove_element(A->openList, A->length_open, N->index);
  A->length_open--;
  A->length_closed++;
}

/* This function returns the index of the node having the lowest cost in the open list
*
* param[in] : Astar
* param[out] : index of lowest cost node
*/
int lowest_cost_open_list(Astar *A)
{
  if (A->length_open==0)
  {
    // printf("Error : No element in the open list!\n");
  }

  int lowest_cost_node = A->openList[0]; // On prend bien l'indice du noeud
  for (int i = 1; i < A->length_open; i++)
  {
    if((A->map[A->openList[i]]->Fcost) < (A->map[lowest_cost_node]->Fcost))
    {
      lowest_cost_node= A->openList[i];
    }
  }
  return lowest_cost_node;
}


/* This function finds the nearest node of the input point
*
* param[in] : Astar, coordinates of the point
* param[out] : Astar, index of the nearest node
*/
int FindNearestNode(Astar *A,double xTarget,double yTarget)
{

  int index_nearest_node;
  double xP,yP;
  double distance_center_point=5000;// Worst case
  double actual_distance_center_point;
  for (int i=0;i<NBR_NODES;i++)
  {

    xP = A->map[i]->x;
    yP = A->map[i]->y;
    printf("Considering node : %d xP:%f Yp:%f\n",A->map[i]->index,xP,yP);

    actual_distance_center_point=EuclidianDistance(xP,yP,xTarget,yTarget);
    if(actual_distance_center_point<distance_center_point && A->map[i]->walkable==1 )
    {
      index_nearest_node=A->map[i]->index;
      distance_center_point=actual_distance_center_point;
        printf("Nearest node found at : %d\n",index_nearest_node);

    }

  }
  printf("Index Nearest node: %d\n",index_nearest_node);
  return index_nearest_node;
}

double slopeBetweenNodes(Astar *A,int indexNode1, int indexNode2)
{
  double slope;
  double x1=A->map[indexNode1]->x;
  double y1=A->map[indexNode1]->y;

  double x2=A->map[indexNode2]->x;
  double y2=A->map[indexNode2]->y;

  if(x2==x1)
  {
    slope =10000.;
  }
  else if(y2==y1)
  {
    slope =0.;
  }
  else
  {
    slope = (y2-y1)/(x2 - x1);

  }
  return slope;
}
/**********************************
***********************************
***********************************
***********************************
FREE
***********************************
***********************************
***********************************
**********************************/

/* This function frees the Astar structure
*
* param[in] : Astar, Node N
*/

void Astar_free(Astar *A)
{
  // Rajouter les free nécessaires!
  free(A->closedList);
  free(A->openList);
  free(A->path);
  //free(A->adjacent);
  for (int i = 0; i < A->nbr_nodes; i++)
  {
    free(A->map[i]);
  }
  free(A->map);
  free(A);
}





//NAMESPACE_CLOSE();
