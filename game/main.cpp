#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gfx_shader.h"
#include "gfx_texture.h"
#define HMM_PREFIX
#define HANDMADE_MATH_NO_SSE
#include "external/handmade_math.h"

#define windowWidth  800
#define windowHeight 600

int main(int argc, char **argv) {
	// Init GLFW
	if(!glfwInit())
		Panic("Can't initialize GLFW");

	// Create Window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "sailor gl", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize OpenGL
	if(glewInit() != GLEW_OK) {
		glfwTerminate();
		return -1;
	}
	printf("OpenGL version %s\n", glGetString(GL_VERSION));

	// Shaders
	gfx::Shader basicShader("content/basic.vs", "content/basic.fs");

	basicShader.SendInt("uTexture1", 0);
	basicShader.SendInt("uTexture2", 1);

	hmm_m4 view = Mat4d(1.0f) * Translate(Vec3(0.0f, 0.0f, -4.0f));
	hmm_m4 projection = Perspective(45.0f, windowWidth / windowHeight, 0.1f, 100.0f);

	basicShader.SendMat4("view", (float*)&view);
	basicShader.SendMat4("projection", (float*)&projection);

	// Textures
	gfx::Texture wallTex("content/wall.jpg");
	gfx::Texture emojoTex("content/emojo.png");

	// Game stuff...
	f32 vertices[] = {
		// // positions         // colors        // texture coords
		//  0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		//  0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		// -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		// -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f

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

	hmm_vec3 positions[] = {
		Vec3( 0.0f,  0.0f,  0.0f), 
		Vec3( 2.0f,  5.0f, -15.0f), 
		Vec3(-1.5f, -2.2f, -2.5f),  
		Vec3(-3.8f, -2.0f, -12.3f),  
		Vec3( 2.4f, -0.4f, -3.5f),  
		Vec3(-1.7f,  3.0f, -7.5f),  
		Vec3( 1.3f, -2.0f, -2.5f),  
		Vec3( 1.5f,  2.0f, -2.5f), 
		Vec3( 1.5f,  0.2f, -1.5f), 
		Vec3(-1.3f,  1.0f, -1.5f)
	};

	// u32 indices[] = {
	// 	0, 1, 3, // first triangle
	// 	1, 2, 3 // second triangle
	// };

	// u32 VAO, VBO, IBO;
	u32 VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// glGenBuffers(1, &IBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, S_FLOAT(5), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, S_FLOAT(8), (void*)S_FLOAT(3));
	// glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, S_FLOAT(5), (void*)S_FLOAT(3));
	glEnableVertexAttribArray(2);

	// Game loop
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, windowWidth, windowHeight);
	while(!glfwWindowShouldClose(window)) {
		// Render game
		glClearColor(0.1f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		basicShader.Bind();

		glActiveTexture(GL_TEXTURE0);
		wallTex.Bind();
		glActiveTexture(GL_TEXTURE1);
		emojoTex.Bind();

		glBindVertexArray(VAO);
		for(u32 i = 0; i < 10; ++i) {
			hmm_m4 model = Mat4d(1.0f) * Translate(positions[i]);
			model = model * Rotate((float)glfwGetTime() * 50.0f * (i + 1), Vec3(1.0f, 0.3f, 0.5f));
			basicShader.SendMat4("model", (float*)&model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Update Window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	printf("bye sailor\n");
	return 0;
}