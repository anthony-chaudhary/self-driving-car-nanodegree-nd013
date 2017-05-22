//===========================================================================//
// This file is part of the Decomp Solver Framework.                         //
//                                                                           //
// Decomp is distributed under the Common Public License as part of the      //
// COIN-OR repository (http://www.coin-or.org).                              //
//                                                                           //
// Author: Matthew Galati, Lehigh University   
// Modified by Kipp Martin
//
//                                                                           //
// Copyright (C) 2002-2009, Lehigh University, Matthew Galati, and Ted Ralphs//
// All Rights Reserved.                                                      //
//===========================================================================//

//===========================================================================//
#include "UtilParameters.h"
//===========================================================================//
#include "OSDipApp.h"
//===========================================================================//
#include "AlpsDecompModel.h"
//===========================================================================//
#include "DecompAlgoC.h"
#include "DecompAlgoPC.h"
#include "DecompAlgoRC.h"
//===========================================================================//
#include "UtilTimer.h"
#include<map>

//===========================================================================//
int main(int argc, char ** argv){
   try{

      //---
      //--- create the utility class for parsing parameters
      //---
      UtilParameters utilParam(argc, argv);  

      bool doCut          = utilParam.GetSetting("doCut",          false);
      bool doPriceCut     = utilParam.GetSetting("doPriceCut",     true);
      bool doDirect       = utilParam.GetSetting("doDirect",       false);

   
      UtilTimer timer;
      double    timeSetupReal = 0.0;
      double    timeSetupCpu  = 0.0;
      double    timeSolveReal = 0.0;
      double    timeSolveCpu  = 0.0;
   
      //---
      //--- start overall timer
      //---
      timer.start();
      
      //---
      //--- create the user application (a DecompApp)
      //---  
      
      OSDipApp osdip( utilParam); 
      
      //return 0;
      
     
      //---
      //--- create the algorithm (a DecompAlgo)
      //---
      DecompAlgo *algo = NULL;
      assert(doCut + doPriceCut == 1);

      //---
      //--- create the CPM algorithm object
      //---      
      if(doCut)
         algo = new DecompAlgoC(&osdip, &utilParam);
   
      //---
      //--- create the PC algorithm object
      //---
      if(doPriceCut){
    	 std::cout << "CREATE NEW DecompAlgoPC" << std::endl;
         algo = new DecompAlgoPC(&osdip, &utilParam);
         std::cout << "DONE CREATE NEW DecompAlgoPC" << std::endl;
         //delete algo;
         //return 0;

      }
   
      if(doCut && doDirect){
         timer.stop();
         timeSetupCpu  = timer.getCpuTime();
         timeSetupReal = timer.getRealTime();
      
         //---
         //--- solve
         //---
         timer.start();      
        // algo->solveDirect(timeLimit);
         algo->solveDirect();
         timer.stop();
         timeSolveCpu  = timer.getCpuTime();
         timeSolveReal = timer.getRealTime();
      }
      else{
         timer.stop();
         timeSetupCpu  = timer.getCpuTime();
         timeSetupReal = timer.getRealTime();
      
         //---
         //--- create the driver AlpsDecomp model
         //---
         int             status = 0;
         AlpsDecompModel alpsModel(utilParam, algo);
      
         //---
         //--- solve
         //---
         timer.start();     
         std::cout << std::endl << std::endl << std::endl << std::endl;
         std::cout << "***************START SOLVE***************" << std::endl;
         
    
         
         status = alpsModel.solve();
         std::cout << "FINISH SOLVE" << std::endl;
         timer.stop();
         timeSolveCpu  = timer.getCpuTime();
         timeSolveReal = timer.getRealTime();
         
         //---
         //--- sanity check
         //---
         //cout << setiosflags(std::ios::fixedl std::ios::showpoint);
         std::cout << "Status= " << status 
              << " BestLB= " << std::setw(10) 
              << UtilDblToStr(alpsModel.getGlobalLB(),5)
              << " BestUB= " << std::setw(10)
              << UtilDblToStr(alpsModel.getGlobalUB(),5)        
              << " Nodes= " << std::setw(6) 
              << alpsModel.getNumNodesProcessed()
              << " SetupCPU= "  << timeSetupCpu
              << " SolveCPU= "  << timeSolveCpu 
              << " TotalCPU= "  << timeSetupCpu + timeSolveCpu
              << " SetupReal= " << timeSetupReal
              << " SetupReal= " << timeSolveReal
              << " TotalReal= " << timeSetupReal + timeSetupReal
              << std::endl;  
         
         
         const DecompSolution * solution = alpsModel.getBestSolution();
         std::cout << "Optimal Solution" << std::endl;
         solution->print();
         //print final solution
         algo->getMasterOSI()->writeLp("finalRestrictedMaster", "lp", 1e-30, 5, 10);
      
      }	 
      //---
      //--- free local memory
      //---
      delete algo;

   }
	catch(const ErrorClass& eclass){
		std::cout << "Something went wrong:" << std::endl;
		std::cout << eclass.errormsg << std::endl;
		
      return 1;
   }
   std::cout << "Program terminates normally" << std::endl;
   return 0;
}

