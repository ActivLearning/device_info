import 'dart:async';

import 'package:flutter/services.dart';

class DeviceInfo {
  static const MethodChannel _channel =
      const MethodChannel('device_info');

  static Future<String> get deviceInfo async {
    final String version = await _channel.invokeMethod('getDeviceInfo');
    return version;
  }
}
