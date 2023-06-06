Qt Object Measure
==== 
- 使用 C++ 和 OpenCV 实现一个具有视频捕捉、图像显示、尺寸测量及结果显示和输出等的桌面应用，实现对圆形物体的圆心、直径和矩形物体的长和宽尺寸测量，包括相应定位及标定功能。
- 实现了在 QML 中使用 OpenCV 画面显示，在 Qt 提供的定时器触发中断时，VideoCapture 读取 Mat 之后转换为 OpenGL 的纹理，交给 Qt 提供的 QQuickFramebufferObject (fbo) 组件渲染到客户端。
- OpenCV 测量功能实现参考
~~~
https://github.com/murtazahassan/Real-Time-Object-Measurement
~~~
- 基于 QML 前端界面参考
~~~
https://github.com/MattLigocki/DNNAssist
~~~
## 开发环境
1. 编程语言：C++ 11
2. 编译器：mingw730_64
3. 技术选型：Qt 5.12.12 + OpenCV 4.5.2 + OpenGL 3.3
4. 编程环境：CLion 2020.3.4 + Window 10
## 项目文件
~~~     
├── src            
│    └── assets               // 静态资源文件
│    └── common               // 宏定义
│    └── component            // 注入组件
│    └── controller           // 控制层
│    └── entity               // 数据体
│    └── model                // 注入模型
│    └── service              // 图像处理
│    └── system               // 系统服务
│    └── util                 // 工具类
│    └── views                // 程序入口
│          └── screen         // 页面
│          └── widget         // 通用 UI 组件
│          └── main.qml       // UI 入口
│    └── main.c               // 程序入口
│    └── resource.qrc         // 资源配置文件
│    └── CMakeLists.txt
├── CMakeLists.txt
├── README.md
~~~
## 示例
### 测量
![2e35c90413b5e17a9783c61c5a14509.png](https://s2.loli.net/2022/09/13/FaPNKkxJfb21WB5.png)
![9432324202b44b520aff7aaeef369c3.png](https://s2.loli.net/2022/09/13/iYnQHOFBb9v8gIw.png)
### 相机矫正
![7312380eb22791d751512fe4b535d13.png](https://s2.loli.net/2022/09/13/pZnFYKk93wmbsfj.png)
![b93f384d35b950175b02a155337e8f7.png](https://s2.loli.net/2022/09/13/nNWQMumKiXljBot.png)
## 本地运行
### 配置 Qt5 环境和 Toolchains
- 安装 Qt 后将  `@/Tools/mingw730_64` 作为 CLion 的 Toolchains，添加环境变量 `@/5.12.12/mingw73_64/bin`， 修改 CMakeLists 中 `CMAKE_PREFIX_PATH ` 为 `@/5.12.12/mingw73_64/lib/cmake`
### 配置 OpenCV 4.5.2 环境
- 下载已编译完成的 MinGW 版 OpenCV 库，并修改 CMakeLists 中 `OpenCV_DIR`，添加环境变量 `@/x64/mingw/bin` 
~~~
git clone -b OpenCV-4.5.2-x64 git@github.com:huihut/OpenCV-MinGW-Build.git
~~~
### 设置网络摄像头
- `https://apkpure.com/cn/` 下载安装 DroidCam 或 IP 摄像头


