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
#include <EXPUtil/data/commitable.hpp>
#include <iostream>
#include <thread>

namespace EXP {
namespace test_case {
    
namespace IDS {
#ifdef __APPLE__
    const std::string TEX_PATH("/Users/Nick/Desktop/eg1.png");
#else
	const std::string TEX_PATH("C:\\Users\\changLab\\Desktop\\blue.jpg");
#endif
    const std::string MAIN_RECT("RECTANGLE");
    const std::string MAIN_RECT2("RECTANGLE2");
    const std::string CIRCLE1("circle1");
    const std::string RECT1("rect1");
    const std::string RECT2("rect2");
    const std::string RECT3("rect3");
    const std::string MAT1("mat1");
    const std::string MAT_ANON("mat_anon");
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
BoundsRectangle *bounds_rect2 = nullptr;
TargetXY *target1 = nullptr;
TargetXY *target2 = nullptr;

RenderLoop *render_looper = nullptr;
glm::vec2 rect_pos = Positions2D::CENTER;

//
//  task stuff
//

void render_loop(EXP::RenderLoop* looper)
{
    gl_manager->PollEvents();
    
    mouse->UpdateCoordinates();
    
    float step_amount = 0.005f;
    if (keyboard->KeyDown(GLFW_KEY_A)) rect_pos.x -= step_amount;
    if (keyboard->KeyDown(GLFW_KEY_S)) rect_pos.y += step_amount;
    if (keyboard->KeyDown(GLFW_KEY_D)) rect_pos.x += step_amount;
    if (keyboard->KeyDown(GLFW_KEY_W)) rect_pos.y -= step_amount;
    if (keyboard->KeyDown(GLFW_KEY_C))
    {
        rect_pos.x = 0.5f;
        rect_pos.y = 0.5f;
    }
    
    Model *rectangle = rsrc->Get<Model>(IDS::MAIN_RECT);
    Model *circle = rsrc->Get<Model>(IDS::CIRCLE1);
    
    rectangle->SetPosition(rect_pos);
    circle->SetPosition(rect_pos);
}

Rect<float> get_pixel_vertices(RenderTarget *target, Model* model)
{
    Rect<float> screen = static_cast<Rect<float>>(render_target->GetFullRect());
    glm::vec3 pos = model->get_units_position(screen);
    glm::vec3 scl = model->get_units_scale(screen);
    
    float left = pos.x - scl.x;
    float right = pos.x + scl.x;
    float top = pos.y - scl.y;
    float bottom = pos.y + scl.y;
    
    return Rect<float>(left, top, right, bottom);
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
    
    int duration1 = 3000;
    
    state1->SetName("state 1");
    
    state1->OnEntry([&] (State* state) {
        std::cout << "Entering state 1!" << std::endl;
        state->LogTime();
        render_looper->OnceDrawReady([] (RenderLoop *looper) {
            looper->ClearQueue();
            std::vector<Model*> elements =  rsrc->GetByTag<Model>(IDS::RECT3);
            Model *rect = rsrc->Get<Model>(IDS::MAIN_RECT);
            Model *rect2 = rsrc->Get<Model>(IDS::MAIN_RECT2);
            Model *circle = rsrc->Get<Model>(IDS::CIRCLE1);
            Material *mat = rsrc->Get<Material>(IDS::MAT1);
            for (unsigned i = 0; i < elements.size(); ++i) elements[i]->SetMaterial(mat);
            elements.push_back(rect);
            elements.push_back(rect2);
            elements.push_back(circle);
            looper->Queue(elements);
        });
    });
    
    state1->OnLoop([&] (State* state) {
        Model *rectangle = rsrc->Get<Model>(IDS::MAIN_RECT);
        Model *rect2 = rsrc->Get<Model>(IDS::MAIN_RECT2);
        Rect<float> pixel_verts = get_pixel_vertices(render_target, rectangle);
        bounds_rectangle->SetBounds(pixel_verts);
        pixel_verts = get_pixel_vertices(render_target, rect2);
        bounds_rect2->SetBounds(pixel_verts);
    });
    state1->OnExit([] (State* state) {
        if (!state->GetNext())
        {
            state->Next(state->GetStateById(STATE1));
        }
    });
    state1->SetTimeIn(Time::duration_ms(duration1));
    state1->ExitOnTimeExceeded();
    
    TargetSet &target_set = state1->GetTargetSet();
    target_set.Add(target1, Time::duration_ms(1000));
    target_set.Add(target2, Time::duration_ms(500));
    
    target_set.OnEllapsed([] (State *state, TargetXY *target) {
        render_looper->OnceDrawReady([] (RenderLoop *looper) {
            Material *mat = rsrc->Get<Material>(IDS::MAT1);
            Texture *tex = rsrc->GetTexture<Texture>(IDS::TEX_PATH.c_str());
            mat->SetAlbedo(tex);
        });
    });
    target_set.OnTargetEntry([] (State *state, TargetXY *target) {
        unsigned id = target->GetId();
        render_looper->OnceDrawReady([id] (RenderLoop *looper) {
            Model *rect = rsrc->Get<Model>(IDS::MAIN_RECT);
            Material *mat = rsrc->Get<Material>(IDS::MAT1);
            Material *tex = rsrc->Get<Material>(IDS::TEX1);
            if (id == 0)
            {
                mat->SetAlbedo(Colors::GREY_50);
                rect->SetMaterial(tex);
            }
            else
            {
                mat->SetAlbedo(Colors::MAGENTA);
            }
        });
    });
    target_set.OnTargetExit([] (State *state, TargetXY *target) {
        unsigned id = target->GetId();
        render_looper->OnceDrawReady([id] (RenderLoop *looper) {
            Model *rect = rsrc->Get<Model>(IDS::MAIN_RECT);
            Material *mat = rsrc->Get<Material>(IDS::MAT1);
            mat->SetAlbedo(Colors::RED);
            rect->SetMaterial(mat);
        });
    });
    
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
            Material *mat = rsrc->Get<Material>(IDS::MAT1);
            Model *rect = rsrc->Get<Model>(IDS::MAIN_RECT);
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
            Material *mat = rsrc->Get<Material>(IDS::MAT_ANON);
            mat->SetAlbedo(Colors::GREEN);
            Model *rect = rsrc->Get<Model>(IDS::MAIN_RECT);
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
    
    task->SetName("Task 1");
    task->OnExit([] (Task* task) {
        render_looper->CancelLoop();
    });
    
    task->ExitOnKeyPress(keyboard, GLFW_KEY_ESCAPE);
    task->Next(state1);
    task->GetTimer()->Reset();
    task->Run();
    task->LogTime();
}

void gl_init(void)
{
    using namespace EXP;
    
    std::vector<Window*> windows;
    windows.push_back(gl_manager->OpenWindow(0, 400, 400, nullptr));
    render_target = gl_manager->CreateRenderTarget(windows);
    Renderer *renderer = new Renderer(render_target);
    render_looper = new RenderLoop(renderer);
    
    //  render init
    renderer->SetClearColor(Colors::BLACK);
    render_target->SetWindowOffsets(RenderTarget::HORIZONTAL);
    
    //  materials
    Texture *tex = rsrc->GetTexture<Texture>(IDS::TEX_PATH.c_str());
    
    Material *mat_anon = rsrc->Create<Material>(render_target);
    Material *mat = rsrc->Create<Material>(render_target);
    Material *mat_tex = rsrc->Create<Material>(render_target);
    mat->SetAlbedo(Colors::RED);
    
    //  rectangle
    Model *rectangle = rsrc->CreateRectangle(render_target);
    Model *rectangle2 = rsrc->CreateRectangle(render_target);
    Model *circle = rsrc->CreateSphere(render_target);
    
    //  globals
    Shader *shader = new Shader2D();
    keyboard = new InputKeyboard(render_target);
    mouse = new InputXY(render_target);
    
    glm::vec2 aspect = glm::vec2(1.0f, (float)tex->GetWidth() / (float)tex->GetHeight());
    
    rectangle->SetShader(shader);
    rectangle->SetUnits(Model::MIXED);
    rectangle->SetScale(50.0f);
    rectangle->SetMaterial(mat);
    rectangle->SetPosition(rect_pos);
    bounds_rectangle = new BoundsRectangle(get_pixel_vertices(render_target, rectangle));
    target1 = new TargetXY(bounds_rectangle, mouse);
    
    rectangle2->MakeLike(rectangle);
    rectangle2->SetPosition(Positions2D::RIGHT_CENTER);
    rectangle2->SetMaterial(mat_tex);
    bounds_rect2 = new BoundsRectangle(get_pixel_vertices(render_target, rectangle2));
    target2 = new TargetXY(bounds_rect2, mouse);
    target2->SetId(1);
    
    circle->SetShader(shader);
    circle->SetPosition(Positions2D::CENTER);
    circle->SetUnits(Model::MIXED);
    circle->SetScale(50.0f);
    circle->GetMaterial()->SetAlbedo(Colors::WHITE);
    
    rsrc->SetName(rectangle2, IDS::MAIN_RECT2);
    rsrc->SetName(mat, IDS::MAT1);
    rsrc->SetName(mat_anon, IDS::MAT_ANON);
    rsrc->SetName(rectangle, IDS::MAIN_RECT);
    rsrc->SetName(mat_tex, IDS::TEX1);
    rsrc->SetName(circle, IDS::CIRCLE1);
    
    for (unsigned i = 0; i < 50; ++i)
    {
        Model *new_rect = rsrc->CreateRectangle(render_target);
        new_rect->MakeLike(rectangle);
        new_rect->SetScale(glm::vec2(10.0f, 10.0f));
        float x = (float) rand()/RAND_MAX;
        float y = (float) rand()/RAND_MAX;
        new_rect->SetPosition(glm::vec2(x, y));
        new_rect->SetTag(IDS::RECT3);
    }
}


int test_states(void)
{
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


