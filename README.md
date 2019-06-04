# GB28181Server  
# Qt+ffmpeg+exosip+jrtplib 实现的GB28181服务器代码  

![Image text](https://raw.githubusercontent.com/yundiantech/GB28181Server/master/screenshot.jpg)

更新日志：  
【V1.8.0】  
Qt5.6.2(vs2013) + ffmpeg4.1 + exosip2.5 + jrtplib  
1.只支持32位，64位库需要自行编译，请参考库源码中的编译说明自行编译。  
2.将底层协议部分和Qt界面分开成2个模块，底层协议部分代码使用纯c++编写，方便以后模块的移植  
3.目前解码部分仅支持H.264格式，请确保相机的视频格式设置为H.264。否则无法显示视频。  

相机设置如下：  
![Image text](https://raw.githubusercontent.com/yundiantech/GB28181Server/master/camera_conf.jpg)

