#include "pch.h"
//#include "OpenGLContext.h"
//#include "SDL/SDL.h"
//#include "glad/glad.h"
//
//namespace
//{
//    struct ContextImpl
//    {
//        ContextImpl(SDL_Window* window) 
//            : window(window)
//        {
//
//        }
//        SDL_Window* window;
//        SDL_GLContext context = nullptr;
//    };
//}
//
//namespace Aura
//{
//    OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
//        : impl(new ContextImpl(windowHandle))
//    {
//
//    }
//
//    void OpenGLContext::Init()
//    {
//        SDL_GL_LoadLibrary(NULL);
//
//        //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
//        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
//        //// Also request a depth buffer
//        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//
//        impl->context = SDL_GL_CreateContext(impl->window);
//        if (!impl->context)
//        {
//            printf("failed to create context: %s\n", SDL_GetError());
//        }
//        SDL_GL_MakeCurrent(impl->window, impl->context);
//        SDL_GL_SetSwapInterval(1);
//
//        printf("OpenGL loaded\n");
//        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
//            printf("failed to load glad\n");
//        printf("Vendor:   %s\n", glGetString(GL_VENDOR));
//        printf("Renderer: %s\n", glGetString(GL_RENDERER));
//        printf("Version:  %s\n", glGetString(GL_VERSION));
//    }
//
//    void OpenGLContext::SwapBuffers()
//    {
//        SDL_GL_SwapWindow(impl->window);
//    }
//}