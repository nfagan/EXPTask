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

namespace EXP {
    namespace test_case {
        namespace IDS {
            const std::string MAIN_RECT("RECTANGLE");
            const std::string RECT1("rect1");
            const std::string RECT2("rect2");
            const std::string RECT3("rect3");
            const std::string MAT1("mat1");
            const std::string TEX1("tex1");
        }
        
        enum STATES
        {
            STATE1,
            STATE2,
            STATE3
        } states;
        
        Task *task = nullptr;
        Time::Keeper *time = new Time::Keeper();
        GLContextManager *gl_manager = new GLContextManager();
        GLResourceManager *rsrc = new GLResourceManager();
        RenderTarget *render_target = nullptr;
        
        InputKeyboard *keyboard = nullptr;
        InputXY *mouse = nullptr;
        
        BoundsRectangle *bounds_rectangle = nullptr;
        TargetXY *target1 = nullptr;
        
        RenderLoop *render_looper = nullptr;
        glm::vec2 rect_pos = Positions2D::CENTER;
        
        float step_amount = 0.005f;
        unsigned long iterations = 0;
        double avg = 0.0;
        double current = 0.0;
        double last = 0.0;
        double delta = 0.0;
        double min_value = std::numeric_limits<double>::max();
        double max_value = std::numeric_limits<double>::min();
        
        //
        //  task stuff
        //
        
        void render_loop(EXP::RenderLoop* looper)
        {
            gl_manager->PollEvents();
            
            mouse->UpdateCoordinates();
            
            if (keyboard->KeyDown(GLFW_KEY_A)) rect_pos.x -= step_amount;
            if (keyboard->KeyDown(GLFW_KEY_S)) rect_pos.y += step_amount;
            if (keyboard->KeyDown(GLFW_KEY_D)) rect_pos.x += step_amount;
            if (keyboard->KeyDown(GLFW_KEY_W)) rect_pos.y -= step_amount;
            if (keyboard->KeyDown(GLFW_KEY_C))
            {
                rect_pos.x = 0.5f;
                rect_pos.y = 0.5f;
            }
            
            Rectangle *rectangle = rsrc->Get<Rectangle>("RECTANGLE");
            
            rectangle->SetPosition(rect_pos);
        }
        
        void summarize_task(double ellapsed, int expected)
        {
            std::cout << "\nEllapsed time: " << ellapsed << " (s)" << std::endl;
            std::cout << task->ExitReason();
            std::cout << "\nN iterations: " << iterations;
            std::cout << "\nAverage trial-time: " << avg * 1000.0f << " (ms)";
            std::cout << "\nExpected trial-time: " << expected << " (ms)";
            std::cout << "\nMax trial-time: " << max_value * 1000.0f << " (ms)";
            std::cout << "\nMin trial-time: " << min_value * 1000.0f << " (ms)" << std::endl;
        }
        
        void update_state_stats()
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
        }
        
        void task_thread_loop(void)
        {
            task = new Task(time);
            State *state1 = task->CreateState(STATE1);
            State *state2 = task->CreateState(STATE2);
            State *state3 = task->CreateState(STATE3);
            
            //
            //  state 1
            //
            
            int duration1 = 650;
            bool was_in_bounds;
            state1->OnEntry([&] (State* state) {
                std::cout << "Entering state 1!" << std::endl;
                was_in_bounds = false;
                update_state_stats();
                render_looper->OnceDrawReady([] (RenderLoop *looper) {
                    looper->ClearQueue();
                    std::vector<Model*> elements =  rsrc->GetByTag<Model>(IDS::RECT3);
                    Rectangle *rect =               rsrc->Get<Rectangle>(IDS::MAIN_RECT);
                    MaterialSolid2D *mat =          rsrc->Get<MaterialSolid2D>(IDS::MAT1);
                    mat->SetAlbedo(Colors::RED);
                    elements.push_back(rect);
                    for (unsigned i = 0; i < elements.size(); ++i) elements[i]->SetMaterial(mat);
                    looper->Queue(elements);
                });
            });
            
            state1->OnLoop([&] (State* state) {
                Rectangle *rectangle = rsrc->Get<Rectangle>(IDS::MAIN_RECT);
                Rect<float> pixel_verts = rectangle->GetPixelVertices(render_target->GetFullRect());
                bounds_rectangle->SetBounds(pixel_verts);
                bool in_bounds = target1->InBounds();
                bool state_changed;
                std::function<void(RenderLoop*)> func;
                if (in_bounds)
                {
                    func = [] (RenderLoop *looper) {
                        MaterialSolid2D *mat = rsrc->Get<MaterialSolid2D>(IDS::MAT1);
                        mat->SetAlbedo(Colors::GREY_50);
                    };
                    state_changed = !was_in_bounds;
                    was_in_bounds = true;
                }
                else
                {
                    func = [] (RenderLoop *looper) {
                        MaterialSolid2D *mat = rsrc->Get<MaterialSolid2D>(IDS::MAT1);
                        mat->SetAlbedo(Colors::RED);
                    };
                    state_changed = was_in_bounds;
                    was_in_bounds = false;
                }
                
                if (state_changed)
                {
                    render_looper->OnceDrawReady(func);
                }
            });
            state1->OnExit([] (State* state) {
                state->Next(state->GetStateById(STATE2));
            });
            state1->SetTimeIn(Time::duration_ms(duration1));
            state1->ExitOnTimeExceeded();
            
            //
            //  state 2
            //
            
            int duration2 = 650;
            state2->SetTimeIn(Time::duration_ms(duration2));
            state2->OnEntry([] (State* state) {
                std::cout << "Entering state 2!" << std::endl;
                render_looper->OnceDrawReady([] (RenderLoop *looper) {
                    looper->ClearQueue();
                    std::vector<Model*> elements = rsrc->GetByTag<Model>(IDS::RECT3);
                    MaterialSolid2D *mat = rsrc->Get<MaterialSolid2D>(IDS::MAT1);
                    Rectangle *rect = rsrc->Get<Rectangle>(IDS::MAIN_RECT);
                    mat->SetAlbedo(Colors::WHITE);
                    rect->SetMaterial(mat);
                    elements.push_back(rect);
                    looper->Queue(elements);
                });
            });
            state2->OnExit([] (State* state) {
                state->Next(state->GetStateById(STATE3));
            });
            state2->ExitOnTimeExceeded();
            
            //
            //  state 3
            //
            
            int duration3 = 1000;
            state3->SetTimeIn(Time::duration_ms(duration3));
            state3->OnEntry([] (State* state) {
                std::cout << "Entering state 3!" << std::endl;
                render_looper->OnceDrawReady([] (RenderLoop *looper) {
                    looper->ClearQueue();
                    std::vector<Model*> elements = rsrc->GetByTag<Model>(IDS::RECT3);
                    MaterialSolid2D *mat =  rsrc->Get<MaterialSolid2D>(IDS::MAT1);
                    Rectangle *rect =       rsrc->Get<Rectangle>(IDS::MAIN_RECT);
                    mat->SetAlbedo(Colors::MAGENTA);
                    rect->SetMaterial(mat);
                    elements.push_back(rect);
                    looper->Queue(elements);
                });
            });
            state3->OnExit([] (State* state) {
                state->Next(state->GetStateById(STATE1));
            });
            state3->ExitOnTimeExceeded();
            
            //
            //  task
            //
            
            task->OnLoop([] (EXP::Task* task) {
                 if (keyboard->KeyDown(GLFW_KEY_SPACE))
                 {
                     render_looper->OnceDrawReady([&] (EXP::RenderLoop* looper) {
                         MaterialSolid2D *mat = rsrc->Get<MaterialSolid2D>(IDS::MAT1);
                         mat->SetAlbedo(Colors::GREY_50);
                     });
                 }
            });
            task->OnExit([] (Task* task) {
                render_looper->CancelLoop();
            });
            
            task->ExitOnKeyPress(keyboard, GLFW_KEY_ESCAPE);
            
            task->Next(state1);
            task->GetTimer()->Reset();
            task->Run();
            summarize_task(task->EllapsedTime().count(), duration1+duration2+duration3);
        }
        
        void gl_init(void)
        {
            using namespace EXP;
            
            std::vector<Window*> windows;
            windows.push_back(gl_manager->OpenWindow(0, 400, 400, nullptr));
            render_target = gl_manager->CreateRenderTarget(windows);
            Renderer *renderer = new Renderer(render_target);
            render_looper = new RenderLoop(renderer);
            Texture2D *tex = rsrc->GetTexture<Texture2D>("/Users/Nick/Desktop/eg1.png");
            MaterialSolid2D *mat = rsrc->Create<MaterialSolid2D>(render_target);
            MaterialTexture2D *mat_tex = rsrc->Create<MaterialTexture2D>(render_target, tex);
            Rectangle *rectangle = rsrc->Create<Rectangle>(render_target);
            Shader *shader = new Shader2D();
            keyboard = new InputKeyboard(render_target);
            mouse = new InputXY(render_target);
            
            bounds_rectangle = new BoundsRectangle(rectangle->GetPixelVertices(render_target->GetFullRect()));
            target1 = new TargetXY(bounds_rectangle, mouse);
            
            rectangle->SetShader(shader);
            rectangle->SetUnits(Model::MIXED);
            rectangle->SetDimensions(100.0f, 100.0f);
            
            rsrc->SetName(mat, IDS::MAT1);
            rsrc->SetName(mat_tex, IDS::TEX1);
            rsrc->SetName(rectangle, IDS::MAIN_RECT);
            
            renderer->SetClearColor(Colors::BLACK);
            render_target->SetWindowOffsets(RenderTarget::HORIZONTAL);
            
            for (unsigned i = 0; i < 1000; ++i)
            {
                Rectangle *new_rect = rsrc->Create<Rectangle>(render_target);
                new_rect->MakeLike(rectangle);
                new_rect->SetDimensions(10.0f, 10.0f);
                float x = (float) rand()/RAND_MAX;
                float y = (float) rand()/RAND_MAX;
                new_rect->SetPosition(glm::vec2(x, y));
                if (i < 333)
                {
                    new_rect->SetTag(IDS::RECT1);
                }
                else if (i < 666)
                {
                    new_rect->SetTag(IDS::RECT2);
                }
                else
                {
                    new_rect->SetTag(IDS::RECT3);
                }
            }
        }
        
        
        int test_states(void)
        {
            using namespace EXP;
            
            time->Start();
            
            gl_init();
            
            std::thread t1(task_thread_loop);
            
            render_looper->OnLoop(render_loop);
            
            render_looper->Loop();
            
            t1.join();
            
            return 0;
        }
        
    }
}

#endif /* test_threaded_states_h */


