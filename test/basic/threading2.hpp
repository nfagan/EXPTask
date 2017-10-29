//
//  basic.hpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef threading2_hpp
#define threading2_hpp

#include <stdio.h>
#include <EXP.hpp>
#include <thread>

//#define EXPGL__USE_MUTEX;

namespace EXP {
namespace threading2 {
    
    void move_rect(const EXP::InputKeyboard *keyboard, glm::vec2 &rect_pos, float step_amount);
    void work();
    
    using namespace EXP;
    
    GLContextManager *context_manager = new GLContextManager();
    ResourceManager *resource_manager = new ResourceManager();
    
    std::vector<Window*> windows;
    RenderTarget *target;
    Renderer *renderer;
    Rectangle *rectangle;
    Image *image;
    MaterialSolid2D *mat;
    MaterialTexture2D *mat2;
    Texture2D *tex;
    InputXY *mouse;
    InputKeyboard *keyboard;
    TargetXY *fix_targ;
    bool draw_ready = false;
    bool queue_ready = true;
    std::mutex mutex;
    
    void init()
    {
        windows.push_back(context_manager->OpenWindow(0, 400, 400, NULL));
        windows.push_back(context_manager->OpenWindow(0, 400, 400, windows[0]));
        target = context_manager->CreateRenderTarget(windows);
        renderer = new Renderer(target);
        rectangle = resource_manager->CreateModel<EXP::Rectangle>(target);
        mat = resource_manager->CreateMaterial<EXP::MaterialSolid2D>();
        tex = resource_manager->GetTexture<Texture2D>("/Users/Nick/Desktop/eg.png");
        mat2 = resource_manager->CreateMaterial<EXP::MaterialTexture2D>(tex);
        image = resource_manager->CreateModel<EXP::Image>(target, mat2);
        mouse = new InputXY(target);
        keyboard = new InputKeyboard(target);
    }
    
    void work()
    {
        while (!keyboard->KeyDown(GLFW_KEY_ESCAPE))
        {
#ifndef EXPGL__USE_MUTEX
            if (queue_ready)
            {
//                printf("\nQueueing ...");
                renderer->Queue(image);
                queue_ready = false;
                draw_ready = true;
            }
#else
            mutex.lock();
            renderer->Queue(rectangle);
            mutex.unlock();
#endif
            
        }
    }

    int test_case()
    {
        init();
        
        if (!context_manager->DidInitialize())
        {
            fprintf(stderr, "Error initializing");
            delete resource_manager;
            delete context_manager;
            return EXIT_FAILURE;
        }
        
        target->SetWindowOffsets(RenderTarget::HORIZONTAL);
        renderer->SetClearColor(Colors::GREY_50);
        
        glm::vec2 rect_pos = Positions2D::CENTER;
        float step_amount = 0.005f;
        
        rectangle->SetUnits(Model::MIXED);
        rectangle->SetDimensions(100.0f, 100.0f);
        rectangle->SetPosition(rect_pos);
        mat->SetAlbedo(Colors::BLUE);
        rectangle->SetMaterial(mat);
        
        image->SetPosition(rect_pos);
        image->SetUnits(Model::MIXED);
        image->SetScale(100.0f);
        
        target->GetPrimaryWindow()->Focus();
        
        GLFrameTimer frame_timer;
        
        std::thread thread1(work);
        
        Rect<float> bounds_rect(0.0f, 0.0f, 100.0f, 100.0f);
        BoundsRectangle *bounds = new EXP::BoundsRectangle(bounds_rect);
        fix_targ = new TargetXY(bounds, mouse);
        
        while (true)
        {
            frame_timer.Update();
            
#ifndef EXPGL__USE_MUTEX
            if (draw_ready) {
                renderer->Draw();
                draw_ready = false;
                queue_ready = true;
            }
#else
            mutex.lock();
            renderer->Draw();
            mutex.unlock();
#endif
            
            if (keyboard->KeyDown(GLFW_KEY_ESCAPE))
            {
                context_manager->CloseWindow(windows[0]);
                break;
            }
            if (keyboard->KeyDown(GLFW_KEY_J))
            {
                image->SetScale(200.0f);
                image->SetMaterial(mat);
            }
            else if (keyboard->KeyDown(GLFW_KEY_K))
            {
                image->SetScale(100.0f);
                image->SetMaterial(mat2);
            }
            
            move_rect(keyboard, rect_pos, step_amount);
            
            rectangle->SetPosition(rect_pos);
            image->SetPosition(rect_pos);
            
            mouse->UpdateCoordinates();
//            mouse->PrintCoordinates();
            
            if (fix_targ->InBounds())
                printf("\nIn bounds!");
            else
                printf("\nNot in bounds!");
            
            bounds->SetBounds(image->GetPixelVertices(target->GetFullRect()));
            
            glfwPollEvents();
        }
        
        thread1.join();
    
        frame_timer.Summarize();
    
        delete context_manager;
        delete resource_manager;
        delete renderer;
    
        return 0;
    }
    
    void move_rect(const EXP::InputKeyboard *keyboard, glm::vec2 &rect_pos, float step_amount)
    {
        if (keyboard->KeyDown(GLFW_KEY_A)) rect_pos.x -= step_amount;
        if (keyboard->KeyDown(GLFW_KEY_D)) rect_pos.x += step_amount;
        if (keyboard->KeyDown(GLFW_KEY_W)) rect_pos.y -= step_amount;
        if (keyboard->KeyDown(GLFW_KEY_S)) rect_pos.y += step_amount;
        if (keyboard->KeyDown(GLFW_KEY_C))
        {
            rect_pos.x = 0.5f;
            rect_pos.y = 0.5f;
        }
        if (keyboard->KeyDown(GLFW_KEY_V))
        {
            rect_pos.x = 0.333333f;
            rect_pos.y = 0.5f;
        }
        if (keyboard->KeyDown(GLFW_KEY_B))
        {
            rect_pos.x = 0.666666f;
            rect_pos.y = 0.5f;
        }
    }
    
}
}

#endif /* basic_hpp */

