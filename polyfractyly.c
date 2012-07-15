#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif



/* Option: Represents an option such as "gravity on" */
typedef struct {
    char* label;
    int value;
} Option;

enum option {
    OPT_GRAV,
    OPT_INTER_OBJ_GRAV,
    OPT_INTER_OBJ_BOUNCE,
    OPT_MOUSE_ATTR,
    OPT_AIR_RESIST,
    OPT_WALL_BOUNCE
};
const Option options[] = {
    /*        {"[g] gravity", false},
              {"[i] inter-obj gravity", false},
              {"[b] inter-obj bounce", false},
              {"[m] mouse attraction", true},
              {"[d] air resistance", false},
              {"[w] wall bounce", false}
     */
};
const char* otherCommands[] = {
    /*    "[r] add object",
          "[r] remove object",
          "[c] clear objects"
     */
};

/* ------------------------------------------------------------------- */


const char* HELP_STR =
"ARGUMENTS\n\n"
;

int width = 1000;
int height = 600;

int xMousePos = -1;
int yMousePos = -1;

char rotateMode = 'z';



void init();
void display();
void reshape(int w, int h); 
void renderScene();
void mouse(int x, int y);
bool processArgs(int argc, char* argv[]);
void drawString(char* str, int x, int y);
void keyPressed(unsigned char key, int x, int y);
void specialKeyPressed(int key, int x, int y);
float min(float a, float b);
float max(float a, float b);

int main(int argc, char* argv[]) {

    int validArgs = processArgs(argc, argv);
    if (validArgs == false) {
        printf("\nQuitting...\n");
        return 0;
    }

    // GLUT setup stuff
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("polyfractyly");

    // register display callback
    glutDisplayFunc(display);

    // when mouse moves with button down
    //glutMotionFunc(mouse);

    // when mouse moves without button down
    glutPassiveMotionFunc(mouse);

    // keyboard functions
    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(specialKeyPressed);

    // redisplay
    //glutIdleFunc(display);

    // reshape
    glutReshapeFunc(reshape);

    init();

    glutMainLoop();
}



/*
   Set up screen stuff
 */
void init() {

    // enable z axis
    glEnable(GL_DEPTH_TEST);

    glDepthMask(GL_TRUE);

    glShadeModel(GL_SMOOTH);

    glViewport(0,0,width,height);

    // choose background color
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // set point size
    glPointSize(1.0);

    // load matrix mode
    glMatrixMode (GL_PROJECTION);

    // load identity matrix
    glLoadIdentity();
    
    glOrtho(0, width, 0, height, -max(width,height), max(width,height));
    //glFrustum(0, width, 0, height, -1, 1);
    
    //         eye      at      up
    gluLookAt(0,0,0,  0,0,-1,  0,1,0);


}



/*
   Display the state of the game visually
 */
void display() {
    
    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //glLoadIdentity();

    //glRotatef(10, 0, 1, 1);

    //glTranslatef(0, 0, -1);

    // draw the current state of the game
    renderScene();

    // swap front and back buffers
    glutSwapBuffers();

}

float yStart = -1.0;
float yEnd = 1.0;
float xStart = -2.5;
float xEnd = 1;

int maxIteration = 50;

void renderScene() {

    int c, r, iteration;
    float x, y, x0, y0;
    float xTemp;
    float yRange = yEnd - yStart;
    float xRange = xEnd - xStart;
    float h,s,b;

    glBegin(GL_POINTS);

    // Mandelbrot-drawing algorithm
    for (r=0; r<height; r++) {
        y0 = (yRange * r / height) + yStart;

        for (c=0; c<width; c++) {
            x0 = (xRange * c / width) + xStart;

            x = 0; 
            y = 0;
            iteration = 0;

            while (iteration < maxIteration  &&  x*x + y*y <= (2*2)) {
                xTemp =  x*x  -  y*y  +  x0;
                y =  2*x*y  +  y0;

                x = xTemp;
                iteration++;
            }



            if (iteration != maxIteration) {
                // calculate color
                h = 1.*iteration/maxIteration;
                //h = 1-h;

                // set the color
                //glColor3f(h,s,b);
                glColor3f(0, h, 0);

                // draw the pixel
                glVertex3f(c,r,-100*h);
            }
        }
    }

    glEnd();

    // flushes all unfinished drawing commands
    glFlush();

}



float max(float a, float b) {
    return a>b ? a:b;
}
float min(float a, float b) {
    return a<b ? a:b;
}

/*
float* RGBToHSV(float r, float g, float b) {
    float* ans = malloc(sizeof(float)*3);

    float M = max(max(r,g),b);
    float m = min(min(r,g),b);
    float C = M-m;
}
*/

void reshape(int w, int h) {
    width = w;
    height = h;
    glViewport(0,0,w,h);
    glutPostRedisplay();
}

void mouse(int x, int y) {
    xMousePos = x;
    yMousePos = height - y;
}

void timer() {
    glutPostRedisplay();
}

bool processArgs(int argc, char* argv[]) {
    int argNum;
    char* argLabel, *argVal;
   
    // read in all arguments
    argNum=1;
    while (argNum < argc) {
        argLabel = argv[argNum];
        argVal = argv[argNum+1];

        // strcmp(a,b) returns 0 if the strings are equal, so my shorthand
        // for checking if two strings are equal is !strcmp(a,b)
        //if (!strcmp(argLabel, "--nVal") || !strcmp(argLabel, "-n")) {    
        //}

        argNum++;
    }

    return true;
}



void drawString(char* str, int x, int y) {
    int i;
    int len;
    
    // set color
    glColor3f(0.0, 1.0, 0.0);
    
    // set position
    glRasterPos2i(x, y);

    // draw each character
    len = strlen(str);
    for (i=0; i<len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
    }

}

void specialKeyPressed(int key, int x, int y) {

    float rx, ry, rz;
    rx = ry = rz = 0;
    switch (rotateMode) {
        case 'x':
            rx = 1;
            break;
        case 'y':
            ry = 1;
            break;
        case 'z':
            rz = 1;
            break;
    }

    float angle = 10;

    glPopMatrix();

    glTranslatef(width/2, height/2, 0);

    switch(key) {
        case GLUT_KEY_LEFT:
            glRotatef(angle, rx, ry, rz);
            break;
        case GLUT_KEY_RIGHT:
            glRotatef(-angle, rx, ry, rz);
            break;
        case GLUT_KEY_UP:
            glScalef(1.1, 1.1, 1.1);
            break;
        case GLUT_KEY_DOWN: 
            glScalef(.9, .9, .9);
            break;
    }

    glTranslatef(-width/2, -height/2, 0);

    glutPostRedisplay();
    glPushMatrix();

}


void keyPressed(unsigned char key, int x, int y) {

    switch(key) {
        case 'r':
            glutPostRedisplay();
            break;
        case 'x':
        case 'y':
        case 'z':
            rotateMode = key;
            break;
    }

}

