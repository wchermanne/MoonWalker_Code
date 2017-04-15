//
//  Actions.hpp
//
//
//  Created by Robin Van Lierde  on 4/11/17.
//
//

#ifndef Actions_hpp
#define Actions_hpp
#include "MyIncludes_gr1.h"

#include <stdio.h>


//NAMESPACE_INIT(ctrlGr1);
void StructActions_init(CtrlStruct *cvs);

void funnyAction(MyVannes electrovannes);

void takeModules(args *atab,int ModulesToTake);
void lowerElevator(MyDynamixel dynaElevator);
void ejectModules(args *atab, int ModulesToEject);
void closeGripper(MyVannes electrovannes);
void openGripper(MyVannes electrovannes);
void gripperOut(MyVannes electrovannes);
void gripperIn(MyVannes electrovannes);
void upperElevator(MyDynamixel dynaElevator);
void Stop_Everything(args* atab);





//NAMESPACE_CLOSE();

#endif /* Actions.hpp */
