#include "Object.h"

#include "Transform.h"
#include "Sprite.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowSize((GLUT_SCREEN_WIDTH - WINDOW_WIDTH) / 2, (GLUT_SCREEN_HEIGHT - WINDOW_HEIGHT) / 2);
	glutCreateWindow("EcoEngine");

	if (glewInit() != GLEW_OK) return 0;
}