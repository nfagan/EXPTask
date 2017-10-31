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
        
        GLContextManager *gl_manager = nullptr;
        ResourceManager *resource_manager = nullptr;
        Window *window = nullptr;
        RenderTarget *render_target = nullptr;
        Renderer *renderer = nullptr;
        InputKeyboard *keyboard = nullptr;
        Rectangle *rectangle = nullptr;
        MaterialSolid2D *mat = nullptr;
        glm::vec2 rect_pos = Positions2D::CENTER;
        float step_amount = 0.005f;
        
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
            renderer->Draw();
            gl_manager->PollEvents();
        }
        
        void on_loop(EXP::State *input)
        {
            mat->SetAlbedo(Colors::RED);
            renderer->Queue(rectangle);
        }
        
        void on_loop2(EXP::State *input)
        {
            mat->SetAlbedo(Colors::GREEN);
            renderer->Queue(rectangle);
        }
        
        void on_loop3(State *input)
        {
            mat->SetAlbedo(Colors::GREY_50);
            renderer->Queue(rectangle);
            if (keyboard->KeyDown(GLFW_KEY_SPACE))
            {
                input->Next(input->GetStateById(STATE2));
                input->AddTransientExitCondition<exit_conditions::now>();
            }
        }
        
        void on_entry(State *state)
        {
            std::cout << "Entered state 1!" << std::endl;
        }
        
        void on_entry2(State *state)
        {
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
            window = gl_manager->OpenWindow(0, nullptr);
            render_target = gl_manager->CreateRenderTarget(window);
            renderer = new Renderer(render_target);
            mat = resource_manager->CreateMaterial<EXP::MaterialSolid2D>();
            rectangle = resource_manager->CreateModel<EXP::Rectangle>(render_target);
            keyboard = new InputKeyboard(render_target);
            
            rectangle->SetDimensions(100.0f, 200.0f);
            rectangle->SetPosition(glm::vec2(0.5f, 0.5f));
            rectangle->SetUnits(Model::MIXED);
            rectangle->SetMaterial(mat);
            
            render_target->SetWindowOffsets(RenderTarget::HORIZONTAL);
            
            Time::Keeper *time = new Time::Keeper();
            time->Start();

            Task *task = new Task(time);
            State *state = task->CreateState(STATE1);
            State *state2 = task->CreateState(STATE2);
            State *state3 = task->CreateState(STATE3);
            
            state->AddExitCondition<exit_conditions::time_exceeded>();
            
            state->Entry(on_entry);
            state->Loop(on_loop);
            state->Exit(on_exit);
            state->TimeIn(Time::duration_ms(1000));
            
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
            task->Next(state);
            
            double start = time->Now();
            task->Run();
            double stop = time->Now();
            std::cout << (stop - start) << std::endl;
            std::cout << task->ExitReason() << std::endl;
            
            delete gl_manager;
            delete keyboard;
            delete task;
            delete time;
            
            return 0;
        }
        
    }
}

#endif /* test_states_h */
