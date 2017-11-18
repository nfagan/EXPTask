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
        Rectangle *rect3 = nullptr;
        MaterialSolid2D *mat = nullptr;
        Texture2D *tex = nullptr;
        MaterialTexture2D *mat_tex = nullptr;
        glm::vec2 rect_pos = Positions2D::CENTER;
        
        std::vector<Model*> rectangles;
        
        float step_amount = 0.5f;
        unsigned long iterations = 0;
        double avg = 0.0;
        double current = 0.0;
        double last = 0.0;
        double delta = 0.0;
        double min_value = std::numeric_limits<double>::max();
        double max_value = std::numeric_limits<double>::min();
        
        //
        //  thread stuff
        //
        
        std::vector<EXP::Model*> conditional_queue;
        std::unordered_map<unsigned int, bool> already_present;
        std::atomic<bool> should_draw;
        std::atomic<bool> can_access_draw_queue;
        
        void render_loop(void)
        {
            glm::vec2 velocity(0.0f);
            float current_frame = 0.0;
            float last_frame = 0.0;
            float delta_ticks = 0;
            
            while (true)
            {
                gl_manager->PollEvents();
                current_frame = glfwGetTime();
                delta_ticks = current_frame - last_frame;
                last_frame = current_frame;
                
                while (!should_draw.load());
//                if (should_draw.load())
//                {
                    while (!can_access_draw_queue.load())
                    {
                        //
                    }
                    can_access_draw_queue.store(false);
                    for (unsigned i = 0; i < conditional_queue.size(); i++)
                    {
                        renderer->Queue(conditional_queue[i]);
                    }
                    can_access_draw_queue.store(true);
                    renderer->Draw();
//                }
                
                velocity.x = 0;
                velocity.y = 0;
                
                if (keyboard->KeyDown(GLFW_KEY_ESCAPE))
                {
                    break;
                }
                if (keyboard->KeyDown(GLFW_KEY_A)) velocity.x -= step_amount;
                if (keyboard->KeyDown(GLFW_KEY_S)) velocity.y += step_amount;
                if (keyboard->KeyDown(GLFW_KEY_D)) velocity.x += step_amount;
                if (keyboard->KeyDown(GLFW_KEY_W)) velocity.y -= step_amount;
                if (keyboard->KeyDown(GLFW_KEY_SPACE))
                {
                    mat->SetAlbedo(Colors::GREY_50);
                }
                
                rect_pos.x += velocity.x * (delta_ticks);
                rect_pos.y += velocity.y * (delta_ticks);
                
                rectangle->SetPosition(rect_pos);
            }
        }
        
        void sync_render_loop(void)
        {
            
            glm::vec2 velocity(0.0f);
            float current_frame = 0.0;
            float last_frame = 0.0;
            float delta_ticks = 0;
            
            while (true)
            {
                
                gl_manager->PollEvents();
                
                current_frame = glfwGetTime();
                delta_ticks = current_frame - last_frame;
                last_frame = current_frame;
                
                velocity.x = 0;
                velocity.y = 0;
            
                if (keyboard->KeyDown(GLFW_KEY_ESCAPE))
                {
                    return;
                }
                if (keyboard->KeyDown(GLFW_KEY_A)) velocity.x -= step_amount;
                if (keyboard->KeyDown(GLFW_KEY_S)) velocity.y += step_amount;
                if (keyboard->KeyDown(GLFW_KEY_D)) velocity.x += step_amount;
                if (keyboard->KeyDown(GLFW_KEY_W)) velocity.y -= step_amount;
                if (keyboard->KeyDown(GLFW_KEY_SPACE))
                {
                    mat->SetAlbedo(Colors::GREY_50);
                }
            
                rect_pos.x += velocity.x * (delta_ticks);
                rect_pos.y += velocity.y * (delta_ticks);
            
                rectangle->SetPosition(rect_pos);
            
                for (unsigned i = 0; i < rectangles.size(); i++)
                {
                    renderer->Queue(rectangles[i]);
                }
                renderer->Draw();
            }
        }
        
        void do_conditional_queue(const std::vector<Model*> &models)
        {
            
            while (!can_access_draw_queue.load())
            {
                //
            }

            can_access_draw_queue.store(false);
            
            for (unsigned i = 0; i < models.size(); i++)
            {
                auto it = already_present.find(models[i]->GetId());
                if (it == already_present.end())
                {
                    already_present[models[i]->GetId()] = true;
                    conditional_queue.push_back(models[i]);
                }
            }
            can_access_draw_queue.store(true);
        }
        
        void do_conditional_clear()
        {
            while (!can_access_draw_queue.load())
            {
                //
            }
            can_access_draw_queue.store(false);
            
            conditional_queue.clear();
            already_present.clear();
            
            can_access_draw_queue.store(true);
        }
        
        //
        //  task stuff
        //
        
        void task_loop(EXP::Task* task)
        {
        }
        
        void on_loop(EXP::State *input)
        {
            should_draw.store(false);
            std::vector<EXP::Model*> elements = { rectangle, rectangle2, rect3 };
            rectangle2->SetDimensions(50.0f, 50.0f);
//            do_conditional_queue(elements);
            do_conditional_queue(rectangles);
            should_draw.store(true);
        }
        
        void on_loop2(EXP::State *input)
        {
            should_draw.store(false);
            std::vector<EXP::Model*> elements = { rectangle2, rectangle, rect3 };
            rectangle2->SetDimensions(50.0f, 25.0f);
//            do_conditional_queue(elements);
            do_conditional_queue(rectangles);
            should_draw.store(true);
        }
        
        
        void on_entry(State *state)
        {
            should_draw.store(false);
            mat->SetAlbedo(Colors::RED);
            should_draw.store(true);
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
            should_draw.store(false);
            mat->SetAlbedo(Colors::GREEN);
            should_draw.store(true);
            rectangle->SetMaterial(mat_tex);
            std::cout << "Entered state 2!" << std::endl;
        }
        
        void on_exit(State *state)
        {
            do_conditional_clear();
            state->Next(state->GetStateById(STATE2));
            std::cout << "Exited state 1!" << std::endl;
        }
        
        void on_exit2(State *state)
        {
            do_conditional_clear();
            rectangle->SetMaterial(mat);
            std::cout << "Exited state 2!" << std::endl;
            state->Next(state->GetStateById(STATE1));
        }
        
        bool custom_exit_condition(State *state)
        {
            return true;
        }
        
        void task_thread_loop(void)
        {
            Task *task = new Task(time);
            State *state1 = task->CreateState(STATE1);
            State *state2 = task->CreateState(STATE2);
            
            state1->ExitOnTimeExceeded();
            
            int duration1 = 398;
            state1->Entry(on_entry);
            state1->Loop(on_loop);
            state1->Exit(on_exit);
            state1->TimeIn(Time::duration_ms(duration1));
            
            int duration2 = 402;
            state2->TimeIn(Time::duration_ms(duration2));
            state2->Entry(on_entry2);
            state2->Loop(on_loop2);
            state2->Exit(on_exit2);
            state2->ExitOnTimeExceeded();
            
            task->AddExitCondition<exit_conditions::key_pressed>(keyboard, GLFW_KEY_ESCAPE);
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
            windows.push_back(gl_manager->OpenWindow(0, nullptr));
//            windows.push_back(gl_manager->OpenWindow(0, 400, 400, windows[0]));
//            windows.push_back(gl_manager->OpenWindow(0, 400, 400, windows[0]));
//            windows.push_back(gl_manager->OpenWindow(0, 400, 400, windows[0]));
            render_target = gl_manager->CreateRenderTarget(windows);
            renderer = new Renderer(render_target);
            mat = resource_manager->CreateMaterial<EXP::MaterialSolid2D>();
            rectangle = resource_manager->CreateModel<EXP::Rectangle>(render_target);
            rectangle2 = resource_manager->CreateModel<EXP::Rectangle>(render_target);
            rect3 = resource_manager->CreateModel<EXP::Rectangle>(render_target);
            tex = resource_manager->GetTexture<EXP::Texture2D>("/Users/Nick/Desktop/eg1.png");
            mat_tex = resource_manager->CreateMaterial<MaterialTexture2D>(tex);
            shader = new Shader2D();
            keyboard = new InputKeyboard(render_target);
            
            renderer->SetClearColor(Colors::WHITE);
            
            rectangle->SetShader(shader);
            rectangle->SetDimensions(100.0f, 200.0f);
            rectangle->SetPosition(Positions2D::CENTER);
            rectangle->SetUnits(Model::MIXED);
            rectangle->SetMaterial(mat);
            
            rectangle2->MakeLike(rectangle);
            rectangle2->SetPosition(Positions2D::LEFT_CENTER);
            rectangle2->SetDimensions(50.0f, 50.0f);
            
            rect3->MakeLike(rectangle);
            rect3->SetPosition(Positions2D::RIGHT_CENTER);
            rect3->SetDimensions(50.0f, 100.0f);
            
            render_target->SetWindowOffsets(RenderTarget::HORIZONTAL);
            
            for (unsigned i = 0; i < 100; ++i)
            {
                Rectangle *new_rect = resource_manager->CreateModel<EXP::Rectangle>(render_target);
                new_rect->MakeLike(rectangle);
                rectangles.push_back(new_rect);
            }
            rectangles.push_back(rectangle);
            
            time->Start();
            
            should_draw.store(false);
            can_access_draw_queue.store(true);
            
            std::thread t1(task_thread_loop);
            render_loop();
            t1.join();
            
            return 0;
        }
        
    }
}

#endif /* test_threaded_states_h */

