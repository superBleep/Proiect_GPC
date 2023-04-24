/*
* Proiect Grafica Pe Calculator
* UNIBUC, FMI, CTI, Anul III, Grupa 361
* Croitoru Eduard-Adrian
* Florian Luca-Paul
*/
#include <windows.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;

double idxGropi = 0.;
double viteza = 2.;
double fazaFar = 1.;
double sirena = 0.5;

double xcar = 0., ycar = 0.;
// unde se va opri masina pe axa x, cand face schimbarea de benzi
double xFinal = 100;
double j = 1.0;
GLsizei winWidth = 500, winHeight = 500;
GLuint traficID;
static GLfloat rotTheta = 0.0;

class scrPt {
	public:
		GLfloat x, y, z;

		void print(const char* text = " ") {
			cout << text << ' ' << x << ' ' << y << ' ' << z << endl;
		}

		scrPt operator + (scrPt const& a) {
			scrPt res;
			res.x = x + a.x;
			res.y = y + a.y;
			res.z = z + a.z;
			return res;
		}

		scrPt operator - (scrPt const& a) {
			scrPt res;
			res.x = x - a.x;
			res.y = y - a.y;
			res.z = z - a.z;
			return res;
		}
};

void funcTranzitii(void) {
	// Girofar
	j += 10;

	// Gropi
	idxGropi -= viteza;
	if (idxGropi < -500.) {
		idxGropi = 500.;
	}

	glutPostRedisplay();
}

static void init(void) {
	scrPt cerc;
	GLdouble theta;
	GLint aux;
	GLint n = 2000;

	// 159 / 255 , 61 / 255 , 0 , 1
	glClearColor(0.6235294, 0.239215, 0, 1);
	gluOrtho2D(-100, 100, -100, 100);

	traficID = glGenLists(1);

	for (aux = 0; aux < 20; aux++)
	{
		glColor3f(0, 1, 0);
		glRectf(20, 100 + aux * 20, 60, 160 + aux * 20);

		glColor3f(0.5, 0, 0.5);
		glRectf(-120, 250 + aux * 20, -80, 310 + aux * 20);
	}
	glEndList();

	// Opacitatea culorilor
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
			ycar += 5;
			break;
		case 'a':
			xcar -= 5;
			if (xcar < -220) xcar = -220;
			break;
		case 's':
			ycar -= 5;
			break;
		case 'd':
			xcar += 5;
			if (xcar > 120) xcar = 120;
			break;
		case 'f':
			if (fazaFar - 0.2 <= 0.) fazaFar = -1.;
			else fazaFar -= 0.2;
			if (fazaFar == -1.) fazaFar = 1.;
			break;
		case 'g':
			if (sirena == 0.5) sirena = 0.;
			else sirena = 0.5;
		default:
			break;
	}
}

void genGroapa(int origx, int origy) {
	int range = 100, min = 0, nrVf = rand() % 10 + 4;

	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_POLYGON);
		for (int iter = 0; iter < nrVf; iter++) {
			glVertex2i(rand() % range + min + origx, rand() % range + min + origy);
		}
	glEnd();
}

void desenDrum(void) {
	srand(2);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ----- DRUMUL -----
	glColor3f(0.3, 0.3, 0.3);
	glRectf(-200., -1000., 200., 1000.);

	// Linia ce separa benzile
	glColor3f(1, 1, 1);
	glRectf(-10., -1000., 10., 1000.);

	// Gropi
	glPushMatrix();
	glTranslated(0, idxGropi, 0);
	genGroapa(20, 20);
	genGroapa(-200, 100);

	// Initializare pentru tranzitii
	glPopMatrix();
	glTranslated(xcar, ycar, 0);

	// ----- MASINA DE POLITIE -----
	// Sasiu
	glColor3f(0, 0, 0);
	glRectf(20, -180, 80, -155);
	glColor3f(1, 1, 1);
	glRectf(20, -155, 80, -115);
	glColor3f(0, 0, 0);
	glRectf(20, -115, 80, -90);

	// Stopuri
	glColor3f(1, 0, 0);
	glRectf(25, -185, 35, -180);
	glColor3f(1, 0, 0);
	glRectf(65, -185, 75, -180);

	// Faruri
	glColor3f(0.8, 0.5, 0);
	glRectf(25, -90, 35, -85);
	glColor3f(0.8, 0.5, 0);
	glRectf(65, -90, 75, -85);

	glColor4f(1, 1, 0, fazaFar);
	glBegin(GL_TRIANGLES);
		glVertex2f(30, -85);
		glVertex2f(0, 0);
		glVertex2f(60, 0);

		glVertex2f(40, 0);
		glVertex2f(100, 0);
		glVertex2f(70, -85);
	glEnd();


	// ----- GIROFAR -----
	// Animatie
	glTranslated(50, -135, 0);
	glRotated(j, 0, 0, 1);
	glTranslated(-50, 135, 0);

	// Girofarul efectiv
	glColor4f(1, 0, 0, sirena);
	glBegin(GL_TRIANGLES);
		glVertex2f(10, -115);
		glVertex2f(50, -135);
		glVertex2f(10, -155);
	glEnd();

	glColor4f(0, 0, 1, sirena);
	glBegin(GL_TRIANGLES);
		glVertex2f(50, -135);
		glVertex2f(90, -115);
		glVertex2f(90, -155);
	glEnd();

	glPopMatrix();
	glPopMatrix();

	// Incrementare pentru girofar si alte translatii
	glutIdleFunc(funcTranzitii);

	glutSwapBuffers();
	glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Need for Speed Chiajna");

	init();

	glutDisplayFunc(desenDrum);
	glutReshapeFunc(winReshapeFcn);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}