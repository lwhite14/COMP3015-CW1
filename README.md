# COMP3015-CW1 - Deep Space Nova
An OpenGL project utilizing a custom shading model implemented in GLSL, with custom lighting set up, showcasing a textured based technique. <br> 
In this project, I have used the blinn-phong model to shade a handful of objects within the scene (alien ship, meteors, and a teapot). The scene contains two lights, a spot light and a point light. These two lights combine to shade the meteors and teapot. The alien ship is shaded using a normal map to give the ships surface more personality. With a button press, a gaussian blur filter is applied to the objects. Snap out of the still mode with Shift to fly around and inspect the shading.

### How to navigate the code.
The bulk of the logic pertaining to the shading techniques is done within the scenebasic_uniform.cpp source file. The initScene, initGuass, compile, and resize methods are excecuted once at the start of the program, whereas render and update are executed once per frame. The setupFBO, pass1, pass2, pass3, initGauss, and gauss methods are all used for applying the Gaussian filter. The setMatrices method is used to set shader uniform matrices every time a shader is used, and the bindTex method is used to bind the appropriate texture when using different shader programs. Vertex and Fragment shaders are located within the shaders directory, this is where the shading techniques are executed.

### Running the program.
To view the scene, download the .zip file from the released tab, extract the foler, then run the .exe file name 'COMP3015-CW1' to start the program. Or you can download the source code to inspect the project further. You will need all the OpenGL dependancies installed and linked to the project. These include: glfw, glad, and glm.

![cw1-ss1](https://user-images.githubusercontent.com/55700734/160313803-3f7ad528-ddec-42a5-8cdc-25feba5e7df6.png)
![cw1-ss3](https://user-images.githubusercontent.com/55700734/160315782-36e274a4-abe0-41cc-8e2d-06acdc042a5f.png)

