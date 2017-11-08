//
//  EXP_ASSERT.h
//  EXP
//
//  Created by Nick Fagan on 11/5/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef EXP_ASSERT_h
#define EXP_ASSERT_h

#include <iostream>

#ifndef NDEBUG
#   define EXP_ASSERT(condition, message) \
do { \
if (! (condition)) { \
std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
<< " line " << __LINE__ << ": " << "\n\n" << message << std::endl; \
std::terminate(); \
} \
} while (false)
#else
#   define EXP_ASSERT(condition, message) do { } while (false)
#endif

#endif /* EXP_ASSERT_h */
