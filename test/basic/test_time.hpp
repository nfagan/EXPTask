//
//  test_time.hpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef test_time_h
#define test_time_h

#include <EXP.hpp>
#include <iostream>

namespace EXP {
namespace test_case {
    
    int test_time_(void);
    
    int test_time(void)
    {
        for (unsigned i = 0; i < 10; i++)
        {
            test_time_();
        }
        return 0;
    }
    
    int test_time_(void)
    {
        typedef std::chrono::high_resolution_clock Time;
        typedef std::chrono::duration<float> fsec;
        
        EXP::Time::Keeper *time = new EXP::Time::Keeper();
        time->Start();
        
        while (!time->DidStart()) {};
        
        double start = time->Now();
        double ellapsed = 0.0;
        double avg = 0.0;
        double current = 0.0;
        double last = 0.0;
        unsigned long iterations = 0;
        EXP::Time::Timer timer(time, EXP::Time::duration_ms(100));
        timer.Reset();
        while (!timer.Ellapsed())
        {   
            auto t0 = Time::now();
            ellapsed = time->Now() - start;
            auto t1 = Time::now();
            fsec fs = t1 - t0;
            double iterationd = (double)iterations;

            double delta = fs.count();

            iterationd = (double)iterations;
            avg = (avg * iterationd + delta) / (iterationd+1.0f);

            iterations++;

            if (ellapsed > 1)
            {
                fprintf(stderr, "\nWas greater");
            }
        }
        
        time->Stop();
        
        std::cout << avg << std::endl;
        std::cout << ellapsed << std::endl;
        
        return 0;
    }
    
}
}

#endif /* test_time_h */
