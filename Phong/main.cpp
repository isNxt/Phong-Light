#ifdef WIN32
#include <windows.h>
#endif

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "viewModule.h"
#include "PLY.h"
#include "geometry.h"

int window;
int updateFlag;

PLYObject *ply;

perspectiveData pD;

Vector4f initial_light_pos = { -100.0, 100.0, 100.0, 1.0 };
Vector4f ambient_light = { 0.3, 0.3, 0.3, 1.0 };
Vector4f light_color = { 0.6, 0.6, 0.6, 1.0 };
Vector4f black_color = { 0.0, 0.0, 0.0, 1.0 };

Vector3f light_pos, viewer_pos;

static int motionMode;
static int startX;
static int startY;
static GLfloat angle = 20;    /* in degrees */
static GLfloat angle2 = 30;   /* in degrees */

GLfloat current_pos[] = { 0.0, 0.0, 5.0 };

int flat = 0;
int light = 0;

extern PLYObject* ply;

void readKeyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case  0x1B:
	case  'q':
	case  'Q':
		exit(0);
		break;
	case '+':
		if (ply)
			ply->eat();
		break;
	case '-':
		if (ply)
			ply->starve();
		break;
	case 'd':
		if (ply)
			ply->dance();
		break;
	case 'i':
	case 'I':
		if (ply)
			ply->invertNormals();
		break;
	case 'l':
	case 'L':
		light = (light + 1) % 2;
		printf("%s lighting\n", (light ? "OpenGL" : "User"));
		break;
	case 't':
	case 'T':
		// PA4: Change some variable here...
		break;
	case 'r':
	case 'R':
		// reset initial view parameters
		angle = 20;
		angle2 = 30;
		current_pos[0] = 0.0;
		current_pos[1] = 0.0;
		current_pos[2] = 5.0;
		break;
	case 'h':
	case 'H':
		printf("\tPress q/Q for Quit\n");
		printf("\tPress h/H to print this help\n");
		printf("\tPress l/L to turn on/off Lighting\n");
		printf("\tPress i/I to invert the normals\n");
		printf("\tPress r/R to revert ViewPoint to initial position\n");
		printf("\tPress + to make the bunny grow fatter\n");
		printf("\tPress - to make the bunny grow thinner\n");
		printf("\tPress d/D to make the bunny dance randomly\n");
	default:
		break;
	}
	glutPostRedisplay();
}


void readSpecialKeys(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:

		break;
	case GLUT_KEY_DOWN:

		break;
	case GLUT_KEY_RIGHT:

		break;
	case GLUT_KEY_LEFT:

		break;
	}
	glutPostRedisplay();
}


void mouseButtHandler(int button, int state, int x, int y)
{
	motionMode = 0;

	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			motionMode = 1;		// Rotate object
			startX = x;
			startY = y;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			motionMode = 2;		// Translate object
			startX = x;
			startY = y;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			motionMode = 3;		// Zoom
			startX = x;
			startY = y;
		}
		break;
	}
	glutPostRedisplay();
}


void mouseMoveHandler(int x, int y)
{

	// No mouse button is pressed... return
	switch (motionMode) {
	case 0:
		return;
		break;

	case 1: // Calculate the rotations
		angle = angle + (x - startX);
		angle2 = angle2 + (y - startY);
		startX = x;
		startY = y;
		break;

	case 2:
		current_pos[0] = current_pos[0] - (x - startX) / 100.0;
		current_pos[1] = current_pos[1] - (y - startY) / 100.0;
		startX = x;
		startY = y;
		break;

	case 3:
		current_pos[2] = current_pos[2] - (y - startY) / 10.0;
		startX = x;
		startY = y;
		break;
	}

	glutPostRedisplay();
}


void cleanup(int sig)
{
	// insert cleanup code here (i.e. deleting structures or so)
	//fprintf(stderr,"Cleaning up\n");
	if (ply)
		delete(ply);
	exit(0);
}


//##########################################
// OpenGL Display function
//display 1
void display(void)
{
	int i, j;
	float M[16];
	Matrix4f m;

	glutSetWindow(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glGetFloatv(GL_MODELVIEW_MATRIX, M);
	for (i = 0; i < 3; i++) {
		m[i][3] = 0.0;
		m[3][i] = 0.0;
		for (j = 0; j < 3; j++) {
			m[i][j] = M[i * 4 + j];
		}
	}
	m[3][3] = 1.0;
	multVector(light_pos, m, initial_light_pos);
    multVector(viewer_pos, m, current_pos);
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(-current_pos[0], current_pos[1], -current_pos[2]);
	glRotatef(angle, 0.0, 1.0, 0.0);
	ply->draw();

	glutSwapBuffers();
}

/*display 2
void display(void)
{
	int i, j;
	float M[16];
	Matrix4f m;

	glutSetWindow(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(-current_pos[0], current_pos[1], -current_pos[2]);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, M);
	for (i = 0; i < 3; i++) {
		m[i][3] = 0.0;
		m[3][i] = 0.0;
		for (j = 0; j < 3; j++) {
			m[i][j] = M[i * 4 + j];
		}
	}
	m[3][3] = 1.0;
	multVector(light_pos, m, initial_light_pos);
	multVector(viewer_pos, m, current_pos);

	ply->draw();

	glutSwapBuffers();
}
*/


/*display 3
void display(void)
{
	int i, j;
	float M[16];
	Matrix4f m;

	glutSetWindow(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPushMatrix();
	glTranslatef(-current_pos[0], current_pos[1], -current_pos[2]);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, M);
	for (i = 0; i < 3; i++) {
		m[i][3] = 0.0;
		m[3][i] = 0.0;
		for (j = 0; j < 3; j++) {
			m[i][j] = M[i * 4 + j];
		}
	}
	m[3][3] = 1.0;
	multVector(light_pos, m, initial_light_pos);
	multVector(viewer_pos, m, current_pos);
	glPopMatrix();

	ply->draw();

	glutSwapBuffers();
}
*/
//##########################################
// Init display settings

void initDisplay()
{
	// Perspective projection parameters
	pD.fieldOfView = 45.0;
	pD.aspect = (float)IMAGE_WIDTH / IMAGE_HEIGHT;
	pD.nearPlane = 0.1;
	pD.farPlane = 50.0;

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(pD.fieldOfView, pD.aspect, pD.nearPlane, pD.farPlane);

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClearColor(0.0, 0, 0, 1.0);
	glClearIndex(0);
	glClearDepth(1);

	// setup lights
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black_color); // No global ambient light
	glLightfv(GL_LIGHT0, GL_POSITION, initial_light_pos);
	//  glLightfv(GL_LIGHT0, GL_AMBIENT, black_color);
	//  glLightfv(GL_LIGHT0, GL_DIFFUSE, black_color);
	//  glLightfv(GL_LIGHT0, GL_SPECULAR, black_color);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

//##########################################
// Main function

int main(int argc, char **argv)
{

	//signal(SIGHUP, cleanup);

	glutInit(&argc, argv);

	FILE *in;
	char *filename;

	filename = (char*)"lizhenxiout-repaired.ply";
	if (!(in = fopen(filename, "r"))) {
		fprintf(stderr, "Cannot open input file %s.\n", filename);
		exit(1);
	}

	ply = new PLYObject(in);
	ply->resize();
	srand(time(NULL));


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);

	window = glutCreateWindow("Assignment 4");

	glutDisplayFunc(display);
	glutKeyboardFunc(readKeyboard);
	glutMouseFunc(mouseButtHandler);
	glutMotionFunc(mouseMoveHandler);
	glutPassiveMotionFunc(mouseMoveHandler);
	glutIdleFunc(NULL);

	initDisplay();

	glutMainLoop();

	return 0;             /* ANSI C requires main to return int. */
}
