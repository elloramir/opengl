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
hmm_vec3 camera = Vec3(0.0f, 0.0f, 3.0f);
hmm_vec3 cameraFront = Vec3(0.0f, 0.0f, -1.0f);
hmm_vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0f);

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

	// Initialize OpenGL
	if(glewInit() != GLEW_OK)
	{
		Panic("Can't load OpenGL with GLEW");
	}
	printf("OpenGL version %s\n", glGetString(GL_VERSION));

	// Load basic shader
	gfx::Shader basicShader("content/basic.vs", "content/basic.fs");

	// MVP uniforms
	hmm_m4 model = Mat4d(1.0f) * Rotate(45.0f, Vec3(0.0f, 0.0f, 1.0f));
	hmm_m4 view = Mat4d(1.0f) * Translate(Vec3(0.0f, 0.0f, -4.0f));
	hmm_m4 projection = Perspective(45.0f, windowWidth / windowHeight, 0.1f, 100.0f);

	basicShader.SendMat4("model", (float*)&model);
	basicShader.SendMat4("view", (float*)&view);
	basicShader.SendMat4("projection", (float*)&projection);

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