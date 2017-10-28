//
//  EXPPriv.hpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

/* The classes below are not exported */
#pragma GCC visibility push(hidden)

class EXPPriv
{
    public:
    void HelloWorldPriv(const char *);
};

#pragma GCC visibility pop
