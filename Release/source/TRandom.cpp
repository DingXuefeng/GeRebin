// @(#)root/mathcore:$Id$
// Author: Rene Brun, Lorenzo Moneta   15/12/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//
// TRandom
//
// basic Random number generator class (periodicity = 10**9).
// Note that this is a very simple generator (linear congruential)
// which is known to have defects (the lower random bits are correlated)
// and therefore should NOT be used in any statistical study.
// One should use instead TRandom1, TRandom2 or TRandom3.
// TRandom3, is based on the "Mersenne Twister generator", and is the recommended one,
// since it has good random proprieties (period of about 10**6000 ) and it is fast.
// TRandom1, based on the RANLUX algorithm, has mathematically proven random proprieties
// and a period of about 10**171. It is however slower than the others.
// TRandom2, is based on the Tausworthe generator of L'Ecuyer, and it has the advantage
// of being fast and using only 3 words (of 32 bits) for the state. The period is 10**26.
//
// The following table shows some timings (in nanoseconds/call)
// for the random numbers obtained using an Intel Pentium 3.0 GHz running Linux
// and using the gcc 3.2.3 compiler
//
//    TRandom           34   ns/call     (BAD Generator)
//    TRandom1          242  ns/call
//    TRandom2          37   ns/call
//    TRandom3          45   ns/call
//
//
// The following basic Random distributions are provided:
// ===================================================
//   -Exp(tau)
//   -Integer(imax)
//   -Gaus(mean,sigma)
//   -Rndm()
//   -Uniform(x1)
//   -Landau(mpv,sigma)
//   -Poisson(mean)
//   -Binomial(ntot,prob)
//
// Random numbers distributed according to 1-d, 2-d or 3-d distributions
// =====================================================================
// contained in TF1, TF2 or TF3 objects.
// For example, to get a random number distributed following abs(sin(x)/x)*sqrt(x)
// you can do :
//   TF1 *f1 = new TF1("f1","abs(sin(x)/x)*sqrt(x)",0,10);
//   double r = f1->GetRandom();
// or you can use the UNURAN package. You need in this case to initialize UNURAN
// to the function you would like to generate.
//   TUnuran u;
//   u.Init(TUnuranDistrCont(f1));
//   double r = u.Sample();
//
// The techniques of using directly a TF1,2 or 3 function is powerful and
// can be used to generate numbers in the defined range of the function.
// Getting a number from a TF1,2,3 function is also quite fast.
// UNURAN is a  powerful and flexible tool which containes various methods for
// generate random numbers for continuous distributions of one and multi-dimension.
// It requires some set-up (initialization) phase and can be very fast when the distribution
// parameters are not changed for every call.
//
// The following table shows some timings (in nanosecond/call)
// for basic functions,  TF1 functions and using UNURAN obtained running
// the tutorial math/testrandom.C
// Numbers have been obtained on an Intel Xeon Quad-core Harpertown (E5410) 2.33 GHz running
// Linux SLC4 64 bit and compiled with gcc 3.4
//
// Distribution            nanoseconds/call
//                     TRandom  TRandom1 TRandom2 TRandom3
// Rndm..............    5.000  105.000    7.000   10.000
// RndmArray.........    4.000  104.000    6.000    9.000
// Gaus..............   36.000  180.000   40.000   48.000
// Rannor............  118.000  220.000  120.000  124.000
// Landau............   22.000  123.000   26.000   31.000
// Exponential.......   93.000  198.000   98.000  104.000
// Binomial(5,0.5)...   30.000  548.000   46.000   65.000
// Binomial(15,0.5)..   75.000 1615.000  125.000  178.000
// Poisson(3)........   96.000  494.000  109.000  125.000
// Poisson(10).......  138.000 1236.000  165.000  203.000
// Poisson(70).......  818.000 1195.000  835.000  844.000
// Poisson(100)......  837.000 1218.000  849.000  864.000
// GausTF1...........   83.000  180.000   87.000   88.000
// LandauTF1.........   80.000  180.000   83.000   86.000
// GausUNURAN........   40.000  139.000   41.000   44.000
// PoissonUNURAN(10).   85.000  271.000   92.000  102.000
// PoissonUNURAN(100)   62.000  256.000   69.000   78.000
//
//  Note that the time to generate a number from an arbitrary TF1 function
//  using TF1::GetRandom or using TUnuran is  independent of the complexity of the function.
//
//  TH1::FillRandom(TH1 *) or TH1::FillRandom(const char *tf1name)
//  ==============================================================
//  can be used to fill an histogram (1-d, 2-d, 3-d from an existing histogram
//  or from an existing function.
//
//  Note this interesting feature when working with objects
//  =======================================================
//  You can use several TRandom objects, each with their "independent"
//  random sequence. For example, one can imagine
//     TRandom *eventGenerator = new TRandom();
//     TRandom *tracking       = new TRandom();
//  eventGenerator can be used to generate the event kinematics.
//  tracking can be used to track the generated particles with random numbers
//  independent from eventGenerator.
//  This very interesting feature gives the possibility to work with simple
//  and very fast random number generators without worrying about
//  random number periodicity as it was the case with Fortran.
//  One can use TRandom::SetSeed to modify the seed of one generator.
//
//  a TRandom object may be written to a Root file
//  ==============================================
//    -as part of another object
//    -or with its own key (example gRandom->Write("Random");
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "TRandom.h"

#include <ctime>
//#include <random>

////////////////////////////////////////////////////////////////////////////////
/// Default constructor. For seed see SetSeed().

TRandom::TRandom(unsigned int seed)
{
   SetSeed(seed);
}

////////////////////////////////////////////////////////////////////////////////
///  Machine independent random number generator.
///  Based on the BSD Unix (Rand) Linear congrential generator.
///  Produces uniformly-distributed floating points between 0 and 1.
///  Identical sequence on all machines of >= 32 bits.
///  Periodicity = 2**31, generates a number in (0,1).
///  Note that this is a generator which is known to have defects
///  (the lower random bits are correlated) and therefore should NOT be
///  used in any statistical study).

double TRandom::Rndm()
{
#ifdef OLD_TRANDOM_IMPL
   const double kCONS = 4.6566128730774E-10;
   const int kMASK24  = 2147483392;

   fSeed *= 69069;
   int jy = (fSeed&kMASK24); // Set lower 8 bits to zero to assure exact float
   if (jy) return kCONS*jy;
   return Rndm();
#endif

   // kCONS = 1./2147483648 = 1./(RAND_MAX+1) and RAND_MAX= 0x7fffffffUL
   const double kCONS = 4.6566128730774E-10; // (1/pow(2,31)
   fSeed = (1103515245 * fSeed + 12345) & 0x7fffffffUL;

   if (fSeed) return  kCONS*fSeed;
   return Rndm();
}

////////////////////////////////////////////////////////////////////////////////
/// Set the random generator seed. Note that default value is zero, which is
/// different than the default value used when constructing the class.
/// If the seed is zero the seed is set to a random value
/// which in case of TRandom depends on the lowest 4 bytes of TUUID
/// The UUID will be identical if SetSeed(0) is called with time smaller than 100 ns
/// Instead if a different generator implementation is used (TRandom1, 2 or 3)
/// the seed is generated using a 128 bit UUID. This results in different seeds
/// and then random sequence for every SetSeed(0) call.

void TRandom::SetSeed(unsigned int seed)
{
   if( seed==0 ) {
	   fSeed = time(NULL);
   } else {
	   fSeed = seed;
   }
}

