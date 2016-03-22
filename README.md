# TexCam Prototype (An Imaging Software)
created by Hang Zhang, Kristin Dana and Ko Nishino

### Introduction

This project is the TexCam Prototype, a imaging software. It handles the communication with [1394 camera](http://www.cs.cmu.edu/~iwan/1394/) and [VXM stepping motor controller](http://www.velmex.com/products/controls/vxm_controller.html). The codes are written in c++ using .NET Framework programming and [XMV](https://github.com/zhanghang1989/XMV). 

Link to the [project](http://www.hangzh.com/Reflectance.html) and [paper](http://www.cv-foundation.org/openaccess/content_cvpr_2015/papers/Zhang_Reflectance_Hashing_for_2015_CVPR_paper.pdf). 

If you use the code for related research, please cite our paper:
  
  @article{zhang2015reflectance,
      title={Reflectance Hashing for Material Recognition},
      author={Zhang, Hang and Dana, Kristin and Nishino, Ko},
      journal={IEEE Conference on Computer Vision and Pattern Recognition (CVPR)},
      pages={3071--3080},
      year={2015}
  }

### The codes

*"Camera1394.cpp" handles with the comunication with the camera using SDK and VXM motor controller using serial port. 
*"MFC_GUI" provides a simple GUI which for this imaging program. 
