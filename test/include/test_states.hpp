//
//  test_states.hpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef test_states_h
#define test_states_h

#include <EXP.hpp>
#include <EXPGL.hpp>
#include <iostream>

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
        
        void task_loop(EXP::State* input)
        {
            if (keyboard->KeyDown(GLFW_KEY_A)) rect_pos.x -= step_amount;
            if (keyboard->KeyDown(GLFW_KEY_S)) rect_pos.y += step_amount;
            if (keyboard->KeyDown(GLFW_KEY_D)) rect_pos.x += step_amount;
            if (keyboard->KeyDown(GLFW_KEY_W)) rect_pos.y -= step_amount;
            if (keyboard->KeyDown(GLFW_KEY_C))
            {
                rect_pos.x = 0.5f;
                rect_pos.y = 0.5f;
            }
            
            rectangle->SetPosition(rect_pos);
            gl_manager->PollEvents();
        }
        
        void on_loop(EXP::State *input)
        {
            mat->SetAlbedo(Colors::RED);
            if (do_draw)
            {
                renderer->Queue(rectangle);
                renderer->Draw();
            }
        }
        
        void on_loop2(EXP::State *input)
        {
            mat->SetAlbedo(Colors::GREEN);
            rectangle->SetDimensions(50.0f, 50.0f);
            if (do_draw)
            {
                renderer->Queue(rectangle);
                renderer->Draw();
            }
        }
        
        void on_loop3(State *input)
        {
            mat->SetAlbedo(Colors::GREY_50);
            if (do_draw)
            {
                renderer->Queue(rectangle);
                renderer->Draw();
            }
            if (keyboard->KeyDown(GLFW_KEY_SPACE))
            {
                input->Next(input->GetStateById(STATE2));
                input->AddTransientExitCondition<exit_conditions::now>();
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
            rect_pos = Positions2D::LEFT_CENTER;
            std::cout << "Entered state 2!" << std::endl;
        }
        
        void on_entry3(State *state)
        {
            std::cout << "Entered state 3!" << std::endl;
        }
        
        void on_exit(State *state)
        {
            state->Next(state->GetStateById(STATE2));
            std::cout << "Exited state 1!" << std::endl;
        }
        
        void on_exit2(State *state)
        {
            rect_pos = Positions2D::CENTER;
            rectangle->SetDimensions(100.0f, 200.0f);
            std::cout << "Exited state 2!" << std::endl;
            state->Next(state->GetStateById(STATE3));
        }
        
        void on_exit3(State *state)
        {
            std::cout << "Exited state 3!" << std::endl;
            state->Next(state->GetStateById(STATE1));
        }
        
        int test_states(void)
        {
            using namespace EXP;
            
            gl_manager = new GLContextManager();
            resource_manager = new ResourceManager();
            windows.push_back(gl_manager->OpenWindow(0, nullptr));
            render_target = gl_manager->CreateRenderTarget(windows);
            renderer = new Renderer(render_target);
            shader = new Shader2D();
            mat = resource_manager->CreateMaterial<EXP::MaterialSolid2D>();
            rectangle = resource_manager->CreateModel<EXP::Rectangle>(render_target);
            keyboard = new InputKeyboard(render_target);
            
            renderer->SetClearColor(Colors::WHITE);
            
            rectangle->SetShader(shader);
            rectangle->SetDimensions(100.0f, 200.0f);
            rectangle->SetPosition(glm::vec2(0.5f, 0.5f));
            rectangle->SetUnits(Model::MIXED);
            rectangle->SetMaterial(mat);
            
            render_target->SetWindowOffsets(RenderTarget::HORIZONTAL);
            
            time->Start();

            Task *task = new Task(time);
            State *state1 = task->CreateState(STATE1);
            State *state2 = task->CreateState(STATE2);
            State *state3 = task->CreateState(STATE3);
            
            state1->AddExitCondition<exit_conditions::time_exceeded>();
            
            state1->Entry(on_entry);
            state1->Loop(on_loop);
            state1->Exit(on_exit);
            state1->TimeIn(Time::duration_ms(250));
            
            state2->TimeIn(Time::duration_ms(500));
            state2->Entry(on_entry2);
            state2->Loop(on_loop2);
            state2->Exit(on_exit2);
            state2->AddExitCondition<exit_conditions::time_exceeded>();
            
            state3->TimeIn(Time::duration_ms(2000));
            state3->Entry(on_entry3);
            state3->Loop(on_loop3);
            state3->Exit(on_exit3);
            state3->AddExitCondition<exit_conditions::time_exceeded>();
            
            task->AddExitCondition<exit_conditions::key_pressed>(keyboard, GLFW_KEY_ESCAPE);
            task->Loop(task_loop);
            task->Next(state1);
            
            double start = time->Now();
            task->Run();
            double stop = time->Now();
            std::cout << "\nEllapsed time: " << (stop - start) << "s\n" << std::endl;
            std::cout << task->ExitReason() << std::endl;
            std::cout << "\nAverage trial-time: " << avg << "\n";
            std::cout << "\nMax trial-time: " << max_value << "\n";
            std::cout << "\nMin trial-time: " << min_value << std::endl;
            
            delete gl_manager;
            delete keyboard;
            delete task;
            delete time;
            
            return 0;
        }
        
    }
}

#endif /* test_states_h */
