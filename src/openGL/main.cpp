#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

class vertice_obj {
    public:
        float x;
        float y;
        float z;

        vertice_obj(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
};

class face_obj {
    public:
        int v1;
        int v2;
        int v3;

        face_obj(int v1, int v2, int v3)
        {
            this->v1 = v1;
            this->v2 = v2;
            this->v3 = v3;
        }
};

class OBJ{
    public: 
        vector<face_obj> faces;
        vector<vertice_obj> vertices;

        float escala = 450;
        float z0 = 30;
        float offset = 600;

        void ler_obj(string caminho)
        {
            ifstream arquivo(caminho);

            string palavra;
            char tipo;

            while(arquivo >> tipo){
                if(tipo == 'v'){
                    float x;
                    float y;
                    float z;

                    arquivo >> x;
                    arquivo >> y;
                    arquivo >> z;
                    vertices.push_back(vertice_obj(x, y, z));
                }
                if(tipo == 'f'){
                    float v1;
                    float v2;
                    float v3;

                    arquivo >> v1;
                    arquivo >> v2;
                    arquivo >> v3;
                    faces.push_back(face_obj(v1, v2, v3));
                }
            }
        }
     
};

int window_width  = 800;  // "static int" evitaria que outros códigos do mesmo projeto vejam estas variáveis
int window_height = 600;

GLfloat light0_position[] = { 50.0f, 50.0f, 50.0f, 1.0f }; // x,y,z,w = 1 , luz pontual
GLfloat light0_ambient[]  = { 0.15f, 0.15f, 0.15f, 1.0f };
GLfloat light0_diffuse[]  = { 0.80f, 0.80f, 0.80f, 1.0f };
GLfloat light0_specular[] = { 1.00f, 1.00f, 1.00f, 1.0f };

GLfloat global_ambient[]  = { 0.05f, 0.05f, 0.05f, 1.0f };


GLfloat mat_ambient[]   = { 0.1f, 0.1f, 0.2f, 1.0f }; 
GLfloat mat_diffuse[]   = { 0.2f, 0.3f, 0.8f, 1.0f }; 
GLfloat mat_specular[]  = { 0.9f, 0.9f, 0.9f, 1.0f }; 
GLfloat mat_shininess[] = { 64.0f };                   

GLfloat cam_eye[]    = { 40.0f, 1.5f, 4.0f };
GLfloat cam_center[] = { 0.0f, 0.5f, 0.0f };
GLfloat cam_up[]     = { 0.0f, 1.0f, 0.0f };

float angle = 0.0f; // "static float", a mesma coisa.

void desenhaObjeto(OBJ obj)
{

    for(auto& face : obj.faces)
    {
        float x1 = obj.vertices[face.v1 - 1].x;
        float y1 = obj.vertices[face.v1 - 1].y;
        float z1 = obj.vertices[face.v1 - 1].z;

        float x2 =  obj.vertices[face.v2 - 1].x;
        float y2 =  obj.vertices[face.v2 - 1].y;
        float z2 =  obj.vertices[face.v2 - 1].z;

        float x3 = obj.vertices[face.v3 - 1].x;
        float y3 = obj.vertices[face.v3 - 1].y;
        float z3 = obj.vertices[face.v3 - 1].z;

        float ux = x2 - x1;
        float uy = y2 - y1;
        float uz = z2 - z1;

        float vx = x3 - x2;
        float vy = y3 - y2;
        float vz = z3 - z2;

        float x = uy * vz - (uz * vy);
        float y = uz * vx - (ux * vz);
        float z = ux * vy - (uy * vx);

        float modulo = sqrt(pow(x, 2) + pow(y,2) + pow(z,2));

        x = x/modulo;
        y = y/modulo;
        z = z/modulo;

        glNormal3f(x, y, z);
        glBegin(GL_TRIANGLES);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
        glEnd();

    }

}

void meuinit(void)
{
    glEnable(GL_DEPTH_TEST);    
    glShadeModel(GL_SMOOTH);    
    glEnable(GL_NORMALIZE);     

    // Iluminacao global 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // modelo de observador local para LUZ especular

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

         glEnable(GL_COLOR_MATERIAL);
       glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
}


void reshape(int w, int h)
{
    window_width = w;
    window_height = (h == 0) ? 1 : h;
    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble)window_width / (GLdouble)window_height, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glulookat configra a matriz de visualizacao
    glMatrixMode(GL_MODELVIEW);
    
	glLoadIdentity();
    gluLookAt(cam_eye[0], cam_eye[1], cam_eye[2],
              cam_center[0], cam_center[1], cam_center[2],
              cam_up[0], cam_up[1], cam_up[2]);

        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    
    glPushMatrix();
      glDisable(GL_LIGHTING); // chao simples
//      glColor3f(0.2f, 0.2f, 0.2f);
      glBegin(GL_LINES);
        for (int i = -10; i <= 10; i++) {
            glColor3f((GLfloat)(i+10)/20, (GLfloat)(i+20)/30,(GLfloat)(i+40)/50);
            glVertex3f((GLfloat)i, 0.0f, -10.0f);
            glVertex3f((GLfloat)i, 0.0f,  10.0f);
            glVertex3f(-10.0f, 0.0f, (GLfloat)i);
            glVertex3f( 10.0f, 0.0f, (GLfloat)i);
        }
      glEnd();
      glEnable(GL_LIGHTING);
    glPopMatrix();
    
 //   glPushMatrix();
 //   glTranslatef(2.0f, 0.0f, 0.0f); 
 //   glRotatef(angle, 0.0f, 1.0f, 0.0f);

    OBJ obj = OBJ();
    obj.ler_obj("imagens/elephant.obj");

    glPushMatrix();
      glTranslatef(0.0f, 0.5f, 0.0f); 
      glRotatef(angle, 0.0f, 1.0f, 0.0f);
	  glColor3f(1.0f, 0.0f, 0.0f);
	  //glutSolidSphere(0.7, 36, 36);
      desenhaObjeto(obj);
	  //glutSolidTeapot(0.6);
    glPopMatrix();
//  glPopMatrix();

    glutSwapBuffers();
}


void idleFunc(void)
{
    angle += 0.2f;
    if (angle > 360.0f) angle -= 360.0f;
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
    (void)x; (void)y;
    if (key == 27 || key == 'q' || key == 'Q') { // ESC ou Q sai 
        exit(0);
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("bulezinho exemplo");

    meuinit();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idleFunc);

    glutMainLoop();
    return 0;
}
