// cc `pkg-config --cflags glfw3` -o test test.c `pkg-config --static --libs glfw3`
#include <stdio.h>
#include <stdlib.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

GLfloat vertex_data[] = { -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 0.0, 1.0, 0.0 };
GLfloat color_data[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };

  GLuint prog;
  GLuint vert;
  GLuint frag;

const char* vert_shader =
  "\
#version 150\n\
\n\
in vec3 position;\n\
in vec3 color;\n\
out vec3 vcol;\n\
\n\
void main(void) { \n\
    vcol = color;\n\
    gl_Position = vec4(position, 1.0);\n\
}\n\
";

const char* frag_shader =
  "\
#version 150\n\
\n\
in vec3 vcol;\n\
out vec4 fcol;\n\
\n\
void main(void) { \n\
    fcol = vec4(vcol,1.0);\n\
}\n\
";

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int code, int action, int mods) {
  if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}


void init(void) {
  GLuint vao;
  GLuint buffs[2];
  glGenVertexArrays(1, &vao);
  glGenBuffers(2, buffs);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0); // 0 position
  glEnableVertexAttribArray(1); // 1 color

  glBindBuffer(GL_ARRAY_BUFFER, buffs[0]);
  glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, buffs[1]);
  glBufferData(GL_ARRAY_BUFFER, 3*3*sizeof(GLfloat), color_data, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


  prog = glCreateProgram();

  vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_shader, 0);
  glCompileShader(vert);
  glAttachShader(prog, vert);

  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_shader, 0);
  glCompileShader(frag);
  glAttachShader(prog, frag);

  glBindAttribLocation(prog, 0, "position");
  glBindAttribLocation(prog, 1, "color");

  glLinkProgram(prog);
  glUseProgram(prog);
}

void render(void) {
}

int main(int argc, char *argv[]) {
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(800, 600, "CCraft", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  init();

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDetachShader(prog, vert);
  glDetachShader(prog, frag);
  glDeleteShader(vert);
  glDeleteShader(frag);
  glDeleteProgram(prog);
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
