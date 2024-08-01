/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NdkNativeWindow_NativeImageAdaptor_H
#define NdkNativeWindow_NativeImageAdaptor_H

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <bits/alltypes.h>
#include <napi/native_api.h>
#include <native_buffer/native_buffer.h>
#include <native_image/native_image.h>
#include <native_window/external_window.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <sys/mman.h>
#include <mutex>
#include <queue>
#include <glog/logging.h>

namespace RNSkia {
class NativeImageAdaptor {
public:
    NativeImageAdaptor();
    ~NativeImageAdaptor();
    static NativeImageAdaptor *GetInstance();

    static napi_value GetAvailableCount(napi_env env, napi_callback_info info){};
    static napi_value NapiOnGetAttachBufferCount(napi_env env, napi_callback_info info){};
    static napi_value NapiOnGetBufferQueueSize(napi_env env, napi_callback_info info){};
    static napi_value NapiOnGetCacheBufferCount(napi_env env, napi_callback_info info){};
    static napi_value NapiOnProduceBuffer(napi_env env, napi_callback_info info){};
    static napi_value NapiOnAttachBuffer(napi_env env, napi_callback_info info);
    static napi_value NapiOnDettachBuffer(napi_env env, napi_callback_info info);

    static void OnFrameAvailable(void *context);
    void DealCallback(void *context);
    bool Export(napi_env env, napi_value exports);
    void ProduceBuffer(uint32_t value, OHNativeWindow *InNativeWindow);
    void InitEGLEnv();
    bool InitNativeWindow();
    bool InitNativeWindowCache();
    void AttachBuffer();
    void DettachBuffer();
    EGLDisplay GetPlatformEglDisplay(EGLenum platform, void *native_display, const EGLint *attrib_list);
    bool CheckEglExtension(const char *eglExtensions, const char *eglExtension);
    int32_t GetCount();
    int32_t GetAttachBufferCount();
    int32_t GetBufferQueueSize();
    int32_t GetCacheBufferCount();
    void Update();

    OHNativeWindow *getNativeWindow(){
        return nativeWindow_;
    };

private:
    static std::unordered_map<std::string, NativeImageAdaptor *> m_instance;
    void SetConfigAndGetValue();
    void GetBufferMapPlanes(NativeWindowBuffer *buffer);
    OHNativeWindow *nativeWindow_;
    OHNativeWindow *nativeWindowCache_;
    OH_NativeImage *image_;
    OH_NativeImage *imageCache_;
    int32_t height_;
    int32_t width_;
    int32_t availableBufferCount_;
    EGLConfig config_;
    EGLContext eglContext_;
    EGLDisplay eglDisplay_;
    std::mutex opMutex_;
    std::queue<NativeWindowBuffer *> bufferCache_;
    std::queue<NativeWindowBuffer *> bufferAttached_;

};
}
#endif // NdkNativeWindow_NativeImageAdaptor_H
