#include "device_info_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>



namespace {

class DeviceInfoPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  DeviceInfoPlugin();

  virtual ~DeviceInfoPlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void DeviceInfoPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "device_info",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<DeviceInfoPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

DeviceInfoPlugin::DeviceInfoPlugin() {}

DeviceInfoPlugin::~DeviceInfoPlugin() {}

void DeviceInfoPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  // Replace "getPlatformVersion" check with your plugin's method.
  // See:
  // https://github.com/flutter/engine/tree/master/shell/platform/common/cpp/client_wrapper/include/flutter
  // and
  // https://github.com/flutter/engine/tree/master/shell/platform/glfw/client_wrapper/include/flutter
  // for the relevant Flutter APIs.
  if (method_call.method_name().compare("getDeviceInfo") == 0) {
    std::ostringstream device_info;

    std::string systemVersion;
    if (IsWindows10OrGreater()) {
      systemVersion = "win 10+";
    } else if (IsWindows8OrGreater()) {
      systemVersion = "win 8";
    } else if (IsWindows7OrGreater()) {
      systemVersion = "win 7";
    }

    //显示器分辨率
    int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    char str[2048];
    sprintf_s(str,sizeof(str),"{\"systemVersion\":%s,\"screenWidth\":%d,\"screenHeight\":%d}",systemVersion.c_str(),nScreenWidth,nScreenHeight);
    device_info << str;

    flutter::EncodableValue response(device_info.str());
    result->Success(&response);
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void DeviceInfoPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  DeviceInfoPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
