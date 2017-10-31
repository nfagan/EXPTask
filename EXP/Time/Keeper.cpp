//
//  Keeper.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include "Keeper.hpp"
#include <iostream>

EXP::Time::Keeper::Keeper()
{
    keep_counting.store(true, std::memory_order_seq_cst);
    is_counting.store(false, std::memory_order_seq_cst);
}

EXP::Time::Keeper::~Keeper() {
    time_thread->detach();
    delete time_thread;
}

double EXP::Time::Keeper::Now() const
{
    return now.load(std::memory_order_seq_cst);
}

void EXP::Time::Keeper::Start()
{
    start = std::chrono::steady_clock::now();
    if (!DidStart())
    {
        time_thread = new std::thread(&EXP::Time::Keeper::keep_time, this);
    }
}

bool EXP::Time::Keeper::DidStart()
{
    return is_counting.load(std::memory_order_seq_cst);
}

void EXP::Time::Keeper::Stop()
{
    keep_counting.store(false, std::memory_order_seq_cst);
    time_thread->join();
    is_counting.store(false, std::memory_order_seq_cst);
}

void EXP::Time::Keeper::keep_time()
{
    bool did_mark_counting = false;
    while (keep_counting.load(std::memory_order_seq_cst))
    {
        EXP::Time::time_point now_secs = std::chrono::steady_clock::now();
        EXP::Time::duration_s ellapsed = now_secs - start;
        now.store(ellapsed.count());
        if (!did_mark_counting)
        {
            is_counting.store(true, std::memory_order_seq_cst);
        }
    }
}
