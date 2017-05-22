// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SmiMessage_H
#define SmiMessage_H


#include "CoinPragma.hpp"

// This deals with Smi messages (as against Osi messages etc)

#include "CoinMessageHandler.hpp"
enum SMI_Message
{
  SMI_SCENARIO_FINISHED,
  SMI_DUMMY_END
};

/** This deals with Clp messages (as against Osi messages etc)
 */
class SmiMessage : public CoinMessages {

public:

  /**@name Constructors etc */
  //@{
  /** Constructor */
  SmiMessage(Language language=us_en);
  //@}

};

#endif
