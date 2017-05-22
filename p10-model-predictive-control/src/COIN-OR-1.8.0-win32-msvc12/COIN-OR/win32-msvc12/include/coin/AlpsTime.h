/*===========================================================================*
 * This file is part of the Abstract Library for Parallel Search (ALPS).     *
 *                                                                           *
 * ALPS is distributed under the Eclipse Public License as part of the       *
 * COIN-OR repository (http://www.coin-or.org).                              *
 *                                                                           *
 * Authors:                                                                  *
 *                                                                           *
 *          Yan Xu, Lehigh University                                        *
 *          Ted Ralphs, Lehigh University                                    *
 *                                                                           *
 * Conceptual Design:                                                        *
 *                                                                           *
 *          Yan Xu, Lehigh University                                        *
 *          Ted Ralphs, Lehigh University                                    *
 *          Laszlo Ladanyi, IBM T.J. Watson Research Center                  *
 *          Matthew Saltzman, Clemson University                             *
 *                                                                           * 
 *                                                                           *
 * Copyright (C) 2001-2013, Lehigh University, Yan Xu, and Ted Ralphs.       *
 *===========================================================================*/

#ifndef AlpsTime_
#define AlpsTime_

//#############################################################################

#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include "Alps.h"
#include "AlpsConfig.h"

#include "CoinTime.hpp"

#ifdef COIN_HAS_MPI
# include "mpi.h"
#endif

//#############################################################################

#define AlpsCpuTime CoinCpuTime

//#############################################################################

static inline double AlpsWallClock()
{

#ifndef COIN_HAS_MPI
    double cpu_temp;
#if defined(_MSC_VER) || defined(__MSVCRT__)
    unsigned int ticksnow;        /* clock_t is same as int */
    ticksnow = (unsigned int)clock();
    cpu_temp = (double)((double)ticksnow/CLOCKS_PER_SEC);
    double sys_temp = 0.;
#else
    double sys_temp;
    struct rusage usage;
    getrusage(RUSAGE_SELF,&usage);
    cpu_temp = (double) usage.ru_utime.tv_sec;
    cpu_temp += 1.0e-6*((double) usage.ru_utime.tv_usec);
    sys_temp = (double) usage.ru_stime.tv_sec
	+ 1.e-6 * (double) usage.ru_stime.tv_usec;
#endif
    return cpu_temp + sys_temp;
#else
    // COIN_HAS_MPI
    return MPI_Wtime();
#endif
}

//#############################################################################

/* A timer used to record cpu and wallclock time. */
class AlpsTimer 
{
 public:  /* Public for parallecl gather. */

    int clockType_;

    /** Time limit. */
    double limit_;
    
    double startCpu_;
    double startWall_;
    double finishCpu_;
    double finishWall_;
    
    /** Cpu time. */
    double cpu_;

    /** Wall clock time. */
    double wall_;
    
 public:
    AlpsTimer() : clockType_(AlpsClockTypeWallClock), limit_(ALPS_DBL_MAX) { reset(); }
    AlpsTimer(double lt) : limit_(lt) { reset(); }
    ~AlpsTimer()  {}

    /** Reset. */
    void reset() {
	startCpu_ = 0.0;
	startWall_ = 0.0;
	finishCpu_ = 0.0;
	finishWall_ = 0.0;
	cpu_ = 0.0;
	wall_ = 0.0;
    }
    
    /** Start to count times. */
    void start() {
	startCpu_ = AlpsCpuTime();
	startWall_ = AlpsWallClock();
    }
    
    /** Stop timer and computing times. */
    void stop() {
	finishCpu_ = AlpsCpuTime();
	finishWall_ = AlpsWallClock();
	cpu_ = finishCpu_ - startCpu_;
	wall_ = finishWall_ - startWall_;
    }
    
    //{@
    void setLimit(double lm) { limit_ = lm; }
    double getLimit() const { return limit_; }
    //@}

    /** Get cpu timee. */
    double getCpuTime() { 
	finishCpu_ = AlpsCpuTime();
        cpu_ = finishCpu_ - startCpu_;
        return cpu_; 
    }

    /** Get cpu timee. */
    double getWallClock() { 
        finishWall_ = AlpsWallClock();
        wall_ = finishWall_ - startWall_;
        return wall_; 
    }
    
    /** Get time depends on clock type. */
    double getTime() {
      assert( (clockType_ == AlpsClockTypeCpu) ||
	      (clockType_ == AlpsClockTypeWallClock) );
      if (clockType_ == AlpsClockTypeCpu) {
	finishCpu_ = AlpsCpuTime();
	cpu_ = finishCpu_ - startCpu_;
	return cpu_;
      }
      else {
        finishWall_ = AlpsWallClock();
        wall_ = finishWall_ - startWall_;
        return wall_; 
      }
    }

    /** Get/Set clock type */
    int getClockType(){ return clockType_; }
    void setClockType(int ct){ clockType_ = ct; }

    /** Check if cpu time reach limit. */
    bool reachCpuLimit() {
	finishCpu_ = AlpsCpuTime();
	finishWall_ = AlpsWallClock();
	if (finishCpu_ - startCpu_ > limit_) {
	    return true;
	}
	else {
	    return false;
	}
    }
    
    /** Check if wallclock time reach limit. */
    bool reachWallLimit() {
	finishCpu_ = AlpsCpuTime();
	finishWall_ = AlpsWallClock();
	if (finishWall_ - startWall_ > limit_) {
	    return true;
	}
	else {
	    return false;
	}
    }
};

#endif
