# Yolo to kitty

### Introduction

This tool runs darknet models and save the predictions in kitti format. 

### Dependencies

OpenCV is used to load the images and display the results on screen. 
The code was tested only with OpenCV 3, but both versions should work. In the future I may remove this dependencie.

Detection with darknet can be performed on the CPU, but you can gain an enormous speed up by using an NVIDIA GPU with cuda and cuDNN. 
Currently cuDNN 5 or 6 and CUDA >=7.5 are supported.

Python is an optional dependencie to use the script that creates the list of images required to run this tool.
This script runs in both python 2 or 3.

### Compilation

Clone the repository in recursive mode to download darknet automatically.
To do so, open an terminal and type:

```bash
git clone --recursive https://github.com/scart97/Yolo-to-kitty.git
```

First you need to compile darknet. 
It can be found in the folder `darknet` inside the folder you just cloned.
Navigate to it and edit some parameters in the makefile:

```bash
cd Yolo-to-kitty/darknet/
gedit Makefile
```
Change the flags to `OPENCV=1`, `OPENMP=1` and `LIBSO=1`. 
If you have an NVIDIA card with cuda installed, also change `GPU=1` and `CUDNN=1` to have an speed up in detection speed.
Close the file and type `make` to compile darknet. Check if an file named `darknet.so` was created.

Go one folder up and type `make` to compile the tool. The file `yolo_to_kitty` should be created there.

### Running the tool

In order to run the tool, you first need to download the yolo weights. 
To do so, run the script:

```bash
sh download_weights.sh
```

While this download is occurring, you can prepare your data. 
Put the pictures you want to detect at the `images` folder, and run the script to generate the image list:

```bash
python generate_image_list.py
```

Each time you change the pictures in the folder you have to run this script before using the tool.
Now, if the weights have finished downloading, you can run the tool by typing:

```bash
./yolo_to_kitty imagelist.txt
```

The network will be loaded and the results will be saved to the `labels` folder.