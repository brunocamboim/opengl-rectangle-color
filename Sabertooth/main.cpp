#include "System.h"

#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#include "TileMap.h"
#include "TileSet.h"
#include "Rectangle.h"

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::mat4 matrix_origem = glm::mat4(1);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}


int main() {
	

	if (!glfwInit()) {
		//fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	/* Caso necessário, definições específicas para SOs, p. e. Apple OSX *
	/* Definir como 3.2 para Apple OS X */
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
	GLFWwindow *g_window = glfwCreateWindow(
		SCR_WIDTH, SCR_HEIGHT, "Teste de versão OpenGL", NULL, NULL);
	if (!g_window) {
		//fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(g_window);
	// inicia manipulador da extensão GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	int height1 = SCR_WIDTH / 40;
	int width1 = SCR_WIDTH / 20;

	Rectangle *rectangle = new Rectangle[width1 * height1];

	//s rand(mudar semente)
	srand(time(NULL));
	for (int i = 0; i < width1 * height1; i++) {

		rectangle[i].preencher(rand() % 256, rand() % 256, rand() % 256);

	}

	float vertices[] = {
		// positions          // colors           // texture coords
		0.0f, 20.0f, 0.0f,   1.0f, 1.0f, // buttom right
		0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top left
		40.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
		40.0f, 20.0f, 0.0f,   0.0f, 1.0f //top right
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute		                //de quanto em quanto	//onde começa
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	const char* vertex_shader =
		"#version 410\n"
		"layout (location = 0) in vec2 vertex_position;"
		"layout (location = 1) in vec2 texture_mapping;"
		"out vec2 texture_coords;"
		
		"uniform mat4 matrix;"
		"uniform mat4 proj;"
		"uniform float layer_z;"
		"uniform float tamanho;"
		
		"void main () {"
			"texture_coords = texture_mapping;"
			"gl_Position = proj * matrix * vec4(vertex_position, layer_z, tamanho);"
		"}";

	const char* fragment_shader =
		"#version 410\n"
		"in vec2 texture_coords;"

		"uniform sampler2D sprite;"
		"uniform float offsetx;"
		"uniform float offsety;"
		"uniform float imagem;"

		"out vec4 frag_color;"

		"uniform vec3 color2;"

		"void main () {"
			/*"vec4 texel = texture (sprite, vec2((texture_coords.x + offsetx), texture_coords.y + offsety));"
			"if (texel.a < 0.5) {"
				"discard;"
			"}"*/
			"frag_color = vec4 (color2, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	// identifica fs e o associa com fragment_shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	// identifica do programa, adiciona partes e faz "linkagem"
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glUseProgram(shader_programme);
	glfwSetKeyCallback(g_window, key_callback);
	
	glm::mat4 proj = glm::ortho(0.0f, (float) SCR_WIDTH, (float) SCR_HEIGHT, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(
		glGetUniformLocation(shader_programme, "proj"), 1,
		GL_FALSE, glm::value_ptr(proj));

	float speed = 1.0f;
	float lastPosition = 0.0f;

	glm::mat4 matrix_triangle = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
	matrix_triangle = glm::translate(glm::mat4(1), glm::vec3(20, 40, 0.0f));

	int teste = false;
	while (!glfwWindowShouldClose(g_window))
	{
		processInput(g_window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		static float previousSeconds = glfwGetTime();
		float currentSeconds = glfwGetTime();
		float elapsedSeconds =
			currentSeconds - previousSeconds;
		previousSeconds = currentSeconds;
		
		glUseProgram(shader_programme);


		glBindVertexArray(VAO);

		for (int i = 0; i < 2; i++) {
			
			if (!teste) {
				matrix_triangle = glm::translate(matrix_triangle, glm::vec3((float)i, 0, 0.0f));
				glUniformMatrix4fv(
					glGetUniformLocation(shader_programme, "matrix"), 1,
					GL_FALSE, glm::value_ptr(matrix_triangle));
			}
			else {
				glUniformMatrix4fv(
					glGetUniformLocation(shader_programme, "matrix"), 1,
					GL_FALSE, glm::value_ptr(glm::mat4(1)));
			}

			glm::vec3 lightColor(0.0f, 1.0f, 0.0f);
			glUniform3fv(glGetUniformLocation(shader_programme, "color2"), 1,
				glm::value_ptr(lightColor));

			
			glUniform1f(
				glGetUniformLocation(shader_programme, "tamanho"), 1);
			
			glUniform1f(
				glGetUniformLocation(shader_programme, "layer_z"), -0.45);

			//glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
			/*glUniform1f(
				glGetUniformLocation(shader_programme, "offsetx"), 0);

			glUniform1f(
				glGetUniformLocation(shader_programme, "offsety"), 1);*/
			/*glUniform1f(
				glGetUniformLocation(shader_programme, "imagem"), 1);*/

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}		

		teste = true;

		glfwSwapBuffers(g_window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;

}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}