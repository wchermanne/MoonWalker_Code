//
//  Odometry_gr1.hpp
//
//
//  Created by William Chermanne on 12/03/17.
//
//
#include <stdio.h>
#include <stdio.h>

#ifndef Website_interface_hpp
#define Website_interface_hpp
#include "MyIncludes_gr1.h"
#include "MyIncludes_Moonwalker.h"
#include <stdio.h>
void StructWebsite_GetData(CtrlStruct *cvs);
void StructWebsite_SendData(CtrlStruct *cvs,int sockfd);
void StructWebsite_init(CtrlStruct *cvs);

//NAMESPACE_CLOSE();

#endif /* Odometry_gr1_hpp */
