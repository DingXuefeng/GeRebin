// @(#)root/mathcore:$Id$
// Author: Rene Brun   15/12/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TRandom
#define ROOT_TRandom



//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRandom                                                              //
//                                                                      //
// Simple prototype random number generator class (periodicity = 10**9) //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TRandom {

protected:
   unsigned int   fSeed;  //Random number generator seed

public:
   TRandom(unsigned int seed=0);
   virtual  void     SetSeed(unsigned int seed=0);
   virtual  double Rndm();

};

#endif
