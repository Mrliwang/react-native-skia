// /*
//  * Copyright (c) 2024 Huawei Device Co., Ltd.
//  * Licensed under the Apache License, Version 2.0 (the "License");
//  * you may not use this file except in compliance with the License.
//  * You may obtain a copy of the License at
//  *
//  * http://www.apache.org/licenses/LICENSE-2.0
//  *
//  * Unless required by applicable law or agreed to in writing, software
//  * distributed under the License is distributed on an "AS IS" BASIS,
//  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  * See the License for the specific language governing permissions and
//  * limitations under the License.
//  */
// #ifndef NATIVE_XCOMPONENT_PLUGIN_RENDER_H
// #define NATIVE_XCOMPONENT_PLUGIN_RENDER_H
//
// #include <ace/xcomponent/native_interface_xcomponent.h>
// #include <cstddef>
// #include <napi/native_api.h>
// #include "napi/n_func_arg.h"
// #include <string>
// #include <unordered_map>
//
// #include "RNSkHarmonyView.h"
// #include "RNSkPlatformContext.h"
// #include "SkiaManager.h"
//
// namespace RNSkia {
// class PluginRender {
// public:
//     PluginRender(std::shared_ptr<RNSkia::RNSkPlatformContext> context);
//     ~PluginRender() {}
//     static PluginRender *GetInstance(std::string &id);
//
//     void OnSurfaceChanged(OH_NativeXComponent *component, void *window);
//     void OnTouchEvent(OH_NativeXComponent *component, void *window);
//     void OnMouseEvent(OH_NativeXComponent *component, void *window);
//     void OnHoverEvent(OH_NativeXComponent *component, bool isHover);
//     void OnFocusEvent(OH_NativeXComponent *component, void *window);
//     void OnBlurEvent(OH_NativeXComponent *component, void *window);
//     void OnKeyEvent(OH_NativeXComponent *component, void *window);
//     void RegisterCallback(OH_NativeXComponent *nativeXComponent);
//     void Export(napi_env env, napi_value exports);
//
//     static napi_value RegisterView(napi_env env, napi_callback_info info) {
//         DLOG(INFO) << "napi RegisterView";
//         // 获取参数
//         NFuncArg funcArg(env, info);
//         if (!funcArg.InitArgs(NARG_CNT::TWO)) {
//             return nullptr;
//         }
//         napi_value v1 = funcArg.GetArg(NARG_POS::FIRST);
//         NVal nValXcId(env, v1);
//         auto [v1Succ, xComponentId, nLength] = nValXcId.ToUTF8String();
//         if (!v1Succ) {
//             DLOG(ERROR) << "napi RegisterView get xComponentId fail";
//             return nullptr;
//         }
//         napi_value v2 = funcArg.GetArg(NARG_POS::SECOND);
//         NVal nValNativeId(env, v2);
//         auto [v2Succ, nativeId] = nValNativeId.ToInt32();
//         if (!v2Succ) {
//             DLOG(ERROR) << "napi RegisterView get nValNativeId fail";
//             return nullptr;
//         }
//         // nativeId 不等于 tag
//         DLOG(INFO) << "napi registerView xComponentId: " << xComponentId << " nativeId: " << nativeId;
//         std::string id(xComponentId.get());
//         if (m_instance.find(id) != m_instance.end()) {
//             PluginRender *instance = m_instance[id];
//             auto view = instance->_harmonyView;
//             std::shared_ptr<RNSkView> rNSkView = view->getSkiaView();
//             size_t nId = static_cast<size_t>(nativeId);
//             SkiaManager::getInstance().getManager()->registerSkiaView(nId, rNSkView);
//             DLOG(INFO) << "napi registerView finish xComponentId: " << id;
//             auto render = PluginRender::GetInstance(id);
//             view->surfaceAvailable(render->m_window, render->m_width, render->m_height);
//             //view->render();
//         }
//         return nullptr;
//     };
//
// public:
//     static std::unordered_map<std::string, PluginRender *> m_instance;
//     OHNativeWindow *m_window;
//     uint64_t m_width;
//     uint64_t m_height;
//    
//     std::shared_ptr<RNSkBaseHarmonyView> _harmonyView;
//
//
// private:
//     OH_NativeXComponent_Callback m_renderCallback;
//     OH_NativeXComponent_MouseEvent_Callback m_mouseCallback;
// };
// } // namespace RNSkia
// #endif // NATIVE_XCOMPONENT_PLUGIN_RENDER_H
