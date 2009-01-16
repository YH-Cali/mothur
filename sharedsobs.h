#ifndef SHAREDSOBS_H
#define SHAREDSOBS_H
/*
 *  sharedsobs.h
 *  Dotur
 *
 *  Created by Sarah Westcott on 1/7/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */

/* This class implements the SharedSobs estimator on two groups. 
It is a child of the calculator class. */


#include <Carbon/Carbon.h>
#include "calculator.h"

/***********************************************************************/
class SharedSobs : public Calculator {

public:
	SharedSobs() : Calculator("SharedSobs", 1) {};
	EstOutput getValues(SAbundVector* rank){ return data; };
	EstOutput getValues(SharedRAbundVector* shared1, SharedRAbundVector* shared2);
};

/***********************************************************************/

#endif
