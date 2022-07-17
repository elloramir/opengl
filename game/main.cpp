#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define HMM_PREFIX
#define HANDMADE_MATH_NO_SSE
#include "external/handmade_math.h"

#include "gfx_shader.h"
#include "gfx_texture.h"

// Settings
#define windowWidth  800
#define windowHeight 600

// Camera
hmm_vec3 cameraPos = Vec3(0.0f, 0.0f, 3.0f);
hmm_vec3 cameraFront = Vec3(0.0f, 0.0f, -1.0f);
hmm_vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
f32 yaw = -90.0f;
f32 pitch = 0.0f;
f32 lastX = windowWidth * 0.5f;
f32 lastY = windowHeight * 0.5f;
f32 fov = 45.0f;

f32 cameraSpeed = 2.5f;
f32 sensitivity = 0.1f;
f32 scrollMultiply = 2.f;

f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;

// Resize window
void FrameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Mouse position
void CursorPosCallback(GLFWwindow *window, f64 x, f64 y)
{
	f32 xPos = (f32)x;
	f32 yPos = (f32)y;

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	f32 xOffset = xPos - lastX;
	f32 yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	printf("%f %f\n", yaw, pitch);

	hmm_vec3 front;
	front.X = CosF(ToRadians(yaw)) * CosF(ToRadians(pitch));
	front.Y = SinF(ToRadians(pitch));
	front.Z = SinF(ToRadians(yaw)) * CosF(ToRadians(pitch));
	cameraFront = NormalizeVec3(front);
}

// Scroll callback
void ScrollCallback(GLFWwindow* window, f64 xoffset, f64 yoffset)
{
	fov -= (f32)yoffset * scrollMultiply;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

// Keyboard callback
void ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= NormalizeVec3(Cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += NormalizeVec3(Cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
}

int main(int argc, char **argv)
{
	// Init GLFW
	if(!glfwInit())
	{
		Panic("Can't initialize GLFW");
	}

	// GLFW window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create glfw window
	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "sailor gl", NULL, NULL);
	if(!window)
	{
		Panic("Error at GLFW window creation");
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Vsync on

	// Window callbacks
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// Initialize OpenGL
	if(glewInit() != GLEW_OK)
	{
		Panic("Can't load OpenGL with GLEW");
	}
	printf("OpenGL version %s\n", glGetString(GL_VERSION));

	// Load basic shader
	gfx::Shader basicShader("content/basic.vs", "content/basic.fs");

	// Textures
	gfx::Texture wallTex("content/wall.jpg");
	gfx::Texture emojoTex("content/emojo.png");

	basicShader.SendInt("uTexture1", 0);
	basicShader.SendInt("uTexture2", 1);

	// Cube vertex array
	f32 cubeVertexArray[] =
	{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	u32 VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexArray), cubeVertexArray, GL_STATIC_DRAW);

	// Position attribute - layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, S_FLOAT(5), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture attribute - layout 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, S_FLOAT(5), (void*)S_FLOAT(3));
	glEnableVertexAttribArray(2);

	// OpenGL setup functions
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, windowWidth, windowHeight);

	// Game loop
	while(!glfwWindowShouldClose(window))
	{
		// Process time
		float currentFrame = (f32)(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input events
		ProcessInput(window);

		// Clear previous buffers
		glClearColor(0.1f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render basics
		basicShader.Bind();

		// Bind textures
		glActiveTexture(GL_TEXTURE0);
		wallTex.Bind();
		glActiveTexture(GL_TEXTURE1);
		emojoTex.Bind();

		// MVP uniforms
		hmm_m4 model = Mat4d(1.0f) * Rotate(45.0f, Vec3(1.0f, 1.0f, 1.0f));
		hmm_m4 view = LookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		hmm_m4 projection = Perspective(fov, (f32)windowWidth / (f32)windowHeight, 0.1f, 100.0f);

		basicShader.SendMat4("model", (float*)&model);
		basicShader.SendMat4("view", (float*)&view);
		basicShader.SendMat4("projection", (float*)&projection);

		// Draw a single cube
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Update Window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	printf("bye sailor\n");
	return 0;
}