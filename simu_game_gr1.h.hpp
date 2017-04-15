/*!
 * \author group 1
 * \file simu_game_gr1.h
 * \brief choose between controller for simulation game or for the real robot
 */

#ifndef _SIMU_GAME_GR1_H_
#define _SIMU_GAME_GR1_H_

//#include "NAMESPACE_ctrl.h"
#include "MyIncludes_gr1.h"

//NAMESPACE_INIT(ctrlGr1);

#ifdef SIMU_PROJECT
#define SIMU_GAME // comment this line to see in simulation the controller which will be tested on the real robot
#endif

//NAMESPACE_CLOSE();

#endif
