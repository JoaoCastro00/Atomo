// OpenGL (EXTENSÕES)
#include <GL/glew.h>
//tempo
#include <chrono>
// Janela / Input
#include <GLFW/glfw3.h>
// Matemática (vetores, matrizes)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// STL (C++)
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
// Definição de PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace glm;
struct Vetor2D {  double x, y;};

class AtomSim {
private:
	//Constante 
	const double K = 100.0; //Constante de força
	const double massa = 1.0; //massa do elétron

	Vetor2D pos, vel; //posição do elétron
public:
	AtomSim(double r, double v_inicial) {
		pos = { r, 0 }; // Começa a distancia r no eixo x
		vel = { 0, v_inicial }; //velocidade perpendicular (eixo y) para orbitar
	}

	void atualizar(float deltatime) {


		// Calcular a distancia ao nucleo (0.0)
		double r = std::sqrt (pos.x * pos.x + pos.y * pos.y);
		if (r < 1e-6) r = 1e-6;

		//calcular a força de atração (Lei de coloumb: F = K / r^2)
		double forca_mag = K / (r * r);

		//direção da força (vetor unitário apontado para o centro)
		double ux = -pos.x / r;
		double uy = -pos.y / r;

		//Aceleração (F = m * a -> a = F/m)
		double ax = (forca_mag * ux) / massa;
		double ay = (forca_mag * uy) / massa;

		//atualizar Velocidade (v = v + a * deltatime)
		vel.x += ax * deltatime;
		vel.y += ay * deltatime;

		//Atualizar Posição (s = s + v * deltatime)
		pos.x += vel.x * deltatime;
		pos.y += vel.y * deltatime;

	}
Vetor2D getPos() const {
		return pos;
}

};



struct Engine {
	GLFWwindow* window;
	int WIdeltatimeH = 800, HEIGHT = 800;

	Engine() {
		if (!glfwInit()) {
			std::cout<< "failed to init glfw, LOL";
			exit(EXIT_FAILURE);
		}

			// ---- Create Window ----
			window = glfwCreateWindow(WIdeltatimeH, HEIGHT, "Átomo 2D por john", nullptr, nullptr);
		if (!window) {
			std::cout << "Falha ao criar janela, KKKKK";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
		
		glViewport(0, 0, WIdeltatimeH, HEIGHT);
		
		glfwSwapInterval(1);
	}
};

void Nucleo(float cx,float cy,  float r, int segments) {
	glBegin(GL_TRIANGLE_FAN);

	glVertex2f(cx, cy); // centro


	for (int i = 0; i <= segments; i++) {
		float angle = 2.0f * M_PI * i / segments;
		float x = r * cos(angle);
		float y = r * sin(angle);

		glVertex2f(cx + x, cy + y);
	}
	glEnd();
}

void Eletron(float cx, float cy, float r, int segments) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(cx, cy);

	for (int i = 0; i <= segments; i++) {
		float angle = 2.0f * M_PI * i / segments;
		glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
	}
	glEnd();
}

void drawAtom(float ex, float ey) {
		
		
		//Nucleo
		glColor3f(1.0f, 0.0f, 0.0f);
		Nucleo(0.0f, 0.0f, 0.1f, 100);


		//Elétron
		glColor3f(0.0f, 0.5, 1.0f);
		Eletron(ex, ey, 0.05f, 50);

		
}




//--   Main   --
int main() {
	Engine engine;

	double r0 = 0.5;
	double v0 = std::sqrt(100.0 / r0);// V_orbital = sqrt(K/r)
	AtomSim eletron(r0, v0); 

	using clock = std::chrono::steady_clock;

	auto lastTime = clock::now();


	while (!glfwWindowShouldClose(engine.window)) {
		
		auto currentTime = clock::now();

		std::chrono::duration<float> delta = currentTime - lastTime;
		float deltaTime = delta.count();

		lastTime = currentTime;


		glClear(GL_COLOR_BUFFER_BIT);
		eletron.atualizar(deltaTime);
		Vetor2D p = eletron.getPos();
		drawAtom((float)p.x, (float)p.y);
		glfwSwapBuffers(engine.window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}
