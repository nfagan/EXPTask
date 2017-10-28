//
//  EXP.cpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <iostream>
#include "EXP.hpp"
#include "EXPPriv.hpp"

void EXP::HelloWorld(const char * s)
{
    EXPPriv *theObj = new EXPPriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void EXPPriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

