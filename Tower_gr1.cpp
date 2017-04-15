//
//  Tower_gr1.cpp
//
//
//  Created by William Chermanne on 20/02/17.
//
//


/*!
 * \file Tower_gr1.cc
 * \brief File to use the tower
 */

 #include "MyIncludes_gr1.h"
 #include "MyIncludes_Moonwalker.h"


//NAMESPACE_INIT(ctrlGr1);


void StructTower_init(CtrlStruct *cvs)
{
  //////////////////////   Structure Tower //////////////////////

  for (int i = 0; i < 5; i++)
  {
    cvs->struct_tower->tabDistance[i]=0;
  }
  for (int i = 0; i < 5; i++)
  {
    cvs->struct_tower->tabAngleDiff[i]=0;
  }
  for (int i = 0; i < 5; i++)
  {
    cvs->struct_tower->tabAngle[i]=0;
  }
  for (int i = 0; i < 3; i++)
  {
    cvs->struct_tower->tabFixed[i]=0;
  }

  cvs->struct_tower->counter=0;
  cvs->struct_tower->previous_rising_index=0;
  cvs->struct_tower->counterDist=0;
  cvs->struct_tower->dist=0;
  cvs->struct_tower->previousDistance=1000;
  cvs->struct_tower->previousMeanDistance = 200;
  cvs->struct_tower->previousMeanAngle = 1000;
  cvs->struct_tower->AngleDiff = 1000;
  cvs->struct_tower->counterAngle=0;
  cvs->struct_tower->previousAngle=1000;
  cvs->struct_tower->x_opponent=0;
  cvs->struct_tower->y_opponent=0;
  cvs->struct_tower->previousYr = 1000;
  cvs->struct_tower->previousXr =1000;
  cvs->struct_tower->currentYr = 1000;
  cvs->struct_tower->currentXr =1000;
}



/* This function returns the angle between our robot and the opponent robot
 *
 * param[in] : cvs controller main structure
 *
 * return[out] : angle opponent
 */
double angle_opponent(CtrlStruct *cvs)
{



    ///////////// Classical /////////////////

    CtrlIn *ivs;
    ivs = cvs->inputs;

    ///////////// Call variables /////////////////

    // General
    double tower_pos= ivs->tower_pos; //rempli dans un thread
    double moyenne_temp, moyenne_temp_diff;
    int counterAngle = cvs->struct_tower->counterAngle;
    double *tabAngle = cvs->struct_tower->tabAngle;
    double *tabAngleDiff = cvs->struct_tower->tabAngleDiff;
/*
    // For fixed beacons
    double *last_rising_fixed = ivs->last_rising_fixed;
    double *last_falling_fixed = ivs->last_falling_fixed;
    int rising_index_fixed = ivs->rising_index_fixed;
    int falling_index_fixed = ivs->falling_index_fixed;
    int nb_rising_fixed=ivs->nb_rising_fixed;
*/
    // Opponent beacons
    double *last_rising = ivs->last_rising;
    double *last_falling = ivs->last_falling;

    double diff_angle;
    double angle_opp;

    ///////////// Calculation of the angles /////////////////

    // Angles
    double rising_edge_angle = (last_rising[9]); // thread
    double falling_edge_angle = (last_falling[9]);// thread

    if((rising_edge_angle>0 && falling_edge_angle<0))
    {
      angle_opp=-PI;
      diff_angle = (rising_edge_angle - (falling_edge_angle + 2*PI))/2;
    }

    else
    {
      angle_opp = (rising_edge_angle+ falling_edge_angle)/2;
      diff_angle = (rising_edge_angle - falling_edge_angle)/2;

    }
    // printf(CYN "angle_opp:%f \n",angle_opp);

    tabAngle[counterAngle]=angle_opp;
    tabAngleDiff[counterAngle] = diff_angle;

    if(cvs->struct_tower->counterAngle==1)
    {
      cvs->struct_tower->counterAngle = 0;
        moyenne_temp = (tabAngle[0]+tabAngle[1])/2; //+tabAngle[2]+tabAngle[3])/4;//+tabAngle[4])/5;//+tabDistance[5]+tabDistance[6]+tabDistance[7]+tabDistance[8]+tabDistance[9])/10;
        moyenne_temp_diff = (tabAngleDiff[0]+tabAngleDiff[1])/2;//+tabAngleDiff[2]+tabAngleDiff[3])/4;//+tabAngleDiff[4])/5;
      cvs->struct_tower->previousMeanAngle = moyenne_temp; //moyenne_temp; // angle_opp;
      cvs->struct_tower->AngleDiff = diff_angle;
      // printf("ANGLE DIFF : %f\n", moyenne_temp_diff);
    }
    else if( cvs->struct_tower->previousAngle != angle_opp)
    {
      cvs->struct_tower->counterAngle++;
      cvs->struct_tower->previousAngle=angle_opp;
        cvs->struct_tower->AngleDiff = diff_angle;
      moyenne_temp = 1000;
    }
    else
    {
      moyenne_temp = 1000;
    }
    return moyenne_temp;

    //return PI;

}

/* This function returns the distance between our robot and the opponent's robot
 *
 * param[in] : cvs controller main structure
 *
 * return[out] : distance between our robot and the opponent's robot (temporal mean of the last 10 distance calculated)
 * IN CENTIMETERS
 */

double distance_opponent(CtrlStruct *cvs)
{

    ///////////// Classical /////////////////

    CtrlIn *ivs;
    ivs = cvs->inputs;

    ///////////// Call variables /////////////////


    // From inputs

    double *last_rising = ivs->last_rising;
    double *last_falling = ivs->last_falling;
   /* int rising_index = ivs->rising_index;
    int falling_index = ivs->falling_index;*/
    double rising_edge_angle = (last_rising[9]);//thread
    double falling_edge_angle = (last_falling[9]);//thread
    double moyenne_temp;
    // From StructTower

    int counterDist = cvs->struct_tower->counterDist;
    double *tabDistance = cvs->struct_tower->tabDistance;
    double previousDistance = cvs->struct_tower->previousDistance;

    // Data

    int r_beacon=BEACON_RADIUS;
    int d_beacon=r_beacon*2;

    ///////////// Calculations /////////////////
    if((rising_edge_angle>0 && falling_edge_angle<0))
    {
      falling_edge_angle+=2*PI;
    }
    //double arg1=sin((rising_edge_angle-falling_edge_angle)/2);
    double arg1=sin(cvs->struct_tower->AngleDiff);
    double arg2= r_beacon/arg1;
    double distance=arg2;

    if(distance<0) // Just to make it positive (function abs generates a warning and it irritates me :-) )
    {
        distance = - distance;
    }

  // Filter for outliers
    if(distance > sqrt(300*300 + 200*200))
    {
      distance = cvs->struct_tower->previousDistance;
    }

    tabDistance[counterDist]=distance;
    cvs->struct_tower->dist=distance;
    if(cvs->struct_tower->counterDist==1)
    {
      cvs->struct_tower->counterDist = 0;
        moyenne_temp = (tabDistance[0]+tabDistance[1])/2; //+tabDistance[2]+tabDistance[3]+tabDistance[4])/5;//+tabDistance[5]+tabDistance[6]+tabDistance[7]+tabDistance[8]+tabDistance[9])/10;
      if(moyenne_temp - cvs->struct_tower->previousMeanDistance >75)
      {
        moyenne_temp = cvs->struct_tower->previousMeanDistance;
      }
      if(moyenne_temp - cvs->struct_tower->previousMeanDistance <-75)
      {
        moyenne_temp = cvs->struct_tower->previousMeanDistance;
      }
      if(moyenne_temp - cvs->struct_tower->previousMeanDistance >10)
      {
        if(cvs->struct_tower->previousMeanDistance >140)
        {
          moyenne_temp = cvs->struct_tower->previousMeanDistance +5;
        }
        else
        {
            moyenne_temp = cvs->struct_tower->previousMeanDistance +10;
        }
      }
      if(moyenne_temp - cvs->struct_tower->previousMeanDistance < -10)
      {
        if(cvs->struct_tower->previousMeanDistance >140)
        {
          moyenne_temp = cvs->struct_tower->previousMeanDistance -5;
        }
        else
        {
            moyenne_temp = cvs->struct_tower->previousMeanDistance -10;
        }
      }
      cvs->struct_tower->previousMeanDistance = moyenne_temp;
    }
    else if( cvs->struct_tower->previousDistance != distance)
    {
      cvs->struct_tower->counterDist++;
      cvs->struct_tower->previousDistance=distance;
      moyenne_temp = 1000;
    }
    else
    {
      moyenne_temp = 1000;
    }
    return moyenne_temp;

    //return 10000;
}


/* This function returns the angles of the fixed beacons (in radians!)
 *
 * param[in] : cvs controller main structure
 *
 * return[out] : Tabular three angles of the fixed beacons.
 *                  Entry 0 = beacon located at (-1062,+1562)
 *                  Entry 1 = beacon located at (0,-1562)
 *                  Entry 2 = beacon located at (+1062,+1562)
 *
 */

double *anglesTable_fixed(CtrlStruct *cvs)
{
/*
    ///////////// Classical /////////////////

    CtrlIn *ivs;
    ivs = cvs->inputs;


    ///////////// Call variables /////////////////

    // From StructTower
    int counter = cvs->struct_tower->counter;
    double *tab = cvs->struct_tower->tabFixed;

    // From inputs
    double tower_pos= ivs->tower_pos;
    double *last_rising_fixed = ivs->last_rising_fixed;
    double *last_falling_fixed = ivs->last_falling_fixed;
    int rising_index_fixed = ivs->rising_index_fixed;
    int falling_index_fixed = ivs->falling_index_fixed;
    int nb_rising_fixed=ivs->nb_rising_fixed;
    int nb_falling_fixed=ivs->nb_falling_fixed;

    ///////////// Computations /////////////////

    // Each time this function is called, we calculate an angle
    double rising_edge_angle_fixed = (last_rising_fixed[rising_index_fixed]);
    double falling_edge_angle_fixed = (last_falling_fixed[falling_index_fixed]);
    double angle_beacon_fixed = (rising_edge_angle_fixed + falling_edge_angle_fixed)/2;

    // We then verify some stuff
    if((rising_index_fixed==falling_index_fixed && nb_rising_fixed>0 && nb_falling_fixed>0) && (cvs->struct_tower->previous_rising_index != rising_index_fixed))
    {
        tab[counter]=angle_beacon_fixed; // Save the angle at the correct place in the tabular of the struct_tower. We must then increment the counter to say that a beacon was saved! The counter goes from 0 to 2.

        if(counter==2) // If we have saved the angles of the 3 beacons, set the counter to 0
        {
            cvs->struct_tower->counter=0;
        }
        else // Increment the counter if we haven't set the angles of the 3 beacons
        {
            cvs->struct_tower->counter++;
        }
        cvs->struct_tower->previous_rising_index = rising_index_fixed; // previous=current
    }

    return tab; // Returns a table with the angles of the fixed beacons
    */
}



/* This function allows an easy call to the cotangent function, unavailable in math.h
 *
 * param[in] : angle a
 * return[out] : cotangent of a
 */

double cotan(double a)
{
    return cos(a)/sin(a);
}



void opponent_position(CtrlStruct *cvs,double distance_opponent, double angle_opponent)
{
  double omega;

  angle_opponent+=PI/2;

  double theta=cvs->struct_odometry->theta_t;
  double xnous=cvs->struct_odometry->x_t;
  double ynous=cvs->struct_odometry->y_t;

  double xrobot= distance_opponent*10*cos(angle_opponent);
  double yrobot= distance_opponent*10*sin(angle_opponent);

  double xbase=(cos(PI/2-theta)*xrobot + sin(PI/2-theta)*yrobot);
  double ybase=(-sin(PI/2-theta)*xrobot + cos(PI/2-theta)*yrobot);

  double xopp = xbase+xnous;
  double yopp= ybase+ynous;
  //xopp=adjust_value_to_bounds(xopp,1062);
  //yopp=adjust_value_to_bounds(yopp,1562);

  cvs->struct_tower->x_opponent=xopp;
  cvs->struct_tower->y_opponent=yopp;


}



/* This function adjusts the value of the cotangent because it could reach infinity
 *
 * param[in] : cotangent
 *
 * param[out] : adjusted cotangent
 */

double adjust_value_to_bounds( double cot , double cot_max )
{
    if (cot>cot_max){
        cot=cot_max;
    }
    else if (cot < -cot_max){
        cot=-cot_max;
    }

    return cot;
}

double weighted_dist_min(double angle_opp,double dist_min)
{
  if (angle_opp<0)
  {
    angle_opp=-angle_opp;
  }
  double alpha= (angle_opp/(PI))+1;
  return alpha*dist_min;
}

/* This function displays some useful informations about the tower
 *
 * param[in] : cvs controller main structure
 *
 */

void tower_display(CtrlStruct *cvs)
{
/*

    CtrlIn *ivs;
    ivs = cvs->inputs;

    ////// Interesting inputs //////

    // General inputs
    double tower_pos= ivs->tower_pos;
    int nb_opponents = ivs->nb_opponents;
    double t = ivs->t;

    // Fixed Beacons locations: 2 different sets depending on the IDs of the robot!

    double x1,y1,x2,y2,x3,y3;
    if(cvs->inputs->robot_id<2)
    {
        x1=0;
        y1=-1562;

        x2=1062;
        y2=1562;

        x3=-1062;
        y3=1562;

    }
    else
    {
        x1=0;
        y1=1562;

        x2=-1062;
        y2=-1562;

        x3=1062;
        y3=-1562;

    }
    // Triangulation

    // Fixed beacons angles
    double *fixedBeacons = anglesTable_fixed(cvs);

    // alpha 1 = beacon located at (0,-1562)
    // alpha 2 = beacon located at (1062,1562)
    // alpha 3 = beacon located at (-1062,1562)

    double alpha1=(fixedBeacons[1]);
    double alpha2=(fixedBeacons[2]);
    double alpha3=(fixedBeacons[0]);

    // Just to have positive angles

    if(alpha1<0)
    {
        alpha1=(2*PI)+alpha1;
    }
    if(alpha2<0)
    {
        alpha2=(2*PI)+alpha2;
    }
    if(alpha3<0)
    {
        alpha3=(2*PI)+alpha3;
    }




    // Fixed beacons
    double *last_rising_fixed = ivs->last_rising_fixed;
    double *last_falling_fixed = ivs->last_falling_fixed;
    int rising_index_fixed = ivs->rising_index_fixed;
    int falling_index_fixed = ivs->falling_index_fixed;
    int nb_rising_fixed=ivs->nb_rising_fixed;

    // Opponent beacon
    double *last_rising = ivs->last_rising;
    double *last_falling = ivs->last_falling;
    int rising_index = ivs->rising_index;
    int falling_index = ivs->falling_index;
    int counterDist = cvs->struct_tower->counterDist;



    ///////////// Computations /////////////////

    // Opponent
    double angle_opp= angle_opponent(cvs);
    double dist_moyenne=distance_opponent(cvs);
    if(dist_moyenne==1000)
    {
           dist_moyenne=cvs->struct_tower->previousMeanDistance;
    }
    if(angle_opp==1000)
    {

      angle_opp=cvs->struct_tower->previousMeanAngle;
    }

    // Computation of the coordinates of the robot thanks to triangulation
    double *coord;
    coord = (double*) malloc(2*sizeof(double));
    coord=triangulation(cvs,alpha1,alpha2,alpha3,x1,y1,x2,y2,x3,y3);

    opponent_position(cvs,dist_moyenne, angle_opp);

    double euclidian= EuclidianDistance(0,0,coord[0]/1000,coord[1]/1000);
    ////// Some // printf's //////

    // printf("***********   TIME   ***********    \n");
    // printf("Time: %f \n",t);

    // printf("\n");

    // printf("***********   GENERAL   ***********    \n");
    // printf("Tower position in degrees: %f \n",(tower_pos*180)/PI);

    // printf("\n");


    // printf("***********   OPPONENT   ***********    \n");
    // printf("Number of opponents: %d \n",nb_opponents);
    // printf("Angle of the opponent's robot in degrees: %f \n", (angle_opp)*180.0/PI);
    // printf("Distance: %f \n", dist_moyenne);
    // printf("Xpos: %f \n", cvs->struct_tower->x_opponent);
    // printf("Ypos: %f \n", cvs->struct_tower->y_opponent);

    // printf("\n");

    angle_opp=lowPassFilter(angle_opp,cvs->struct_tower->previousAngle,0.5,0.001);
    dist_moyenne=lowPassFilter(dist_moyenne,cvs->struct_tower->previousDistance,0.5,0.001);
    cvs->struct_tower->previousAngle=angle_opp;
    cvs->struct_tower->previousDistance=dist_moyenne;

    // printf(RED "***********   AFTER FILTER   ***********    \n");
    // printf("Angle of the opponent's robot in degrees: %f \n", (angle_opp)*180.0/PI);
    // printf("Distance: %f \n" RESET , dist_moyenne);

    // printf("\n");

    // printf("***********   FIXED BEACONS   ***********    \n");
    // printf("Number of rising fixed edges: %d \n",nb_rising_fixed);
    // printf("Number of falling fixed edges: %d \n",nb_rising_fixed);
    // printf("Index of rising fixed edges : %d\n",rising_index_fixed);
    // printf("Index of rising fixed edges : %d\n",falling_index_fixed);
    // printf("Counter : %d \n",cvs->struct_tower->counter);
    // printf("Angle of fixed beacon 1 (=alpha 3): %f\n",(fixedBeacons[0])*180/PI);
    // printf("Angle of fixed beacon 2 (=alpha 1): %f\n",(fixedBeacons[1])*180/PI);
    // printf("Angle of fixed beacon 3 (=alpha 2): %f\n",(fixedBeacons[2])*180/PI);

    // printf("\n");

*/

}

double outliersDetection(CtrlStruct *cvs, double distance)
{
  return 0;

}


//NAMESPACE_CLOSE();
