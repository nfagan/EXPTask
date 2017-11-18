//
//  test_threaded_states.hpp
//  EXP
//
//  Created by Nick Fagan on 11/1/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef test_threaded_states_h
#define test_threaded_states_h

#include <EXP.hpp>
#include <EXPGL.hpp>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <atomic>
#include <condition_variable>

namespace EXP {
    namespace test_case {
        
        enum STATES
        {
            STATE1,
            STATE2,
            STATE3
        } states;
        
        Time::Keeper *time = new Time::Keeper();
        GLContextManager *gl_manager = nullptr;
        ResourceManager *resource_manager = nullptr;
        std::vector<Window*> windows;
        RenderTarget *render_target = nullptr;
        Renderer *renderer = nullptr;
        InputKeyboard *keyboard = nullptr;
        Shader2D *shader = nullptr;
        Rectangle *rectangle = nullptr;
        Rectangle *rectangle2 = nullptr;
        MaterialSolid2D *mat = nullptr;
        glm::vec2 rect_pos = Positions2D::CENTER;
        float step_amount = 0.005f;
        unsigned long iterations = 0;
        double avg = 0.0;
        double current = 0.0;
        double last = 0.0;
        double delta = 0.0;
        double min_value = std::numeric_limits<double>::max();
        double max_value = std::numeric_limits<double>::min();
        bool do_draw = true;
        
        //
        //  thread stuff
        //
        
        std::mutex first_mutex;
        std::condition_variable wait_condition;
        std::vector<EXP::Model2D*> conditional_queue;
        std::unordered_map<unsigned int, bool> already_present;
        std::atomic<bool> should_draw;
        bool can_draw_from_render = true;
        bool can_draw_from_task = false;
        
        void render_loop(void)
        {
            
            while (true)
            {
                gl_manager->PollEvents();
                
                if (should_draw.load())
                {
                    std::unique_lock<std::mutex> lk(first_mutex);
                    wait_condition.wait(lk, [] { return can_draw_from_task; });
                    can_draw_from_task = false;
                    for (unsigned i = 0; i < conditional_queue.size(); i++)
                    {
                        renderer->Queue(conditional_queue[i]);
                    }
                    conditional_queue.clear();
                    already_present.clear();
                    can_draw_from_render = true;
                    lk.unlock();
                    wait_condition.notify_one();
                    renderer->Draw();
                }
                
                if (keyboard->KeyDown(GLFW_KEY_ESCAPE))
                {
                    break;
                }
                if (keyboard->KeyDown(GLFW_KEY_A)) rect_pos.x -= step_amount;
                if (keyboard->KeyDown(GLFW_KEY_S)) rect_pos.y += step_amount;
                if (keyboard->KeyDown(GLFW_KEY_D)) rect_pos.x += step_amount;
                if (keyboard->KeyDown(GLFW_KEY_W)) rect_pos.y -= step_amount;
                
                rectangle->SetPosition(rect_pos);
            }
        }
        
        void do_conditional_queue(const std::vector<Model2D*> &models)
        {
            std::unique_lock<std::mutex> lk(first_mutex);
            wait_condition.wait(lk, [] { return can_draw_from_render; });
            can_draw_from_render = false;
            
            for (unsigned i = 0; i < models.size(); i++)
            {
                auto it = already_present.find(models[i]->GetId());
                if (it == already_present.end())
                {
                    already_present[models[i]->GetId()] = true;
                    conditional_queue.push_back(models[i]);
                }
            }
            can_draw_from_task = true;
            lk.unlock();
            wait_condition.notify_one();
        }
        
        void await_queue_available()
        {
            std::unique_lock<std::mutex> lk(first_mutex);
            wait_condition.wait(lk, [] { return can_draw_from_render; });
            can_draw_from_render = false;
            can_draw_from_task = true;
            lk.unlock();
            wait_condition.notify_one();
        }
        
        //
        //  task stuff
        //
        
        void task_loop(EXP::Task* task)
        {
        }
        
        void on_loop(EXP::State *input)
        {
            mat->SetAlbedo(Colors::RED);
            if (do_draw)
            {
                std::vector<Model2D*> models = { rectangle, rectangle2, rectangle };
                do_conditional_queue(models);

//                do_conditional_queue(rectangle);
                
                should_draw.store(true);
            }
        }
        
        void on_loop2(EXP::State *input)
        {
//            mat->SetAlbedo(Colors::GREEN);
            if (do_draw)
            {
//                std::vector<Model2D*> models = { rectangle };
//                do_conditional_queue(models);
                await_queue_available();
                should_draw.store(true);
            }
        }
        
        
        void on_entry(State *state)
        {
            current = time->Now();
            if (iterations > 0)
            {
                delta = current - last;
                avg = (avg * ((double)iterations-1.0) + delta) / ((double)iterations);
                if (delta < min_value) min_value = delta;
                if (delta > max_value) max_value = delta;
            }
            last = current;
            iterations++;
            std::cout << "Entered state 1!" << std::endl;
        }
        
        void on_entry2(State *state)
        {
            std::cout << "Entered state 2!" << std::endl;
        }

        void on_exit(State *state)
        {
            state->Next(state->GetStateById(STATE2));
//            rect_pos = Positions2D::LEFT_CENTER;
            std::cout << "Exited state 1!" << std::endl;
        }
        
        void on_exit2(State *state)
        {
//            rect_pos = Positions2D::CENTER;
//            rectangle->SetDimensions(100.0f, 200.0f);
            std::cout << "Exited state 2!" << std::endl;
            state->Next(state->GetStateById(STATE1));
        }
        
        void task_thread_loop(void)
        {
            Task *task = new Task(time);
            State *state1 = task->CreateState(STATE1);
            State *state2 = task->CreateState(STATE2);
            
            state1->AddExitCondition<exit_conditions::time_exceeded>();
            
            int duration1 = 400;
            state1->Entry(on_entry);
            state1->Loop(on_loop);
            state1->Exit(on_exit);
            state1->TimeIn(Time::duration_ms(duration1));
            
            int duration2 = 400;
            state2->TimeIn(Time::duration_ms(duration2));
            state2->Entry(on_entry2);
            state2->Loop(on_loop2);
            state2->Exit(on_exit2);
            state2->AddExitCondition<exit_conditions::time_exceeded>();
            
            task->AddExitCondition<exit_conditions::key_pressed>(keyboard, GLFW_KEY_ESCAPE);
            task->AddExitCondition<exit_conditions::key_pressed>(keyboard, GLFW_KEY_SPACE);
            task->Loop(task_loop);
            task->Next(state1);
            
            double start = time->Now();
            task->Run();
            double stop = time->Now();
            std::cout << "\nEllapsed time: " << (stop - start) << "s\n" << std::endl;
            std::cout << task->ExitReason() << std::endl;
            std::cout << "\nN iterations: " << iterations << "\n";
            std::cout << "\nAverage trial-time: " << avg * 1000 << "\n";
            std::cout << "\nExpected trial-time: " << (duration1 + duration2) << "\n";
            std::cout << "\nMax trial-time: " << max_value << "\n";
            std::cout << "\nMin trial-time: " << min_value << std::endl;
        }
        
        
        int test_states(void)
        {
            using namespace EXP;
            
            gl_manager = new GLContextManager();
            resource_manager = new ResourceManager();
            windows.push_back(gl_manager->OpenWindow(0, 400, 400, nullptr));
            render_target = gl_manager->CreateRenderTarget(windows);
            renderer = new Renderer(render_target);
            mat = resource_manager->CreateMaterial<EXP::MaterialSolid2D>();
            rectangle = resource_manager->CreateModel<EXP::Rectangle>(render_target);
            rectangle2 = resource_manager->CreateModel<EXP::Rectangle>(render_target);
            shader = new Shader2D();
            keyboard = new InputKeyboard(render_target);
            
            renderer->SetClearColor(Colors::WHITE);
            
            rectangle->SetShader(shader);
            rectangle->SetDimensions(100.0f, 200.0f);
            rectangle->SetPosition(Positions2D::CENTER);
            rectangle->SetUnits(Model::MIXED);
            rectangle->SetMaterial(mat);
            
            rectangle2->SetMaterial(mat);
            rectangle2->SetShader(shader);
            rectangle2->SetPosition(Positions2D::LEFT_CENTER);
            rectangle2->SetUnits(Model::MIXED);
            rectangle2->SetDimensions(50.0f, 50.0f);
            
            render_target->SetWindowOffsets(RenderTarget::HORIZONTAL);
            
            time->Start();
            
            std::thread t1(task_thread_loop);
            render_loop();
            t1.join();
            
            return 0;
        }
        
    }
}

#endif /* test_threaded_states_h */
