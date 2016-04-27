## TexCam Prototype (An Imaging Software)

### Introduction

This project is the TexCam Prototype, a imaging software. It handles the communication with [1394 camera](http://www.cs.cmu.edu/~iwan/1394/) and [VXM stepping motor controller](http://www.velmex.com/products/controls/vxm_controller.html). The codes are written in c++ using .NET Framework programming and [XMV](https://github.com/zhanghang1989/XMV). 

Link to the [project](http://www.hangzh.com/Reflectance.html) and [paper](http://www.cv-foundation.org/openaccess/content_cvpr_2015/papers/Zhang_Reflectance_Hashing_for_2015_CVPR_paper.pdf). 
    
### The codes

* ``Camera1394.cpp`` handles with the comunication with the camera using SDK and VXM motor controller using serial port. 
* ``MFC_GUI`` foler provides a GUI for the imaging program implemented using C++ MFC. 
