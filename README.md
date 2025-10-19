# STM32 LED闪烁示例

![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Zephyr-green.svg)
![Board](https://img.shields.io/badge/board-STM32F103C6T6-orange.svg)

本示例演示了如何使用Zephyr RTOS在STM32F103C6T6开发板上实现LED闪烁功能。

## 目录
- [硬件设置](#硬件设置)
- [项目概述](#项目概述)
- [先决条件](#先决条件)
- [构建项目](#构建项目)
- [烧录程序](#烧录程序)
- [预期效果](#预期效果)
- [代码说明](#代码说明)
- [故障排除](#故障排除)
- [许可证](#许可证)

## 硬件设置

### 开发板信息
- **开发板**: STM32F103C6T6最小系统板
- **微控制器**: STM32F103C6T6 ARM Cortex-M3
- **板载LED**: 连接至PC13引脚（低电平有效）

本示例无需外部组件，直接使用开发板上的板载LED。

## 项目概述

本项目通过在STM32F103C6T6开发板上控制GPIO来实现LED闪烁，演示了Zephyr RTOS的基本GPIO控制功能。LED以5Hz的频率闪烁（200ms亮/灭间隔）。

### 主要特性
- 使用Zephyr的设备树(DTS)进行硬件配置
- 实现GPIO驱动API控制LED
- 展示Zephyr的基本线程和延时功能

## 先决条件

在构建应用程序之前，请确保您已安装：

1. **Zephyr SDK**（推荐版本0.16.0或更高）
2. **West** 工具链
3. **Python** 3.8或更高版本
4. 正确设置环境变量（ZEPHYR_BASE）



## 构建项目

### 使用West构建（推荐）

```bash
# 为STM32F103C6T6开发板构建，需要指定BOARD_ROOT
west build -b custom_bluepill -- -DBOARD_ROOT=
D:/my_zephyr/zephyrproject/my_zephyer_code/stm32_led/boards 
# 不知道行不行
west build -p always -b custom_bluepill
```


## 烧录程序

### 使用West烧录会出错

通过ST-Link编程器连接开发板并运行west flash时，会报错,需要烧录时长按开发板上的复位按钮才能正常烧录。使用stlink utility直接烧录可以正常运行。



### 手动烧录

您也可以手动烧录生成的二进制文件：
- `build/zephyr/zephyr.bin` - 原始二进制镜像
- `build/zephyr/zephyr.hex` - Intel HEX格式

使用stlink utility或其他烧录工具。

## 预期效果

烧录完成后，连接到PC13引脚的板载LED将以200ms亮/灭的间隔开始闪烁，即5Hz的闪烁频率。

## 代码说明

### 设备树配置

硬件配置定义在`app.overlay`文件中：

```dts
/ {
    leds {
        compatible = "gpio-leds";
        userled: led_1 {
            gpios = <&gpioc 13 GPIO_ACTIVE_LOW>;
            label = "LED 1";
            status = "okay";
        };
    };
    
    aliases {
        led1 = &userled;
    };
};
```

### 主应用程序

`src/main.c`中的主应用程序执行以下步骤：

1. **设备初始化**: 从设备树获取GPIO设备规范
2. **GPIO配置**: 将LED引脚配置为输出
3. **主循环**: 持续切换LED状态并延时200ms

关键代码段：

```c
// 从设备树获取GPIO规范
#define LED1_NODE DT_NODELABEL(userled)
static const struct gpio_dt_spec led_spec = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

// 配置GPIO引脚
gpio_pin_configure_dt(&led_spec, GPIO_OUTPUT_ACTIVE);

// 在循环中切换LED
while (1) {
    gpio_pin_toggle_dt(&led_spec);
    k_sleep(K_MSEC(200));  // 200ms延时
}
```

## 项目配置

项目配置定义在`prj.conf`文件中：

```
CONFIG_GPIO=y             # 启用GPIO驱动
CONFIG_LED=y              # 启用LED驱动
```

## 故障排除

# ⚠️ **注意：此处很重要！**
 **常见问题**
 
 <span style="color:red">
  因为我使用的是自定义的开发板名称custom_bluepill，所以在烧录时需要指定开发板名称。

  并且需要注意，board.yml和kconfig.custom_bluepill文件中配置的开发板名称与实际开发板名称不一致。这是为了编译成功，因为zephyr官方不支持c6t6。可能会导致一些未知的问题。

</span>

## 构建产物

成功构建后，将在`build/zephyr`目录中生成以下文件：
- `zephyr.elf` - ELF可执行文件
- `zephyr.bin` - 二进制固件镜像
- `zephyr.hex` - Intel HEX格式固件镜像
- `zephyr.map` - 内存映射文件

## 许可证

本项目采用Apache License 2.0许可证 - 详情请见[LICENSE](LICENSE)文件。
