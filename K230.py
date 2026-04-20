import gc
import os
import time  # 添加time模块
import aicube
import image
import nncase_runtime as nn
import ujson
import ulab.numpy as np
from libs.PipeLine import ScopedTiming
from libs.Utils import *
from media.display import *
from media.media import *
from media.sensor import *
from machine import Pin
from machine import UART
from machine import FPIOA

# 配置引脚
fpioa = FPIOA()
fpioa.set_function(11, FPIOA.UART2_TXD)
fpioa.set_function(12, FPIOA.UART2_RXD)
target_digit = -1  # 识别所有数字

# 初始化UART2，波特率9600
uart = UART(UART.UART2, baudrate=9600, bits=UART.EIGHTBITS,
            parity=UART.PARITY_NONE, stop=UART.STOPBITS_ONE)

# 设置LED引脚
fpioa.set_function(62, FPIOA.GPIO62)
fpioa.set_function(20, FPIOA.GPIO20)
fpioa.set_function(63, FPIOA.GPIO63)
LED_R = Pin(62, Pin.OUT, pull=Pin.PULL_NONE, drive=7)
LED_G = Pin(20, Pin.OUT, pull=Pin.PULL_NONE, drive=7)
LED_B = Pin(63, Pin.OUT, pull=Pin.PULL_NONE, drive=7)
LED_R.high()
LED_G.high()
LED_B.high()

# 显示设置
display_mode = "lcd"
if display_mode == "lcd":
    DISPLAY_WIDTH = ALIGN_UP(800, 16)
    DISPLAY_HEIGHT = 480
else:
    DISPLAY_WIDTH = ALIGN_UP(1920, 16)
    DISPLAY_HEIGHT = 1080

OUT_RGB888P_WIDTH = ALIGN_UP(640, 16)
OUT_RGB888P_HEIGH = 360

root_path = "/sdcard/mp_deployment_source/"
config_path = root_path + "deploy_config.json"
deploy_conf = {}
debug_mode = 1

# 新增全局变量
target_digit = -1  # -1表示识别所有数字，0-9表示只识别特定数字
mode_changed = False  # 模式是否改变标志

def two_side_pad_param(input_size, output_size):
    ratio_w = output_size[0] / input_size[0]  # 宽度缩放比例
    ratio_h = output_size[1] / input_size[1]  # 高度缩放比例
    ratio = min(ratio_w, ratio_h)  # 取较小的缩放比例
    new_w = int(ratio * input_size[0])  # 新宽度
    new_h = int(ratio * input_size[1])  # 新高度
    dw = (output_size[0] - new_w) / 2  # 宽度差
    dh = (output_size[1] - new_h) / 2  # 高度差
    top = int(round(dh - 0.1))
    bottom = int(round(dh + 0.1))
    left = int(round(dw - 0.1))
    right = int(round(dw - 0.1))
    return top, bottom, left, right, ratio

def read_deploy_config(config_path):
    # 打开JSON文件以进行读取deploy_config
    with open(config_path, "r") as json_file:
        try:
            # 从文件中加载JSON数据
            config = ujson.load(json_file)
        except ValueError as e:
            print("JSON 解析错误:", e)
    return config

def parse_uart_command():
    global target_digit, mode_changed
    if uart.any() > 0:  # 确保有数据可读
        command = uart.read(1)  # 读取一个字符
        if command is not None:
            command = command.decode('utf-8').strip()  # 转换为字符串并去除空白
            print("Received command:", command)

#            if command == "0":
#                target_digit = -1  # 识别所有数字
#                mode_changed = True
            if command in ["1", "2", "3", "4", "5", "6", "7", "8", "9"]:
                target_digit = int(command)
                mode_changed = True


def detection():
    global target_digit, mode_changed

    # 使用json读取内容初始化部署变量
    deploy_conf = read_deploy_config(config_path)
    kmodel_name = deploy_conf["kmodel_path"]
    labels = deploy_conf["categories"]
    confidence_threshold = deploy_conf["confidence_threshold"]
    nms_threshold = deploy_conf["nms_threshold"]
    img_size = deploy_conf["img_size"]
    num_classes = deploy_conf["num_classes"]
    color_four = get_colors(num_classes)
    nms_option = deploy_conf["nms_option"]
    model_type = deploy_conf["model_type"]
    if model_type == "AnchorBaseDet":
        anchors = deploy_conf["anchors"][0] + deploy_conf["anchors"][1] + deploy_conf["anchors"][2]
    kmodel_frame_size = img_size
    frame_size = [OUT_RGB888P_WIDTH, OUT_RGB888P_HEIGH]
    strides = [8, 16, 32]

    # 计算padding值
    top, bottom, left, right, ratio = two_side_pad_param(frame_size, kmodel_frame_size)

    # 初始化kpu
    kpu = nn.kpu()
    kpu.load_kmodel(root_path + kmodel_name)
    # 初始化ai2d
    ai2d = nn.ai2d()
    ai2d.set_dtype(nn.ai2d_format.NCHW_FMT, nn.ai2d_format.NCHW_FMT, np.uint8, np.uint8)
    ai2d.set_pad_param(True, [0, 0, 0, 0, top, bottom, left, right], 0, [114, 114, 114])
    ai2d.set_resize_param(True, nn.interp_method.tf_bilinear, nn.interp_mode.half_pixel)
    ai2d_builder = ai2d.build(
        [1, 3, OUT_RGB888P_HEIGH, OUT_RGB888P_WIDTH], [1, 3, kmodel_frame_size[1], kmodel_frame_size[0]]
    )
    # 初始化并配置sensor
    sensor = Sensor()
    sensor.reset()
    # 设置镜像
    sensor.set_hmirror(False)
    # 设置翻转
    sensor.set_vflip(False)
    # 通道0直接给到显示VO，格式为YUV420
    sensor.set_framesize(width=DISPLAY_WIDTH, height=DISPLAY_HEIGHT)
    sensor.set_pixformat(PIXEL_FORMAT_YUV_SEMIPLANAR_420)
    # 通道2给到AI做算法处理，格式为RGB888
    sensor.set_framesize(width=OUT_RGB888P_WIDTH, height=OUT_RGB888P_HEIGH, chn=CAM_CHN_ID_2)
    sensor.set_pixformat(PIXEL_FORMAT_RGB_888_PLANAR, chn=CAM_CHN_ID_2)
    # 绑定通道0的输出到vo
    sensor_bind_info = sensor.bind_info(x=0, y=0, chn=CAM_CHN_ID_0)
    Display.bind_layer(**sensor_bind_info, layer=Display.LAYER_VIDEO1)
    if display_mode == "lcd":
        # 设置为ST7701显示，默认800x480
        Display.init(Display.ST7701, to_ide=True)
    else:
        # 设置为LT9611显示，默认1920x1080
        Display.init(Display.LT9611, to_ide=True)
    # 创建OSD图像
    osd_img = image.Image(DISPLAY_WIDTH, DISPLAY_HEIGHT, image.ARGB8888)
    # media初始化
    MediaManager.init()
    # 启动sensor
    sensor.run()
    rgb888p_img = None
    ai2d_input_tensor = None
    data = np.ones((1, 3, kmodel_frame_size[1], kmodel_frame_size[0]), dtype=np.uint8)
    ai2d_output_tensor = nn.from_numpy(data)

    # 等待启动命令
    start_cmd = b"START"
    print("Waiting for START command from TI board...")
    received = b''
    while received != start_cmd:
        if uart.any() > 0:
            data = uart.read()
            if data is not None:
                if start_cmd in data:
                    received = start_cmd
                    print("Received START command")
                    uart.write("K230 Ready\n")
                    LED_G.low()  # 绿灯亮表示准备就绪
                    break
                else:
                    # 清空接收缓冲区
                    while uart.any() > 0:
                        uart.read()

    # 主循环
    while True:
        # 检查并解析UART命令
        parse_uart_command()

        with ScopedTiming("total", debug_mode > 0):
            rgb888p_img = sensor.snapshot(chn=CAM_CHN_ID_2)
            if rgb888p_img is not None and rgb888p_img.format() == image.RGBP888:
                ai2d_input = rgb888p_img.to_numpy_ref()
                ai2d_input_tensor = nn.from_numpy(ai2d_input)
                # 使用ai2d进行预处理
                ai2d_builder.run(ai2d_input_tensor, ai2d_output_tensor)
                # 设置模型输入
                kpu.set_input_tensor(0, ai2d_output_tensor)
                # 模型推理
                kpu.run()
                # 获取模型输出
                results = []
                for i in range(kpu.outputs_size()):
                    out_data = kpu.get_output_tensor(i)
                    result = out_data.to_numpy()
                    result = result.reshape((result.shape[0] * result.shape[1] * result.shape[2] * result.shape[3]))
                    del out_data
                    results.append(result)
                # 使用aicube模块封装的接口进行后处理
                det_boxes = aicube.anchorbasedet_post_process(
                    results[0],
                    results[1],
                    results[2],
                    kmodel_frame_size,
                    frame_size,
                    strides,
                    num_classes,
                    confidence_threshold,
                    nms_threshold,
                    anchors,
                    nms_option,
                )
                # 绘制结果
                osd_img.clear()
                detected = False
                found_target = False

                if det_boxes:
                    # 模式1: 识别所有数字 - 只发送第一个检测结果
                    if target_digit == -1:
                        det_boxe = det_boxes[0]
                        x1, y1, x2, y2 = det_boxe[2], det_boxe[3], det_boxe[4], det_boxe[5]
                        x = int(x1 * DISPLAY_WIDTH // OUT_RGB888P_WIDTH)
                        y = int(y1 * DISPLAY_HEIGHT // OUT_RGB888P_HEIGH)
                        w = int((x2 - x1) * DISPLAY_WIDTH // OUT_RGB888P_WIDTH)
                        h = int((y2 - y1) * DISPLAY_HEIGHT // OUT_RGB888P_HEIGH)
                        osd_img.draw_rectangle(x, y, w, h, color=color_four[det_boxe[0]][1:])
                        text = labels[det_boxe[0]] + " " + str(round(det_boxe[1], 2))
                        osd_img.draw_string_advanced(x+50, y +h+ 40, 32, text, color=color_four[det_boxe[0]][1:])

                        digit = int(labels[det_boxe[0]])
                        confidence = int(round(det_boxe[1] * 100))
                        center_x = (x1 + x2) / 2 * DISPLAY_WIDTH / OUT_RGB888P_WIDTH
                        location_val = 1 if center_x > DISPLAY_WIDTH / 2 else 0

                        output_str = f"[{digit},{confidence},{location_val}]\r\n"
                        uart.write(output_str)
                        print(f"Sent: {output_str.strip()}")
                        detected = True

                    # 模式2: 只识别特定数字
                    else:
                        for det_boxe in det_boxes:
                            digit = int(labels[det_boxe[0]])

                            # 只处理目标数字
                            if digit == target_digit:
                                x1, y1, x2, y2 = det_boxe[2], det_boxe[3], det_boxe[4], det_boxe[5]
                                x = int(x1 * DISPLAY_WIDTH // OUT_RGB888P_WIDTH)
                                y = int(y1 * DISPLAY_HEIGHT // OUT_RGB888P_HEIGH)
                                w = int((x2 - x1) * DISPLAY_WIDTH // OUT_RGB888P_WIDTH)
                                h = int((y2 - y1) * DISPLAY_HEIGHT // OUT_RGB888P_HEIGH)
                                osd_img.draw_rectangle(x, y, w, h, color=color_four[det_boxe[0]][1:])
                                text = labels[det_boxe[0]] + " " + str(round(det_boxe[1], 2))
                                osd_img.draw_string_advanced(x+50, y +h+ 40, 32, text, color=color_four[det_boxe[0]][1:])

                                confidence = int(round(det_boxe[1] * 100))
                                center_x = (x1 + x2) / 2 * DISPLAY_WIDTH / OUT_RGB888P_WIDTH
                                location_val = 1 if center_x > DISPLAY_WIDTH / 2 else 0

                                output_str = f"[{digit},{confidence},{location_val}]\r\n"
                                uart.write(output_str)
                                print(f"Sent: {output_str.strip()}")
                                detected = True
                                found_target = True
                                break  # 只发送第一个匹配的目标

                        # 没有检测到目标数字
                        if not found_target:
                            output_str = "[-1,0,0]\r\n"
                            uart.write(output_str)
                            print("Sent: [-1,0,0]")

                # 没有检测到任何目标
                if not detected:
                    output_str = "[-1,0,0]\r\n"
                    uart.write(output_str)
                    print("Sent: [-1,0,0]")

                # 显示模式改变提示
#                if mode_changed:
#                    mode_text = f"Mode: {'All' if target_digit == -1 else f'Only {target_digit}'}"
#                    osd_img.draw_string(10, 10, mode_text, color=(0, 255, 0), scale=3)
#                    mode_changed = False

                Display.show_image(osd_img, 0, 0, Display.LAYER_OSD3)
                gc.collect()
            elif rgb888p_img is not None:
                rgb888p_img = None

    # 停止摄像头输出
    sensor.stop()
    # 去初始化显示设备
    Display.deinit()
    # 释放媒体缓冲区
    MediaManager.deinit()
    gc.collect()
    time.sleep(1)
    nn.shrink_memory_pool()
    print("det_infer end")
    return 0

if __name__ == "__main__":
    detection()
