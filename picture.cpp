/* Standard A,S,D,W to move around.
 Space bar to increase Y value (move up)
 'C' to check current X, Z coordinates of camera
 */

#include <cstdlib>
#include <cmath>
#include <OpenGL/gl3.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <gl/freeglut.h>
#define PI 3.14159265w

#include <cstdlib>
#include <iostream>
#include <vector>
#define PI 3.14159265
using namespace std;

void drawScene();
void setup();
class Building;
class drawTree;
class drawPond;
//variables for camera positioning
float eyex = 0, eyey = 10, eyez = 0, cenx = 9000, ceny = 5, cenz = 0, fieldOfView = 1000, amt = 2, upVx = 0, upVy = 1, upVz = 0, upAngle = 0;
float deltaAnglex = 0.0f, deltaAngley = 0.0f, anglex = 628.31f, angley = 0.0f;
int xOrigin = -1, yOrigin = -1;

//Growing/Animated Tree variabiles
float treeGrowthHeight = 1;
float treeGrowthWidth = 1;
float treeGrowthRate = .5;

//keyboard vars
bool* keyStates = new bool[256];

void normalize(float *v) {
    float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    for (int i = 0; i < 3; i++) {
        v[i] = v[i] / length;
    }
}

float *cross_product(float *a, float *b) {
    float result[] = { a[1] * b[2] - a[2] * b[1], -(a[0] * b[2] - a[2] * b[0]),
        a[0] * b[1] - a[1] * b[0] };

    normalize(result);

    return result;
}

float *calculate_normal(float vector1X, float vector1Y, float vector1Z,float vector2X, float vector2Y, float vector2Z,float vector3X, float vector3Y, float vector3Z) {
    float x[] = { vector2X - vector1X,vector2Y - vector1Y,vector2Z - vector1Z };
    float y[] = { vector3X - vector1X,vector3Y - vector1Y,vector3Z - vector1Z };

    float *result = cross_product(x, y);

    return result;
}

void drawCircle(GLfloat cx, GLfloat cy, GLfloat cz, GLfloat radius, GLfloat numberOfSides)
{
    glBegin(GL_TRIANGLE_FAN);
    for (int ii = 0; ii < numberOfSides; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(numberOfSides);//get the current angle

        float x = radius * cosf(theta);//calculate the x component
        float y = radius * sinf(theta);//calculate the y component

        glVertex3f(x + cx, 0.1, y + cy);//output vertex
        glNormal3f(0, 1, 0);
    }
    glEnd();
}

class drawPark {
public:
    float x, y, z, r, g, b;
public:
    drawPark(float ox, float oy, float oz, float red, float green, float blue) {
        x = ox;
        y = oy;
        z = oz;
        r = red;
        g = green;
        b = blue;
    }
    void draw() {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);

        glVertex3f(x - 10, y + .01, z + 285);
        glNormal3fv(calculate_normal(x - 10, y + .01, z + 285, x - 10, y + .01, z - 240, x + 210, y + .01, z + 285));
        glVertex3f(x - 10, y + .01, z - 240);
        glNormal3fv(calculate_normal(x - 10, y + .01, z + 285, x - 10, y + .01, z - 240, x + 210, y + .01, z + 285));
        glVertex3f(x + 210, y + .01, z - 240);
        glNormal3fv(calculate_normal(x - 10, y + .01, z + 285, x - 10, y + .01, z - 240, x + 210, y + .01, z + 285));
        glVertex3f(x + 210, y + .01, z + 285);
        glNormal3fv(calculate_normal(x - 10, y + .01, z + 285, x - 10, y + .01, z - 240, x + 210, y + .01, z + 285));
        glEnd();
    }
};
//Park drawing set to Global variable
drawPark park = drawPark(0, 0, 0, 0, 0, 0);

vector<drawTree> trees;
class drawTree {
public:
    float x, z, h, w, d, r, g, b;
public:
    drawTree(float ox, float oz, float height, float width, float red, float green, float blue) {
        x = ox;
        z = oz;
        h = height;
        w = width;
        d = 7;
        r = red;
        g = green;
        b = blue;
    }
    void draw() {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);

        // Top-face
        glVertex3f(x - (w/8), h / 3, z - (w/8));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/8), h / 3, z - (w/8));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/8), h / 3, z + (w/8));
        glNormal3f(0, 0, 0);
        glVertex3f(x - (w/8), h / 3, z + (w/8));
        glNormal3f(0, 0, 0);

        // Bottom-face
        glVertex3f(x - (w/8), 0, z - (w/8));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/8), 0, z - (w/8));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/8), 0, z + (w/8));
        glNormal3f(0, 0, 0);
        glVertex3f(x - (w/8), 0, z + (w/8));
        glNormal3f(0, 0, 0);

        // Front-face
        glVertex3f(x - (w/8), 0, z - (w/8));
        glNormal3f(0, 0, 1);
        glVertex3f(x + (w/8), 0, z - (w/8));
        glNormal3f(0, 0, 1);
        glVertex3f(x + (w/8), h / 3, z - (w/8));
        glNormal3f(0, 0, 1);
        glVertex3f(x - (w/8), h / 3, z - (w/8));
        glNormal3f(0, 0, 1);

        // Back-face
        glVertex3f(x - (w/8), 0, z + (w/8));
        glNormal3f(0, 0, -1);
        glVertex3f(x + (w/8), 0, z + (w/8));
        glNormal3f(0, 0, -1);
        glVertex3f(x + (w/8), h / 3, z + (w/8));
        glNormal3f(0, 0, -1);
        glVertex3f(x - (w/8), h / 3, z + (w/8));
        glNormal3f(0, 0, -1);

        // Left-face
        glVertex3f(x - (w/8), 0, z + (w/8));
        glNormal3f(-1, 0, 0);
        glVertex3f(x - (w/8), 0, z - (w/8));
        glNormal3f(-1, 0, 0);
        glVertex3f(x - (w/8), h / 3, z - (w/8));
        glNormal3f(-1, 0, 0);
        glVertex3f(x - (w/8), h / 3, z + (w/8));
        glNormal3f(-1, 0, 0);

        // Right-face
        glVertex3f(x + (w/8), 0, z + (w/8));
        glNormal3f(1, 0, 0);
        glVertex3f(x + (w/8), 0, z - (w/8));
        glNormal3f(1, 0, 0);
        glVertex3f(x + (w/8), h / 3, z - (w/8));
        glNormal3f(1, 0, 0);
        glVertex3f(x + (w/8), h / 3, z + (w/8));
        glNormal3f(1, 0, 0);

        glEnd(); // of the color cube

        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex3f(x - (w/2), h / 3, z - (w/2));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/2), h / 3, z - (w/2));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/2), h / 3, z + (w/2));
        glNormal3f(0, 0, 0);
        glVertex3f(x - (w/2), h / 3, z + (w/2));
        glNormal3f(0, 0, 0);
        glEnd();

        glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front

        glVertex3f(x - (w/2), h / 3, z - (w/2));
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z - (w/2)), (x + (w/2)), (h / 3), (z - (w/2)), x,
                                     (h / 3 * 2), z));
        glVertex3f(x + (w/2), h / 3, z - (w/2));
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z - (w/2)), (x + (w/2)), (h / 3), (z - (w/2)), x,
                                     (h / 3 * 2), z));
        glVertex3f(x, (h / 3 * 2), z);
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z - (w/2)), (x + (w/2)), (h / 3), (z - (w/2)), x,
                                     (h / 3 * 2), z));

        // Right

        glVertex3f(x + (w/2), h / 3, z - (w/2));
        glNormal3fv(calculate_normal((x + (w/2)), (h / 3), (z - (w/2)), (x + (w/2)), (h / 3), (z + (w/2)), x,
                                     (h / 3 * 2), z));
        glVertex3f(x + (w/2), h / 3, z + (w/2));
        glNormal3fv(calculate_normal((x + (w/2)), (h / 3), (z - (w/2)), (x + (w/2)), (h / 3), (z + (w/2)), x,
                                     (h / 3 * 2), z));
        glVertex3f(x, (h / 3 * 2), z);
        glNormal3fv(calculate_normal((x + (w/2)), (h / 3), (z - (w/2)), (x + (w/2)), (h / 3), (z + (w/2)), x,
                                     (h / 3 * 2), z));

        // Back

        glVertex3f(x - (w/2), h / 3, z + (w/2));
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z + (w/2)), (x + (w/2)), (h / 3), (z + (w/2)), x,
                                     (h / 3 * 2), z));
        glVertex3f(x + (w/2), h / 3, z + (w/2));
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z + (w/2)), (x + (w/2)), (h / 3), (z + (w/2)), x,
                                     (h / 3 * 2), z));
        glVertex3f(x, (h / 3 * 2), z);
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z + (w/2)), (x + (w/2)), (h / 3), (z + (w/2)), x,
                                     (h / 3 * 2), z));

        // Left

        glVertex3f(x - (w/2), h / 3, z + (w/2));
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z + (w/2)), (x - (w/2)), (h / 3), (z - (w/2)), x,
                                     (h / 3 * 2), z));
        glVertex3f(x - (w/2), h / 3, z - (w/2));
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z + (w/2)), (x - (w/2)), (h / 3), (z - (w/2)), x,
                                     (h / 3 * 2), z));
        glVertex3f(x, (h / 3 * 2), z);
        glNormal3fv(calculate_normal((x - (w/2)), (h / 3), (z + (w/2)), (x - (w/2)), (h / 3), (z - (w/2)), x,
                                     (h / 3 * 2), z));
        glEnd();

        glBegin(GL_QUADS);
        glVertex3f(x - (w/3), h / 2, z - (w/3));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/3), h / 2, z - (w/3));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/3), h / 2, z + (w/3));
        glNormal3f(0, 0, 0);
        glVertex3f(x - (w/3), h / 2, z + (w/3));
        glNormal3f(0, 0, 0);
        glEnd();

        glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front

        glVertex3f(x - (w/3), h / 2, z - (w/3));
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z - (w/3)), (x + (w/3)), (h / 2), (z - (w/3)), x, (h * 3 / 4), z));
        glVertex3f(x + (w/3), h / 2, z - (w/3));
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z - (w/3)), (x + (w/3)), (h / 2), (z - (w/3)), x, (h * 3 / 4), z));
        glVertex3f(x, (h * 3 / 4), z);
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z - (w/3)), (x + (w/3)), (h / 2), (z - (w/3)), x, (h * 3 / 4), z));

        // Right

        glVertex3f(x + (w/3), h / 2, z - (w/3));
        glNormal3fv(calculate_normal((x + (w/3)), (h / 2), (z - (w/3)), (x + (w/3)), (h / 2), (z + (w/3)), x, (h * 3 / 4), z));
        glVertex3f(x + (w/3), h / 2, z + (w/3));
        glNormal3fv(calculate_normal((x + (w/3)), (h / 2), (z - (w/3)), (x + (w/3)), (h / 2), (z + (w/3)), x, (h * 3 / 4), z));
        glVertex3f(x, (h * 3 / 4), z);
        glNormal3fv(calculate_normal((x + (w/3)), (h / 2), (z - (w/3)), (x + (w/3)), (h / 2), (z + (w/3)), x, (h * 3 / 4), z));

        // Back

        glVertex3f(x - (w/3), h / 2, z + (w/3));
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z + (w/3)), (x + (w/3)), (h / 2), (z + (w/3)), x, (h * 3 / 4), z));
        glVertex3f(x + (w/3), h / 2, z + (w/3));
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z + (w/3)), (x + (w/3)), (h / 2), (z + (w/3)), x, (h * 3 / 4), z));
        glVertex3f(x, (h * 3 / 4), z);
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z + (w/3)), (x + (w/3)), (h / 2), (z + (w/3)), x, (h * 3 / 4), z));

        // Left

        glVertex3f(x - (w/3), h / 2, z + (w/3));
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z + (w/3)), (x - (w/3)), (h / 2), (z - (w/3)), x, (h * 3 / 4), z));
        glVertex3f(x - (w/3), h / 2, z - (w/3));
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z + (w/3)), (x - (w/3)), (h / 2), (z - (w/3)), x, (h * 3 / 4), z));
        glVertex3f(x, (h * 3 / 4), z);
        glNormal3fv(calculate_normal((x - (w/3)), (h / 2), (z + (w/3)), (x - (w/3)), (h / 2), (z - (w/3)), x, (h * 3 / 4), z));
        glEnd();

        glBegin(GL_QUADS);
        glVertex3f(x - (w/4), (h * 2 / 3), z - (w/4));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/4), (h * 2 / 3), z - (w/4));
        glNormal3f(0, 0, 0);
        glVertex3f(x + (w/4), (h * 2 / 3), z + (w/4));
        glNormal3f(0, 0, 0);
        glVertex3f(x - (w/4), (h * 2 / 3), z + (w/4));
        glNormal3f(0, 0, 0);
        glEnd();

        glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front

        glVertex3f(x - (w/4), (h * 2 / 3), z - (w/4));
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z - (w/4)), (x + (w/4)), (h * 2 / 3), (z - (w/4)), x, h, z));
        glVertex3f(x + (w/4), (h * 2 / 3), z - (w/4));
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z - (w/4)), (x + (w/4)), (h * 2 / 3), (z - (w/4)), x, h, z));
        glVertex3f(x, h, z);
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z - (w/4)), (x + (w/4)), (h * 2 / 3), (z - (w/4)), x, h, z));

        // Right

        glVertex3f(x + (w/4), (h * 2 / 3), z - (w/4));
        glNormal3fv(calculate_normal((x + (w/4)), (h * 2 / 3), (z - (w/4)), (x + (w/4)), (h * 2 / 3), (z + (w/4)), x, h, z));
        glVertex3f(x + (w/4), (h * 2 / 3), z + (w/4));
        glNormal3fv(calculate_normal((x + (w/4)), (h * 2 / 3), (z - (w/4)), (x + (w/4)), (h * 2 / 3), (z + (w/4)), x, h, z));
        glVertex3f(x, h, z);
        glNormal3fv(calculate_normal((x + (w/4)), (h * 2 / 3), (z - (w/4)), (x + (w/4)), (h * 2 / 3), (z + (w/4)), x, h, z));

        // Back

        glVertex3f(x - (w/4), (h * 2 / 3), z + (w/4));
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z + (w/4)), (x + (w/4)), (h * 2 / 3), (z + (w/4)), x, h, z));
        glVertex3f(x + (w/4), (h * 2 / 3), z + (w/4));
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z + (w/4)), (x + (w/4)), (h * 2 / 3), (z + (w/4)), x, h, z));
        glVertex3f(x, h, z);
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z + (w/4)), (x + (w/4)), (h * 2 / 3), (z + (w/4)), x, h, z));

        // Left

        glVertex3f(x - (w/4), (h * 2 / 3), z + (w/4));
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z + (w/4)), (x - (w/4)), (h * 2 / 3), (z - (w/4)), x, h, z));
        glVertex3f(x - (w/4), (h * 2 / 3), z - (w/4));
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z + (w/4)), (x - (w/4)), (h * 2 / 3), (z - (w/4)), x, h, z));
        glVertex3f(x, h, z);
        glNormal3fv(calculate_normal((x - (w/4)), (h * 2 / 3), (z + (w/4)), (x - (w/4)), (h * 2 / 3), (z - (w/4)), x, h, z));
        glEnd();
    }
};

class drawStore {
public:
    float x, z, h, w, d, r, g, b;
public:
    drawStore(float ox, float oz, float height, float width, float red, float green, float blue) {
        x = ox;
        z = oz;
        h = height;
        w = width;
        d = 7;
        r = red;
        g = green;
        b = blue;
    }
    void draw() {
        glColor3f(1.0, 1.0, 0);
        glBegin(GL_QUADS);

        //front store face
        glNormal3fv(calculate_normal(0, 0, -245, 75, 0, -245, 0, 20, -245));
        glVertex3f(0, 0, -245);
        glNormal3fv(calculate_normal(0, 0, -245, 75, 0, -245, 0, 20, -245));
        glVertex3f(75, 0, -245);
        glNormal3fv(calculate_normal(0, 0, -245, 75, 0, -245, 0, 20, -245));
        glVertex3f(75, 20, -245);
        glNormal3fv(calculate_normal(0, 0, -245, 75, 0, -245, 0, 20, -245));
        glVertex3f(0, 20, -245);

        glNormal3fv(calculate_normal(0, 0, -245, 0, 20, -245, 0, 20, -270));
        glVertex3f(0, 0, -245);
        glNormal3fv(calculate_normal(0, 0, -245, 0, 20, -245, 0, 20, -270));
        glVertex3f(0, 20, -245);
        glNormal3fv(calculate_normal(0, 0, -245, 0, 20, -245, 0, 20, -270));
        glVertex3f(0, 20, -270);
        glNormal3fv(calculate_normal(0, 0, -245, 0, 20, -245, 0, 20, -270));
        glVertex3f(0, 0, -270);

        glNormal3fv(calculate_normal(0, 0, -270, 75, 0, -270, 0, 20, -270));
        glVertex3f(0, 0, -270);
        glNormal3fv(calculate_normal(0, 0, -270, 75, 0, -270, 0, 20, -270));
        glVertex3f(75, 0, -270);
        glNormal3fv(calculate_normal(0, 0, -270, 75, 0, -270, 0, 20, -270));
        glVertex3f(75, 20, -270);
        glNormal3fv(calculate_normal(0, 0, -270, 75, 0, -270, 0, 20, -270));
        glVertex3f(0, 20, -270);

        glNormal3fv(calculate_normal(75, 0, -245, 75, 20, -245, 75, 0, -270));
        glVertex3f(75, 0, -245);
        glNormal3fv(calculate_normal(75, 0, -245, 75, 20, -245, 75, 0, -270));
        glVertex3f(75, 20, -245);
        glNormal3fv(calculate_normal(75, 0, -245, 75, 20, -245, 75, 0, -270));
        glVertex3f(75, 20, -270);
        glNormal3fv(calculate_normal(75, 0, -245, 75, 20, -245, 75, 0, -270));
        glVertex3f(75, 0, -270);

        glNormal3fv(calculate_normal(0, 20, -245, 75, 20, -245, 0, 20, -270));
        glVertex3f(0, 20, -245);
        glNormal3fv(calculate_normal(0, 20, -245, 75, 20, -245, 0, 20, -270));
        glVertex3f(75, 20, -245);
        glNormal3fv(calculate_normal(0, 20, -245, 75, 20, -245, 0, 20, -270));
        glVertex3f(75, 20, -270);
        glNormal3fv(calculate_normal(0, 20, -245, 75, 20, -245, 0, 20, -270));
        glVertex3f(0, 20, -270);
        glEnd();

        glColor3f(1.0, 0.33, 0.0);
        glBegin(GL_QUADS);
        glNormal3fv(calculate_normal(-3, 20, -242, 78, 20, -242, -3, 20, -273));
        glVertex3f(-3, 20, -242);
        glNormal3fv(calculate_normal(-3, 20, -242, 78, 20, -242, -3, 20, -273));
        glVertex3f(78, 20, -242);
        glNormal3fv(calculate_normal(-3, 20, -242, 78, 20, -242, -3, 20, -273));
        glVertex3f(78, 20, -273);
        glNormal3fv(calculate_normal(-3, 20, -242, 78, 20, -242, -3, 20, -273));
        glVertex3f(-3, 20, -273);

        glNormal3fv(calculate_normal(-3, 20, -242, 78, 20, -242, 0, 30, -270));
        glVertex3f(-3, 20, -242);
        glNormal3fv(calculate_normal(-3, 20, -242, 78, 20, -242, 0, 30, -270));
        glVertex3f(78, 20, -242);
        glNormal3fv(calculate_normal(-3, 20, -242, 78, 20, -242, 0, 30, -270));
        glVertex3f(75, 30, -245);
        glNormal3fv(calculate_normal(-3, 20, -242, 78, 20, -242, 0, 30, -270));
        glVertex3f(0, 30, -245);

        glNormal3fv(calculate_normal(78, 20, -242, 78, 20, -273, 75, 30, -245));
        glVertex3f(78, 20, -242);
        glNormal3fv(calculate_normal(78, 20, -242, 78, 20, -273, 75, 30, -245));
        glVertex3f(78, 20, -273);
        glNormal3fv(calculate_normal(78, 20, -242, 78, 20, -273, 75, 30, -245));
        glVertex3f(75, 30, -270);
        glNormal3fv(calculate_normal(78, 20, -242, 78, 20, -273, 75, 30, -245));
        glVertex3f(75, 30, -245);

        glNormal3fv(calculate_normal(78, 20, -273, -3, 20, -273, 75, 30, -270));
        glVertex3f(78, 20, -273);
        glNormal3fv(calculate_normal(78, 20, -273, -3, 20, -273, 75, 30, -270));
        glVertex3f(-3, 20, -273);
        glNormal3fv(calculate_normal(78, 20, -273, -3, 20, -273, 75, 30, -270));
        glVertex3f(0, 30, -270);
        glNormal3fv(calculate_normal(78, 20, -273, -3, 20, -273, 75, 30, -270));
        glVertex3f(75, 30, -270);

        glNormal3fv(calculate_normal(-3, 20, -242, -3, 20, -273, 0, 30, -245));
        glVertex3f(-3, 20, -242);
        glNormal3fv(calculate_normal(-3, 20, -242, -3, 20, -273, 0, 30, -245));
        glVertex3f(-3, 20, -273);
        glNormal3fv(calculate_normal(-3, 20, -242, -3, 20, -273, 0, 30, -245));
        glVertex3f(0, 30, -270);
        glNormal3fv(calculate_normal(-3, 20, -242, -3, 20, -273, 0, 30, -245));
        glVertex3f(0, 30, -245);

        glNormal3f(0, 1, 0);
        glVertex3f(0, 30, -245);
        glNormal3f(0, 1, 0);
        glVertex3f(75, 30, -245);
        glNormal3f(0, 1, 0);
        glVertex3f(75, 30, -270);
        glNormal3f(0, 1, 0);
        glVertex3f(0, 30, -270);
        glEnd();

        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
        glVertex3f(25, 21, -242);
        glVertex3f(50, 21, -242);
        glVertex3f(50, 29, -242);
        glVertex3f(25, 29, -242);

        glColor3f(.5, 1.0, 0.0);
        glVertex3f(25, 27, -241.9);
        glVertex3f(50, 27, -241.9);
        glVertex3f(50, 29, -241.9);
        glVertex3f(25, 29, -241.9);

        glVertex3f(34.5, 24, -241.9);
        glVertex3f(39.5, 24, -241.9);
        glVertex3f(39.5, 29, -241.9);
        glVertex3f(34.5, 29, -241.9);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0, 0, 0.5);
        glNormal3fv(calculate_normal(0, 0, -245, 75, 0, -245, 0, 20, -245));
        glVertex3f(25, 0, -244.9);
        glNormal3fv(calculate_normal(0, 0, -245, 75, 0, -245, 0, 20, -245));
        glVertex3f(50, 0, -244.9);
        glNormal3fv(calculate_normal(0, 0, -245, 75, 0, -245, 0, 20, -245));
        glVertex3f(50, 20, -244.9);
        glNormal3fv(calculate_normal(0, 0, -245, 75, 0, -245, 0, 20, -245));
        glVertex3f(25, 20, -244.9);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(.75, .75, .75);
        glVertex3f(25, 0, -244.8);
        glVertex3f(26, 0, -244.8);
        glVertex3f(26, 20, -244.8);
        glVertex3f(25, 20, -244.8);

        glVertex3f(49, 0, -244.8);
        glVertex3f(50, 0, -244.8);
        glVertex3f(50, 20, -244.8);
        glVertex3f(49, 20, -244.8);

        glVertex3f(25, 4, -244.8);
        glVertex3f(50, 4, -244.8);
        glVertex3f(50, 5, -244.8);
        glVertex3f(25, 5, -244.8);

        glVertex3f(30, 14, -244.8);
        glVertex3f(45, 14, -244.8);
        glVertex3f(45, 15, -244.8);
        glVertex3f(30, 15, -244.8);

        glVertex3f(30, 0, -244.8);
        glVertex3f(31, 0, -244.8);
        glVertex3f(31, 20, -244.8);
        glVertex3f(30, 20, -244.8);

        glVertex3f(44, 0, -244.8);
        glVertex3f(45, 0, -244.8);
        glVertex3f(45, 20, -244.8);
        glVertex3f(44, 20, -244.8);

        glVertex3f(37, 0, -244.8);
        glVertex3f(38, 0, -244.8);
        glVertex3f(38, 15, -244.8);
        glVertex3f(37, 15, -244.8);
        glEnd();
    }
};

//Lightning variables
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };



void mouseButton(int button, int state, int x, int y) {

    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {

        // when the button is released
        if (state == GLUT_UP) {
            anglex += deltaAnglex;
            angley += deltaAngley;
            xOrigin = -1;
            yOrigin = -1;
        }
        else {// state = GLUT_DOWN
            xOrigin = x;
            yOrigin = y;
        }
    }
    if (button == 4 && fieldOfView < 1000) {
        fieldOfView -= 10;
        setup();
        drawScene();
    }
    else if (button == 3 && fieldOfView > 0) {
        fieldOfView -= 10;
        setup();
        drawScene();
    }
}

void mouseMove(int x, int y) {

    // this will only be true when the left button is down
    if (xOrigin >= 0) {
        // update deltaAngle
        deltaAnglex = (x - xOrigin) * 0.001f;
        deltaAngley = (y - yOrigin) * 0.001f;
        // update camera's direction
        cenx = eyex + fieldOfView * cos(anglex + deltaAnglex);
        cenz = eyez + fieldOfView * sin(anglex + deltaAnglex);
        ceny = eyey + fieldOfView * sin(angley + deltaAngley);

        setup();
        drawScene();
    }

}

drawTree animatedTree = drawTree(110, -250, treeGrowthHeight, treeGrowthWidth, 0.5, 0.5, 0);

// Drawing routine.
void drawScene(void)
{
    glClearColor(0, 0.8, 1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glColor3f(0, 0, 0);
    //Road
    glBegin(GL_QUADS);
    glNormal3fv(calculate_normal(-60, -.02, 325, -60, -0.02, -275, 250, -0.02, 325));
    glVertex3f(-60, -.02, 325);
    glNormal3fv(calculate_normal(-60, -.02, 325, -60, -0.02, -275, 250, -0.02, 325));
    glVertex3f(-60, -0.02, -275);
    glNormal3fv(calculate_normal(-60, -.02, 325, -60, -0.02, -275, 250, -0.02, 325));
    glVertex3f(250, -0.02, -275);
    glNormal3fv(calculate_normal(-60, -.02, 325, -60, -0.02, -275, 250, -0.02, 325));
    glVertex3f(250, -0.02, 325);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-60, 0, 325);
    glVertex3f(-50, 0, 325);
    glVertex3f(-50, 0, -275);
    glVertex3f(-60, 0, -275);

    glVertex3f(-60, 0, 325);
    glVertex3f(-50, 0, 325);
    glVertex3f(-50, 0, -275);
    glVertex3f(-60, 0, -275);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(-7, 0, -211);
    glVertex3f(250, 0, -211);
    glVertex3f(250, 0, -275);
    glVertex3f(-7, 0, -275);
    glEnd();



    drawStore store = drawStore(0, 0, 0, 0, 0, 0, 0);
    store.draw();

    //draw parking lot lines
    int x = -40;
    int z = 250;
    int x2 = -45;
    // int z2 = 250;
    glLineWidth(10);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    for (int i=0; i < 15; i++) {
        glVertex3i(x, -.02, 250);
        glVertex3i(x, -.02, -150);
        x += 15;
    }
    glEnd();

    //draw perpendicular lines
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    for (int y=0; y < 15; y++) {
        for (int i=0; i < 21; i++) {
            glVertex3i(x2, -.02, z);
            glVertex3i(x2 + 10, -.02, z);
            z -= 20;
        }
        z = 250;
        x2 += 15;
    }
    glEnd();

    animatedTree.draw();

    for (drawTree tree : trees) {
        tree.draw();
    }

    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslated(101, 5, -240);
    glRotated(45, 0, 0, -1);
    glutSolidTeapot(.5);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslated(110, 0, -240);
    glRotated(30, 0, 0, -1);
    glRotated(90, 0, -1, 0);
    glutWireCone(.5, 9.75, 5, 5);
    glPopMatrix();

    //stop sign pole
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslated(-7, 2, -211);
    glScaled(.1, 4, .2);
    glutSolidCube(3);
    glPopMatrix();

    //stop sign
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslated(-7, 8, -211);
    glutSolidSphere(1, 10, 10);
    glPopMatrix();

    //light pole
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslated(81, 2, -171);
    glScaled(.1, 15, .2);
    glutSolidCube(3);
    glPopMatrix();

    //light on pole
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslated(81, 24, -171);
    glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 0, 1);
    glTranslated(98, 10, -222);
    glScaled(.5, 2, 1);
    glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslated(98, 6, -222);
    glScaled(.5, 2, 1);
    glutSolidCube(2);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 1, 0);
    glTranslated(98, 2, -222);
    glScaled(.5, 2, 1);
    glutSolidCube(2);
    glPopMatrix();

    glFlush();
}

void init() {
    trees.push_back(drawTree(-55, -200, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, -160, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, -120, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, -80, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, -40, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, 0, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, 40, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, 80, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, 120, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, 160, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, 200, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, 240, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(-55, 280, 20, 15, 0.5, 0.5, 0));

    trees.push_back(drawTree(145, -240, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(180, -240, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(215, -240, 20, 15, 0.5, 0.5, 0));
    trees.push_back(drawTree(240, -240, 20, 15, 0.5, 0.5, 0));
}

// Initialization routine.
void setup(void)
{
    glClearColor(0, 0.8, 1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-50, 50, -50, 50, 1, 10000000);
    gluPerspective(20, 16 / 9, 1, 10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, cenx, ceny, cenz, 0, 100000, 0); // camera

}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
}

float roundx(float var)
{
    float value = round(var * 100 );
    return (float)value / 100;
}

void tilt() {
    float angleP = roundx(fmod(anglex * 180 / PI,360));
    if (315 <= angleP || 0 <= angleP && angleP <= 45) {

        upVz = roundx(sin(upAngle));
        upVy = roundx(cos(upAngle));
    }
    if ( 135<= angleP && angleP <=225) {

        upVz = roundx(-(sin(upAngle)));
        upVy = roundx(cos(upAngle));
    }
    if (135 >= angleP && angleP >= 45) {

        upVx = roundx(-(sin(upAngle)));
        upVy = roundx(cos(upAngle));
    }
    if (225 <= angleP && angleP <= 315) {
        upVx = roundx((sin(upAngle)));
        upVy = roundx(cos(upAngle));
    }
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    //float angle = abs(anglex);
    keyStates[key] = true;
    switch (key) {
        case ('a'): {
            eyex += amt * sin(anglex);
            eyez -= amt * cos(anglex);
            cenx += amt * sin(anglex);
            cenz -= amt * cos(anglex);
            setup();
            drawScene();
            break;
        }
        case ('s'): {
            eyex -= amt * cos(anglex);
            eyez -= amt * sin(anglex);
            cenx -= amt * cos(anglex);
            cenz -= amt * sin(anglex);
            setup();
            drawScene();
            break;
        }
        case ('d'): {
            eyex -= amt * sin(anglex);
            eyez += amt * cos(anglex);
            cenx -= amt * sin(anglex);
            cenz += amt * cos(anglex);
            setup();
            drawScene();
            break;
        }
        case ('w'): {
            eyex += amt * cos(anglex);
            eyez += amt * sin(anglex);
            cenx += amt * cos(anglex);
            cenz += amt * sin(anglex);
            setup();
            drawScene();
            break;
        }
        case ('e'): {
            if (upAngle < 0.20944) {
                upAngle = roundx(upAngle+.01f);
                tilt();
            }
            setup();
            drawScene();
            break;
        }
        case ('q'):
        {

            if (upAngle > -0.20944) {
                upAngle = roundx(upAngle - .01f);
                tilt();
            }
            setup();
            drawScene();
            break;
        }
        case ('v'):
        {
            eyey -= amt;
            ceny -= amt;
            setup();
            drawScene();
        }
        case (' '):
        {
            eyey += amt;
            ceny += amt;
            setup();
            drawScene();
        }

        case ('c'): {
            cout << eyex << endl;
            cout << eyez << endl;
        }

    }
}

void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void idleFunc() {


    if (keyStates['q'] == false && upAngle < 0) {
        upAngle = roundx(upAngle + .01f);
        tilt();
        setup();
        drawScene();

    }
    else if (keyStates['e'] == false && upAngle > 0) {
        upAngle = roundx(upAngle - .01f);
        tilt();
        setup();
        drawScene();

    }

}


void draw(int iUnused)
{
    if (treeGrowthHeight != 25) {
        treeGrowthWidth += treeGrowthRate;
        treeGrowthHeight += treeGrowthRate;
    } else {
        treeGrowthWidth = 0;
        treeGrowthHeight = 0;
    }
    animatedTree = drawTree(110, -240, treeGrowthHeight, treeGrowthWidth, 0.5, 0.5, 0);
    glutPostRedisplay();
    glutTimerFunc(60, draw, 0);
}

// Main routine.
int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DOH!");

    setup();
    init();
    glShadeModel(GL_FLAT);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glutDisplayFunc(drawScene);
    draw(0);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutKeyboardUpFunc(keyUp);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutMainLoop();
}

