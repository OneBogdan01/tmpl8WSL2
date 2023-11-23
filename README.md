
# tmpl8WSL2
This project was made for making 3D games on the Raspberry Pi 4, but also using WSL2 to speed up development time.  
Based on this template: https://github.com/jbikker/tmpl8pi/tree/main
# WSL2 
To run on WSL2 set the target platform to wsl:  
![image](https://github.com/OneBogdan01/tmpl8WSL2/assets/31468440/42b20f99-45a8-4d17-be89-c4a25601b22c)

Make sure you follow this tutorial on setting up wsl, also make sure to use **debian**
Like so:
```
wsl --install -d Debian
```

![image](https://github.com/OneBogdan01/tmpl8WSL2/assets/31468440/5ed48081-9e0a-4a35-b9e9-a1bd41287210)  
Try to run GUI apps on WSL2 https://learn.microsoft.com/en-us/windows/wsl/tutorials/gui-apps

On your WSL terminal install:
```
sudo apt-get install libgles2-mesa-dev
sudo apt-get install libxrandr-dev
sudo apt-get install g++ gdb make ninja-build rsync zip
sudo apt-get install libbullet-dev
```
# Bugs
If your window looks something like this:  
![image](https://github.com/OneBogdan01/tmpl8WSL2/assets/31468440/d96d6100-856c-430c-9e36-2ab75ddd80bc)
Open a powershell and run 
```
wsl --shutdown
```
Next time you run the project it should be displayed normally.

## Controls for the free camera
- W S - zooming
- A D - rotating horizontally
- Z C - rotating vertically
# BUAS
This was made as my first block project (Y1, programming)  
![Screenshot 2023-11-23 at 15-25-29 Logo Breda University_PMS - Logo BUas_RGB pdf](https://github.com/OneBogdan01/tmpl8WSL2/assets/31468440/f3d8586f-4fac-478e-aa49-dea7ac168f6d)

