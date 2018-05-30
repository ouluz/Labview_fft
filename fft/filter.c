/* ==================================================================== 
 * Title:     filter.c                                                  
 *                                                                      
 * Purpose:                                                             
 *            Implement a Fixed-Point filter.                          
 *                                                                      
 * Comments:                                                            
 *                                                                      
 * Structure type:     IIR Cascaded Second Order Sections Form II Transposed
 * 
 * Fixed-Point Models:
 *
 * CoefficientB Quantizer:
 *           Word Length:  16
 *   Integer Word Length:  -5
 *         Overflow Mode:  Saturation
 *        Round-off Mode:  Nearest
 * 
 * CoefficientA Quantizer:
 *           Word Length:  16
 *   Integer Word Length:  1
 *         Overflow Mode:  Saturation
 *        Round-off Mode:  Nearest
 * 
 * Input Quantizer:
 *           Word Length:  16
 *   Integer Word Length:  1
 *         Overflow Mode:  Saturation
 *        Round-off Mode:  Nearest
 * 
 * Output Quantizer:
 *           Word Length:  16
 *   Integer Word Length:  2
 *         Overflow Mode:  Wrap
 *        Round-off Mode:  Truncation
 * 
 * Multiplicand Quantizer:
 *           Word Length:  32
 *   Integer Word Length:  2
 *         Overflow Mode:  Wrap
 *        Round-off Mode:  Truncation
 * 
 * Product Quantizer:
 *           Word Length:  32
 *   Integer Word Length:  2
 *         Overflow Mode:  Wrap
 *        Round-off Mode:  Truncation
 * 
 * Sum Quantizer:
 *           Word Length:  32
 *   Integer Word Length:  2
 *         Overflow Mode:  Wrap
 *        Round-off Mode:  Truncation
 * 
 * Delay Quantizer:
 *           Word Length:  32
 *   Integer Word Length:  2
 *         Overflow Mode:  Wrap
 *        Round-off Mode:  Truncation
 * 
 *                                                           
 * Copyright 2008 National Instruments Corporation. All rights reserved.       
 * ==================================================================== 
 */

#include "filter.h"


/* ---- Constants ----*/
#define FILTER_N_SECTIONS 1
#define FILTER_N_STATES (2*FILTER_N_SECTIONS)
#define FILTER_N_COEFA (2*FILTER_N_SECTIONS)
#define FILTER_N_COEFB (3*FILTER_N_SECTIONS)

/* ---- Global Variables Declaration ----*/
static I16 filter_CoefA[FILTER_N_COEFA];
static I16 filter_CoefB[FILTER_N_COEFB];
static I32 filter_MultB(I32 x, I16 coef);
static I32 filter_MultA(I32 x, I16 coef);


/* ---- SHFT Define ---- */
#define IN2SUM 15                       /* IN2SUM LeftShift 15 */
#define SUM2OUT 16                      /* SUM2OUT RightShift 16 */


/* ---- Create State of filter ---- */ 
filter_State filter_CreateState ( )
{
	filter_State state = NULL;
	
	state = (filter_State)malloc(sizeof(I32) * FILTER_N_STATES);
	
	return state;
}

/* ---- Dispose State of filter ---- */
void filter_DisposeState (filter_State state)
{
	free(state);
	
	return;
}

/* ---- Initialize the State of filter ---- */
void filter_InitState(filter_State state)
{
	int i;
	
	for (i=0; i < FILTER_N_STATES; i++)
		state[i] = 0;
	
	return;
}


/* ---- Implement the Fixed-Point Filter ---- */
I16 filter_Filtering (I16 sampleIn, filter_State state)
{
	I32 accA, accB;
	I32 mult_a, mult_b;
	int i;    
	int iA, iB;

	accA = ((I32)sampleIn << IN2SUM);

	iA = iB = 0;
	for (i=0; i < FILTER_N_SECTIONS; i++)
	{
		mult_b = accA;

		/* Filtering */
		accA = filter_MultB(mult_b, filter_CoefB[iB++]);
		accA += state[iA];

		mult_a = accA;

		/* Update the internal states */
		accB = state[iA+1];
		accB += filter_MultB(mult_b, filter_CoefB[iB++]);
		accB -= filter_MultA(mult_a, filter_CoefA[iA]);
		state[iA++] = accB;

		accB = filter_MultB(mult_b, filter_CoefB[iB++]);
		accB -= filter_MultA(mult_a, filter_CoefA[iA]);
		state[iA++] = accB;
	}

	accA = (accA >> SUM2OUT);

	return (I16)accA;
}

/* ---- Internal Functions ---- */
/* ==================================================================== 
 *  Fixed-Point Multiplication:
 *              x     *   coef    =>   prod
 *           Q2.30   *  Q-5.21   =>  Q2.30
 * ==================================================================== 
 */

static I32 filter_MultB(I32 x, I16 coef)
{
	I32 prod;

	prod = (x >> 16) * coef;
	prod = (prod + (((x & 0xffff) * coef) >> 16)) >> 5;

	return prod;
}

/* ==================================================================== 
 *  Fixed-Point Multiplication:
 *              x     *   coef    =>   prod
 *           Q2.30   *  Q1.15   =>  Q2.30
 * ==================================================================== 
 */

static I32 filter_MultA(I32 x, I16 coef)
{
	I32 prod;

	prod = ((x >> 16) * coef) << 1;
	prod += ((x & 0xffff) * coef) >> 15;

	return prod;
}



/* ---- Filter Coefficients ---- */
static I16 filter_CoefA [] = {
	-32197,
	0
};
static I16 filter_CoefB [] = {
	18287,
	18287,
	0
};
