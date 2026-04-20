# Electronic-Design-Contest---Medicine-Delivery-Car
Dual STM32 vehicles use K230 AI vision for ward detection. Bluetooth syncs dual-car data. Delivery status uploads via OneNET. A simple mobile APP monitors tasks.
| 功能模块 | 实现方案 |
|:---|:---|
| **循迹** | 八路灰度传感器 + 双环 PID 闭环控制 |
| **病房号识别** | K230 边缘 AI 摄像头 + 目标检测模型 |
| **双车协同** | HC-05 蓝牙模块主从配对通信 |
| **物联网上传** | ESP8266 + OneNET 云平台 MQTT 协议 |
| **移动端监控** | Uni-app 开发的物流管理 APP |
| **语音交互** | CI1302 离线语音模块 |
| **超声波避障** | HC-SR04 超声波测距模块 |
| **串口屏交互** | 4.3寸 IPS 触摸串口屏 |
<img width="398" height="775" alt="image" src="https://github.com/user-attachments/assets/9454f47b-33b7-4e81-9a20-5ce640f5d036" />
<img width="313" height="352" alt="image" src="https://github.com/user-attachments/assets/2e5995a5-aa56-4235-8386-a4a12d7af11c" />
<img width="381" height="304" alt="image" src="https://github.com/user-attachments/assets/b2bd7913-8cd6-46cb-9c34-311eeeabbb10" />
