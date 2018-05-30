/* ==================================================================== 
 * Title:     filter.h                                                  
 *                                                                      
 * Purpose:                                                             
 *            Present typedefs, global variables declaration and       
 * function prototypes.                                                 
 *                                                                      
 * Copyright 2008 National Instruments Corporation. All rights reserved.       
 * ==================================================================== 
 */

#ifndef __filter_SOS_DIRECT_IIR_II_T_H__
#define __filter_SOS_DIRECT_IIR_II_T_H__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "nidfdtyp.h"

#ifdef __cplusplus
extern "C"{
#endif

/* ----Typedef---- */
typedef I32* filter_State;

/* ----Create/Dispose State---- */
filter_State filter_CreateState ( );
void filter_DisposeState (filter_State state);

/* ----Initialize State---- */
void filter_InitState (filter_State state);

/* ----Filter Implementation---- */
I16 filter_Filtering (I16 sampleIn, filter_State state);

/* ----Demo Program---- */
/* void main()
 * {
 *     //Declare and create State
 *     filter_State state;
 *     state = filter_CreateState();
 *
 *     ...
 *
 *     //Initialize State
 *     filter_InitState (state);
 *
 *     ...
 *
 *     //Processing
 *     while ()
 *     {
 *         ...
 *         y = filter_Filtering(x, state);
 *         ...
 *     };
 *
 *     ...
 *
 *     //Dispose State
 *     filter_DisposeState (state);
 *
 *     ...
 *
 *     return;
 * }
 */

#ifdef __cplusplus
}
#endif

#endif //#ifndef __filter_SOS_DIRECT_IIR_II_T_H__
