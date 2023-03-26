#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glut.h"


#define dim 300
#define PI (4 * atan(1.0))

unsigned char prevKey;

class C2coord
{
public:
	C2coord()
	{
		m.x = m.y = 0;
	}

	C2coord(double x, double y)
	{
		m.x = x;
		m.y = y;
	}

	C2coord(const C2coord& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
	}

	C2coord& operator=(C2coord& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator==(C2coord& p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

public:
	struct SDate
	{
		double x, y;
	} m;
};

class CPunct : public C2coord
{
public:
	CPunct() : C2coord(0.0, 0.0)
	{}

	CPunct(double x, double y) : C2coord(x, y)
	{}

	CPunct& operator=(const CPunct& p)
	{
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	void getxy(double& x, double& y)
	{
		x = m.x;
		y = m.y;
	}

	void setx(double x)
	{
		m.x = x;
	}

	void sety(double y)
	{
		m.y = y;
	}

	int operator==(CPunct& p)
	{
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	void marcheaza()
	{
		glBegin(GL_POINTS);
		glVertex2d(m.x, m.y);
		glEnd();
	}

	void print(FILE* fis)
	{
		fprintf(fis, "(%+f,%+f)", m.x, m.y);
	}

};

class CartesianGrid {
private:
	int squareSize;
	int rows;

	CPunct ConvertToCartezian(CPunct p) {
		float width = 300;
		float height = 300;

		CPunct convertedPoint;
		convertedPoint.setx(2 * ((-width / 2) + p.m.x * squareSize) / (width + 10));
		convertedPoint.sety(2 * ((height / 2) - p.m.y * squareSize) / (height + 10));
		return convertedPoint;
	}
	void WritePixel(CPunct p) {

		printf("x:%f, y:%f\n", p.m.x, p.m.y);
		glEnable(GL_POINT_SMOOTH);
		glColor3f(0.1, 0.1, 1);
		glPointSize(8);


		glBegin(GL_POINTS);
		glVertex2f(p.m.x, p.m.y);
		glEnd();
	}
public:
	CartesianGrid(int r) {
		float width = 300;
		float height = 300;
		rows = r;
		squareSize = height / r;
	}

	void DrawGrid() {
		float width = 300;
		float height = 300;

		glLineWidth(1);
		glColor3f(0, 0, 0);
		for (float i = -width / 2; i <= width / 2; i += squareSize) {
			glBegin(GL_LINES);
			glVertex2f(2 * i / (width + 10), (float)height / (height + 10));
			glVertex2f(2 * i / (width + 10), (float)-height / (height + 10));
			glEnd();
		}
		for (float i = height / 2; i >= -height / 2; i -= squareSize) {
			glBegin(GL_LINES);
			glVertex2f((float)width / (width + 10), 2 * i / (height + 10));
			glVertex2f((float)-width / (width + 10), 2 * i / (height + 10));
			glEnd();
		}
	}

	void DrawLine(CPunct p1, CPunct p2, int dottedLineWidth) {

		CPunct p1Cartezian, p2Cartezian;
		p1Cartezian = ConvertToCartezian(p1);
		p2Cartezian = ConvertToCartezian(p2);
		glLineWidth(2);
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		glVertex2f(p1Cartezian.m.x, p1Cartezian.m.y);
		glVertex2f(p2Cartezian.m.x, p2Cartezian.m.y);
		glEnd();

		DrawLineAlgorithm(p1, p2, dottedLineWidth);
	}

	void DrawLineAlgorithm(CPunct p1, CPunct p2, int dottedLineWidth) {
		if (fabs(p2.m.y - p1.m.y) < fabs(p2.m.x - p1.m.x)) {
			if (p1.m.x > p2.m.x) {
				DrawDottedHLine(p2, p1, dottedLineWidth);
			}
			else {
				DrawDottedHLine(p1, p2, dottedLineWidth);
			}
		}
		else {
			if (p1.m.y > p2.m.y) {
				DrawDottedVLine(p2, p1, dottedLineWidth);
			}
			else {
				DrawDottedVLine(p1, p2, dottedLineWidth);
			}
		}
	}

	void DrawDottedVLine(CPunct p1, CPunct p2, int dottedLineWidth) {
		int dx = p2.m.x - p1.m.x;
		int dy = p2.m.y - p1.m.y;
		int xi = 1;

		if (dx < 0) {
			xi = -1;
			dx = -dx;
		}

		int d = 2 * dx - dy;
		int x = p1.m.x;

		glBegin(GL_POINTS);
		for (int y = p1.m.y; y <= p2.m.y; y++) {
			CPunct convertedPoint = ConvertToCartezian(CPunct(x, y));
			WritePixel(convertedPoint);
			for (int i = 0; i < (dottedLineWidth - 1) / 2;i++) {
				printf("i = %d\n", i);
				CPunct convertedPoint = ConvertToCartezian(CPunct(x + i + 1, y));
				WritePixel(convertedPoint);
			}
			for (int i = 0; i < (dottedLineWidth - 1) / 2;i++) {
				printf("i = %d\n", i);
				CPunct convertedPoint = ConvertToCartezian(CPunct(x - i - 1, y));
				WritePixel(convertedPoint);
			}

			if (d > 0) {
				x = x + xi;
				d = d + (2 * (dx - dy));
			}
			else {
				d = d + 2 * dx;
			}
		}
		glEnd();
	}

	void DrawDottedHLine(CPunct p1, CPunct p2, int dottedLineWidth) {

		int dx = p2.m.x - p1.m.x;
		int dy = p2.m.y - p1.m.y;

		int yi = 1;

		if (dy < 0) {
			yi = -1;
			dy = -dy;
		}

		int d = (2 * dy) - dx;
		int y = p1.m.y;

		glBegin(GL_POINTS);
		for (int x = p1.m.x; x <= p2.m.x; x++) {
			CPunct convertedPoint = ConvertToCartezian(CPunct(x, y));
			WritePixel(convertedPoint);
			for (int i = 0; i < (dottedLineWidth - 1) / 2;i++) {
				printf("i = %d\n", i);
				CPunct convertedPoint = ConvertToCartezian(CPunct(x, y + i + 1));
				WritePixel(convertedPoint);
			}
			for (int i = 0; i < (dottedLineWidth - 1) / 2;i++) {
				printf("i = %d\n", i);
				CPunct convertedPoint = ConvertToCartezian(CPunct(x, y - i - 1));
				WritePixel(convertedPoint);
			}

			if (d > 0) {
				y = y + yi;
				d = d + (2 * (dy - dx));
			}
			else {
				d = d + 2 * dy;
			}
		}
		glEnd();
	}

	void DrawCircleArc(CPunct p, float radius) {
		glColor3f(1, 0, 0);
		glLineWidth(4);
		// Draw the circle

		glBegin(GL_LINE_LOOP);
		float r = radius / ((float)rows / 2);
		for (float angle = 0; angle < 2 * PI; angle += 0.001) {
			glVertex2f(r * cos(angle) - 1, r * sin(angle) - 1);
		}
		glEnd();


		DrawDottedCircleArc(p, radius);
	}

	void DrawDottedCircleArc(CPunct p, float radius) {

		float x = 0;
		float y = radius;

		float d = 3 - 2 * radius;
		float dE = 3;
		float dSE = -2 * radius + 5;

		while (y > x) {
			if (d < 0) {
				d += dE;
				dE += 2;
				dSE += 2;

			}
			else {
				d += dSE;
				dE += 2;
				dSE += 4;
				y--;
			}

			x++;

			CPunct convertedPoint = ConvertToCartezian(CPunct(y, rows - x + 1));
			WritePixel(convertedPoint);
			convertedPoint = ConvertToCartezian(CPunct(y + 1, rows - x + 1));
			WritePixel(convertedPoint);
			convertedPoint = ConvertToCartezian(CPunct(y - 1, rows - x + 1));
			WritePixel(convertedPoint);
		}
	}


};

void Display1(CartesianGrid grid) {

	CPunct p1(0, 0), p2(15, 5);
	grid.DrawLine(p1, p2, 3);
	CPunct p3(0, 15), p4(15, 8);
	grid.DrawLine(p3, p4, 1);

}

void Display2(CartesianGrid grid) {

	grid.DrawCircleArc(CPunct(0, 15), 13);
}


void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	CartesianGrid cg(15);
	cg.DrawGrid();
	switch (prevKey) {
	case '1':
		Display1(cg);
		break;
	case '2':
		Display2(cg);
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}
