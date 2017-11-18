//
//  threading1.hpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef threading1_h
#define threading1_h

#include <EXP.hpp>
#include <iostream>
#include <thread>
#include <chrono>

std::timed_mutex mutex;

void work(EXP::InputKeyboard &keyboard, EXP::Renderer *renderer, EXP::Image *image) {
    std::chrono::milliseconds timeout(100);
    
    while(!keyboard.KeyDown(GLFW_KEY_ESCAPE)){
        if(mutex.try_lock_for(timeout)){
            std::cout << std::this_thread::get_id() << ": do work with the mutex" << std::endl;
            
            renderer->Queue(image);
//            renderer->Draw();
            
            std::chrono::milliseconds sleepDuration(250);
            std::this_thread::sleep_for(sleepDuration);
            
            mutex.unlock();
            
            std::this_thread::sleep_for(sleepDuration);
        } else {
            std::cout << std::this_thread::get_id() << ": do work without mutex" << std::endl;
            
            std::chrono::milliseconds sleepDuration(100);
            std::this_thread::sleep_for(sleepDuration);
        }
    }
}

int test_case()
{
    using namespace EXP;
    
    GLContextManager *context_manager = new GLContextManager();
    ResourceManager *resource_manager = new ResourceManager();
    
    if (!context_manager->DidInitialize())
    {
        fprintf(stderr, "Error initializing");
        delete resource_manager;
        delete context_manager;
        return EXIT_FAILURE;
    }
    
    std::vector<Window*> windows;
    
    windows.push_back(context_manager->OpenWindow(0, 400, 400, NULL));
    
    RenderTarget *target = context_manager->CreateRenderTarget(windows);
    target->SetWindowOffsets(RenderTarget::HORIZONTAL);
    
    Renderer *renderer = new Renderer(target);
    renderer->SetClearColor(Colors::GREY_50);
    
    Texture2D *tex = resource_manager->GetTexture<Texture2D>("/Users/Nick/Desktop/eg.png");
    EXP::MaterialTexture2D *mat2 = resource_manager->CreateMaterial<EXP::MaterialTexture2D>(tex);
    EXP::Image *image = resource_manager->CreateModel<EXP::Image>(target, mat2);
    
    InputXY mouse(target);
    InputKeyboard keyboard(target);
    
    GLFrameTimer frame_timer;
    
    std::vector<std::thread> threads;
    std::timed_mutex mut1;
    for(int i = 0; i < 5; ++i){
        threads.push_back(std::thread(work, keyboard, renderer, target));
    }
    
    while (true)
    {
        if (keyboard.KeyDown(GLFW_KEY_ESCAPE))
        {
            context_manager->CloseWindow(windows[0]);
            break;
        }
        
//        if (mut1.try_lock()) {
//            printf("\nSuccess from main");
//            renderer->Draw();
//        }
//        mut1.unlock();
        
        frame_timer.Update();
        
        mouse.UpdateCoordinates();
//        mouse.PrintCoordinates();
        
        glfwPollEvents();
    }
    
    for(auto& thread : threads){
        thread.join();
    }
    
    frame_timer.Summarize();
    
    delete context_manager;
    delete resource_manager;
    delete renderer;
    
    return 0;
}


#endif /* threading1_h */
