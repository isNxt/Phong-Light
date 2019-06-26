# Phong-Light
- 实验 Shading
```txt
建议阅读资料：
（1）	课本第6章

Task1. 实现为每个顶点计算Phong照明强度的功能。输入为ply模型，例如之前提供的模型，不使用OpenGL的光照功能，自行完成Phong照明模型，具体要求为：       
（1）	设置视点、光源，以及各个光源系数（要求漫射、镜射、环境光等），设置物体材质。     
（2）	对每个顶点用Phong照明模型计算顶点颜色，用glColor3f上色         
（3）	对于三角形内的颜色，用OpenGL的glShadeModel( GL_SMOOTH) 填色模型填充。         
说明：结果要求与opengl的光照结果相近。        

Task2. 为ply模型设置光照，要求：   
（1）	光源相对模型不动，视点移动      
（2）	模型不动，光源随视点一起移动      
（3）	模型和视点均不动，光源绕模型旋转     
提示：把光源理解成物体即可。       

Phong's lighting implementation in OpenGL       
=========================================         

Though the physical parameters are the same in real world and OpenGL, but
OpenGL has more parameters to tune the lighting so that people who use it have
more freedom to get what they want (rather than what real objects are).
For example: in addition to having global ambient light, OpenGL also allows
“ambient” component for every light. Further, material can also have an
“emission” component (such that an object can be lit even when there is no other
light source). The ambient (ka), diffuse (kd) and specular (ks) constants are
actually arrays of 4 constants each (for the RGBA channels). Instead of one color
per light, OpenGL defines different colors, for the ambient, diffuse, and specular
components. You can probably model diffuse light like fluorescence light, or a
specular light like laser rays using these modeling parameters.
The OpenGL equation for lighting is:
Vertex color = (material emission) + (global ambient)*(material ambient) +
Σ (1/( k1+ k2d + k3d 2)) * [(light ambient)*(material ambient) + (max{L.N, 0})*(light
diffuse)*(material diffuse) + (max{H.N,0})n *(light specular)*(material specular) ]
H=(L+V)/|L+V|, 
	where L is the unit vector towards the light source
			  V is the unit vector towards the eye. 
			  This is called the half-vector between L and V.
			  N is the normal vector. n defines the shininess.
The “max” functions will make
sure that the light is not shining in the back-side of the face. If this value goes
beyond 1, it is clamped to 1 (lighting has been saturated at that point).
```
