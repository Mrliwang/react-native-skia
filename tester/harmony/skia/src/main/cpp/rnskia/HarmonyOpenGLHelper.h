#ifndef SKIAHARMONY_SKIAOPENGLHELPER_H
#define SKIAHARMONY_SKIAOPENGLHELPER_H

#pragma once


#include <EGL/eglplatform.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>


#include <atomic>
#include <glog/logging.h>
#include "native_window/external_window.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include "include/gpu/GrDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"


#pragma clang diagnostic pop

namespace RNSkia {
// using GetPlatformDisplayExt = PFNEGLGETPLATFORMDISPLAYEXTPROC;
// constexpr const char *EGL_EXT_PLATFORM_WAYLAND = "EGL_EXT_platform_wayland";
// constexpr const char *EGL_KHR_PLATFORM_WAYLAND = "EGL_KHR_platform_wayland";
// constexpr int32_t EGL_CONTEXT_CLIENT_VERSION_NUM = 2;
// constexpr char CHARACTER_WHITESPACE = ' ';
// constexpr const char *CHARACTER_STRING_WHITESPACE = " ";
// constexpr const char *EGL_GET_PLATFORM_DISPLAY_EXT = "eglGetPlatformDisplayEXT";
// constexpr int32_t NATIVE_CACHE_BUFFER = 3;

/**
 * Singleton holding the default display and shared eglContext that will be the
 * first context we create so that we can share data between contexts.
 */
class OpenGLResourceHolder {
private:
    OpenGLResourceHolder() {
        DLOG(INFO) << "OpenGLResourceHolder Initialize OpenGL";
//         glDisplay = GetPlatformEglDisplay(EGL_PLATFORM_OHOS_KHR, EGL_DEFAULT_DISPLAY, NULL);
//         if (glDisplay == EGL_NO_DISPLAY) {
//             return;
//         }
//         EGLint major, minor;
//         // Initialize EGLDisplay
//         if (eglInitialize(glDisplay, &major, &minor) == EGL_FALSE) {
//             return;
//         }
//         // The API for binding graphic drawing is OpenGLES
//         if (eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE) {
//             return;
//         }
//         unsigned int ret;
//         EGLint count;
//         EGLint config_attribs[] = {EGL_SURFACE_TYPE,
//                                    EGL_WINDOW_BIT,
//                                    EGL_RED_SIZE,
//                                    8,
//                                    EGL_GREEN_SIZE,
//                                    8,
//                                    EGL_BLUE_SIZE,
//                                    8,
//                                    EGL_ALPHA_SIZE,
//                                    8,
//                                    EGL_RENDERABLE_TYPE,
//                                    EGL_OPENGL_ES3_BIT,
//                                    EGL_NONE};
//         // Obtain a valid system configuration information
//         ret = eglChooseConfig(glDisplay, config_attribs, &glConfig, 1, &count);
//         if (!(ret && static_cast<unsigned int>(count) >= 1)) {
//             return;
//         }
//         static const EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, EGL_CONTEXT_CLIENT_VERSION_NUM, EGL_NONE};
//         glContext = eglCreateContext(glContext, glConfig, EGL_NO_CONTEXT, context_attribs);
//         if (glContext == EGL_NO_CONTEXT) {
//             return;
//         }
//         // Associated Context
//         eglMakeCurrent(glDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, glContext);


        /*// Initialize OpenGL
        glDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (glDisplay == EGL_NO_DISPLAY) {
            DLOG(INFO) << "eglGetDisplay failed : " << glGetError();
            // RNSkLogger::logToConsole("eglGetDisplay failed : %i", glGetError());
            return;
        }

        EGLint major;
        EGLint minor;
        if (eglInitialize(glDisplay, &major, &minor) != EGL_TRUE) {
            DLOG(INFO) << "eglInitialize failed : " << glGetError();
            return;
        }

        // Create a default shared context
        glConfig = getConfig(glDisplay);

        // Create OpenGL context attributes
        EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

        // Initialize the offscreen context for this thread
        glContext = eglCreateContext(glDisplay, glConfig, glContext, contextAttribs);
        if (glContext == EGL_NO_CONTEXT) {
            DLOG(INFO) << "glCreateContext failed : " << glGetError();
        }*/
    }

    ~OpenGLResourceHolder() {
        if (glContext != EGL_NO_CONTEXT) {
            eglDestroyContext(glDisplay, glContext);
            glContext = EGL_NO_CONTEXT;
        }

        if (glDisplay != EGL_NO_DISPLAY) {
            eglTerminate(glDisplay);
            glDisplay = EGL_NO_DISPLAY;
        }
    }
    /* Explicitly disallow copying. */
    OpenGLResourceHolder(const OpenGLResourceHolder &) = delete;
    OpenGLResourceHolder &operator=(const OpenGLResourceHolder &) = delete;

public:
    static OpenGLResourceHolder &getInstance() {
        static OpenGLResourceHolder Instance;
        return Instance;
    }

    /**
     * The first context created will be considered the parent / shared context
     * and will be used as the parent / shareable context when creating subsequent
     * contexts.
     */
    std::atomic<EGLContext> glContext = EGL_NO_CONTEXT;
    /**
     * Shared egl display
     */
    std::atomic<EGLDisplay> glDisplay = EGL_NO_DISPLAY;

    /**
     * Shared eglConfig
     */
    EGLConfig glConfig = 0;
    
    

private:
    /**
     * Finds the correct EGL Config for the given parameters
     * @param glDisplay
     * @return Config or zero if no matching context could be found.
     */
    static EGLConfig getConfig(EGLDisplay glDisplay) {

        EGLint att[] = {EGL_RENDERABLE_TYPE,
                        EGL_OPENGL_ES2_BIT,
                        EGL_ALPHA_SIZE,
                        8,
                        EGL_BLUE_SIZE,
                        8,
                        EGL_GREEN_SIZE,
                        8,
                        EGL_RED_SIZE,
                        8,
                        EGL_DEPTH_SIZE,
                        0,
                        EGL_STENCIL_SIZE,
                        0,
                        EGL_SAMPLE_BUFFERS,
                        0,
                        EGL_NONE};

        EGLint numConfigs;
        EGLConfig glConfig = 0;
        if (eglChooseConfig(glDisplay, att, &glConfig, 1, &numConfigs) != EGL_TRUE || numConfigs == 0) {

            DLOG(INFO) << "Failed to choose a config for %s surface. Error code: " << glGetError();

            return 0;
        }

        return glConfig;
    }

//     EGLDisplay GetPlatformEglDisplay(EGLenum platform, void *native_display, const EGLint *attrib_list) {
//         static GetPlatformDisplayExt eglGetPlatformDisplayExt = NULL;
//         if (!eglGetPlatformDisplayExt) {
//             const char *extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
//             if (extensions && (CheckEglExtension(extensions, EGL_EXT_PLATFORM_WAYLAND) ||
//                                CheckEglExtension(extensions, EGL_KHR_PLATFORM_WAYLAND))) {
//                 eglGetPlatformDisplayExt = (GetPlatformDisplayExt)eglGetProcAddress(EGL_GET_PLATFORM_DISPLAY_EXT);
//             }
//         }
//
//         if (eglGetPlatformDisplayExt) {
//             return eglGetPlatformDisplayExt(platform, native_display, attrib_list);
//         }
//
//         return eglGetDisplay((EGLNativeDisplayType)native_display);
//     }
//
//     bool CheckEglExtension(const char *eglExtensions, const char *eglExtension) {
//         // Check egl extension
//         size_t extLenth = strlen(eglExtension);
//         const char *endPos = eglExtensions + strlen(eglExtensions);
//
//         while (eglExtensions < endPos) {
//             size_t len = 0;
//             if (*eglExtensions == CHARACTER_WHITESPACE) {
//                 eglExtensions++;
//                 continue;
//             }
//             len = strcspn(eglExtensions, CHARACTER_STRING_WHITESPACE);
//             if (len == extLenth && strncmp(eglExtension, eglExtensions, len) == 0) {
//                 return true;
//             }
//             eglExtensions += len;
//         }
//         return false;
//     }
};

struct SkiaOpenGLContext {
    SkiaOpenGLContext() {
        glContext = EGL_NO_CONTEXT;
        gl1x1Surface = EGL_NO_SURFACE;
        directContext = nullptr;
    }
    ~SkiaOpenGLContext() {
        if (gl1x1Surface != EGL_NO_SURFACE) {
            eglDestroySurface(OpenGLResourceHolder::getInstance().glDisplay, gl1x1Surface);
            gl1x1Surface = EGL_NO_SURFACE;
        }

        if (directContext) {
            directContext->releaseResourcesAndAbandonContext();
            directContext = nullptr;
        }

        if (glContext != EGL_NO_CONTEXT) {
            eglDestroyContext(OpenGLResourceHolder::getInstance().glDisplay, // Harmony支持eglDestroyContext
                              glContext);
            glContext = EGL_NO_CONTEXT;
        }
    }
    EGLContext glContext;
    EGLSurface gl1x1Surface;
    sk_sp<GrDirectContext> directContext;
};

class SkiaOpenGLHelper {
public:
    /**
     * Calls eglMakeCurrent on the surface provided using the provided
     * thread context.
     * @param context Skia OpenGL context to use
     * @param surface Surface to set as current
     * @return true if eglMakeCurrent was successfull
     */
    static bool makeCurrent(SkiaOpenGLContext *context, EGLSurface glSurface) {
        // We don't need to call make current if we already are current:
        if (eglGetCurrentSurface(EGL_DRAW) != glSurface || eglGetCurrentSurface(EGL_READ) != glSurface ||
            eglGetCurrentContext() != context->glContext) {

            auto curr =
                eglMakeCurrent(OpenGLResourceHolder::getInstance().glDisplay, glSurface, glSurface, context->glContext);
            // Make current!
            if (curr != EGL_TRUE) {
                DLOG(ERROR) << "eglMakeCurrent failed: " << curr;
                return false;
            }
        }
        return true;
    }

    /**
     * Creates a new windowed surface
     * @param window NativeWindow to create surface in
     * @return EGLSurface or EGL_NO_SURFACE if the call failed
     */
    static EGLSurface createWindowedSurface(void *window) {
        const EGLint attribs[] = {EGL_NONE};
        EGLNativeWindowType nativeWin = static_cast<EGLNativeWindowType>(window);
        return eglCreateWindowSurface(OpenGLResourceHolder::getInstance().glDisplay,
                                      OpenGLResourceHolder::getInstance().glConfig, nativeWin, NULL);
    }


    /**
     * Destroys an egl surface
     * @param glSurface
     * @return
     */
    static bool destroySurface(EGLSurface glSurface) {
        if (eglMakeCurrent(OpenGLResourceHolder::getInstance().glDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
                           EGL_NO_CONTEXT) != EGL_TRUE) {
            DLOG(INFO) << "destroySurface: Could not clear selected surface;";
            return false;
        }
        return eglDestroySurface(OpenGLResourceHolder::getInstance().glDisplay, glSurface) == EGL_TRUE;
    }

    /**
     * Calls the eglSwapBuffer in the current thread with the provided surface
     * @param context Thread context
     * @param glSurface surface to present
     * @return true if eglSwapBuffers succeeded.
     */
    static bool swapBuffers(SkiaOpenGLContext *context, EGLSurface glSurface) {
        if (eglSwapBuffers(OpenGLResourceHolder::getInstance().glDisplay, glSurface) != EGL_TRUE) {
            DLOG(INFO) << "eglSwapBuffers failed:" << eglGetError();
            return false;
        }
        return true;
    }

    /***
     * Creates a new Skia direct context backed by the provided eglContext in the
     * SkiaOpenGLContext.
     * @param context Context to store results in
     * @param sharedContext Shared Context
     * @return true if the call to create a skia direct context suceeded.
     */
    static bool createSkiaDirectContextIfNecessary(SkiaOpenGLContext *context) {
        if (context->directContext == nullptr) {

            // Create OpenGL context
            createOpenGLContext(context);

            // Create attributes for a simple 1x1 pbuffer surface that we can
            // use to activate and create Skia direct context for
            const EGLint offScreenSurfaceAttribs[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};

            context->gl1x1Surface =
                eglCreatePbufferSurface(OpenGLResourceHolder::getInstance().glDisplay, // 创建表面，进行离屏渲染
                                        OpenGLResourceHolder::getInstance().glConfig, offScreenSurfaceAttribs);
            if (context->gl1x1Surface == EGL_NO_SURFACE) {
                DLOG(INFO) << "Failed creating a 1x1 pbuffer surface;";
                return false;
            }

            // Activate
            if (!SkiaOpenGLHelper::makeCurrent(context, context->gl1x1Surface)) {
                return false;
            }

            DLOG(INFO) << "GrGLMakeNativeInterface Create the Skia context";
            // Create the Skia context
            auto backendInterface =
                GrGLMakeNativeInterface(); // GrGLMakeNativeInterface无实现方法，在skia中被声明，Android中直接使用
            context->directContext =
                GrDirectContexts::MakeGL(backendInterface); // GrDirectContexts实现方法在skia的cpp中，Android中直接用

            if (context->directContext == nullptr) {
                DLOG(ERROR) << "GrDirectContexts::MakeGL failed;";
                return false;
            }
        }

        // It all went well!
        return true;
    }

    /**
     * Creates a new GLContext.
     * @param context Context to save results in
     * @return True if the call to eglCreateContext returned a valid OpenGL
     * Context or if the context already is setup.
     */
    static bool createOpenGLContext(SkiaOpenGLContext *context) {
        DLOG(INFO) << "createOpenGLContext eglCreateContext";
        // Create OpenGL context attributes
        EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

        // Initialize the offscreen context for this thread
        context->glContext = eglCreateContext(OpenGLResourceHolder::getInstance().glDisplay,
                                              OpenGLResourceHolder::getInstance().glConfig,
                                              OpenGLResourceHolder::getInstance().glContext, contextAttribs);

        if (context->glContext == EGL_NO_CONTEXT) {
            DLOG(INFO) << "eglCreateContext failed: " << eglGetError();
            return EGL_NO_CONTEXT;
        }

        return true;
    }
};
} // namespace RNSkia


#endif // SKIAHARMONY_SKIAOPENGLHELPER_H