This project is developed using Tuya SDK, which enables you to quickly develop branded apps connecting and controlling smart scenarios of many devices.For more information, please check Tuya Developer Website.

# 										SmartAirSTM

​																																																								——Author: VCJ

​																		 **Only for personal learning use, not for any commercial purposes!!!**

Neteayoudaobb1 youdao dictionary translation readme-cn.md from the grammar is unavoidable, please refer to readme-cn.md content shall be subject to.



​	All the basic functions have been tested and all are normal. All the functions have been realized. The circuit is normal except for the over-high magnification of the op-amp part (the V2 version has been corrected).

​    The graffiti module on the PCB layout uses the module with the IPEX interface, and the RF antenna does not come out without the antenna, so it needs to plug in the RF antenna (this has been fixed for V2 version).

The code is shared on GitHub, there is a link at the end of the article, and only the firmware is provided in the attachment.

 

### Renderings of finished products：



In order to facilitate debugging, the panel is not pasted. ![1](https://i.loli.net/2021/03/10/eGXBqzpsaMPv9j6.jpg)

### Circuit board material：

![2](https://i.loli.net/2021/03/10/OU6EQrqbtD7l9NM.jpg)

​               **Element surface**

 


 ![3](https://i.loli.net/2021/03/10/JwHOeC6dRsaMzQN.jpg)

​                                                                                                                     **The screen surface**

## **First, function overview：**

1.support detection of C02, temperature, humidity, illumination, formaldehyde, PM2.5, noise.
2.The 2.8-inch TFT touch screen supports color display and touch control.
3.Support battery power, using 3.7V lithium ion battery.
4.support battery power detection, over charge, over discharge, low voltage protection.
5.With buzzer, it can be used for detection and alarm.
6.with a key to start the circuit, under normal circumstances, when the shutdown, lithium battery will only have a few microampere current.
7.Support LCD backlight adjustment.


## **Second, design function：**

1. User operation interface
   Realize the operation interface of the machine interacting with the user (such as: slide screen, parameter setting, parameter view, etc.).
2. Cloud services
   Doodle cloud is directly adopted to provide cloud service, and the box detection parameters can be checked at any time on the mobile terminal.
3. Power management
   In the case of plugged in power supply, unplugged power supply, low battery power, etc., control the working conditions of different circuits, and keep the box working stably in a specific state.
4. Intelligent warning
   When a certain test value exceeds the standard, the buzzer will alarm, and the screen will display the specific alarm items.Users can press the button to disarm the alarm.

 

## **Third, function realization**

1. The user interface is divided into main interface, detail page, setting page and information page.
   1) The main interface mainly displays the actual parameters of several important test data (PM1, PM2.5, PM10, temperature and humidity, formaldehyde, carbon dioxide).Home page with weather and calendar function, can display the current city issued weather information.
   2) Details page, this page can display the data of all sensors, data real-time update.
   3) Setting page: this page can modify some parameters of the control box (such as: temperature scale, proofreading time, reset WIFI, etc.).
   4) Information page: This page can display the operation information of the box and the information related to the author.
2. Cloud services
   Select the WBR1D module of graffiti, directly onto the graffiti cloud. As for modules, the modules selected in version V1 do not have the IPEX interface, because they are installed in the box, it is suggested to choose the module with the IPEX socket, so that the signal is better.
3. Power management
   The power supply of the box can be turned on/off through the power button to realize the switch machine.At the same time, after normal operation of the box, the button can also be used as an ordinary button, multi-purpose.
   Built-in 3.7V 2000mAh lithium-ion rechargeable battery, the board has power management and booster circuit, which can realize charging management, battery power supply, power supply + battery, single power supply, battery over-discharge protection and other functions.
   With battery power monitoring, according to the change of power, adjust the power consumption of the box, protect the battery will not be damaged.
   Built-in lithium battery booster circuit, high boost efficiency, low loss, high output power.


## **Four, shell and sensor**

The shell uses common mode shell, the appearance is not good.

![4](https://i.loli.net/2021/03/10/PvAZ41tIfnr5cgk.png)

Because the housing is common mode housing, so the installation, some places the size is a little inappropriate, but within the acceptable range.

 


The sensor uses off-the-shelf modules.As follows:
Dust sensor, model: PMS7003, high accuracy, stability, low power consumption.

![5](https://i.loli.net/2021/03/10/eDTSA7CUdgzIs5h.png)

 

Formaldehyde sensor, model: DART WZ-S, high accuracy, power consumption is not high.It just takes some time for data to come back after power is turned on.

![6](https://i.loli.net/2021/03/10/OVDmA5BNc1bXGiK.png)



Carbon dioxide sensor, model T6703-5K, high accuracy, low power consumption, we should pay attention to the detection mouth not contaminated with debris and stains.
In my opinion, this module should still lead to the row, or welding on the board is too much position, it is not easy to dismantle.

![7](https://i.loli.net/2021/03/10/NmXAyonGFdgKaWM.png)

 

## **Five, description**

1. The program is written by the firmware library, and the code is constantly updated to fix bugs.
   2, the box used LCD screen, support Chinese characters display and picture display, because to display Chinese characters and pictures, so there should be Chinese characters and photo library (used pictures),
   The library and gallery are stored in external Flash (W25Q128). If there is no library and gallery in W25Q128, the box will not work properly.
   3, font and gallery is downloaded in the box program, along with STM32 program written, the principle is through MDK loading STM32 external SPI FLASH programming algorithm for W25Q128 programming,
   Only need to program once, the follow-up need not modify the W25Q128 internal data, modify any part of the microcontroller program will not affect the W25Q128 internal font and gallery data (except the entire erase).
   4, the resistive screen with the display screen, relative to the capacitive screen, touch reaction is slow, easy to detect errors, so the operation is not too fast.
2. Please refer to the attached instructions for downloading the font and gallery.
3. PCB open source and demonstration video: [立创开源平台-涂鸦智能七合一空气盒子](https://oshwhub.com/VCJ123/tu-ya-zhi-neng-wen-shi-du-he-zi)

 