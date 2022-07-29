#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

// �ubuk de�erlerini yazal�m. 
struct Cubuk
{
    float x; //�ubu�un x (yatay) konumu
    float y; // �ubu�un y konumu
    float w; //�ubuk genisli�i (x^le orant�l�)
    float h; //�ubuk y�ksekli�i (y ile orant�l�)
    float adim; // ne kadarl�k hareket edece�i
};

// Top de�erlerini yazal�m. 
struct Top
{
    float r; // top yar��ap�
    float x; //topun x konumu
    float y; // topun y konumu
    float hiz; // topun h�z�
    float xadim; // topun x d�zlemindeki ad�m say�s�
    float yadim; //topun y d�zlemindeki ad�m say�s�
};

//alt �ubuk tan�mla
Cubuk bar = { 0, -11, 3, 1, 0.5 };
//�st �ubuk tan�mla
Cubuk top_bar = { 0, 11, 16, 1, 0 };
//topu tan�mla
Top ball = { 0.5, 0, 0, 5.0, 0.10, 0.10 };

const float bar_maxr = 13; // �ubu�un gidece�i max sa� u� s�n�r�
const float bar_maxl = -13; // �ubu�un gidece�i max sol u� s�n�r�

const float ball_maxu = 9.5; // topun gidece�i max �st s�n�r
const float ball_maxd = -9.5; // topun gidece�i max alt s�n�r
const float ball_maxr = 15; // topun gidece�i max sa� s�n�r
const float ball_maxl = -15; // topun gidece�i max sol s�n�r

bool isReachedXMax = false; //en u� x nokt. ula�t� m� kontrol, ula�t�ysa (true) top sekecek, �ubuk sabit/hareket yok
bool isReachedYMax = false; // en u� y nokt. ula�t� m� kontrol, ula�t�ysa (true) top sekecek, �ubuk sabit/hareket yok 
bool isGameEnd = false; // kaybetti mi?


// sahneyi merkezden 30 birim geriye do�ru ba�lat
const int zoom = -30;


char finaltext[20]; //kaybedince ekranda ��kacak metnin max karakter say�s�

void finalscreen()
{

    glColor3f(1, 0, 0); // kaybedince ��kan metnin rengi 

    glRasterPos3f(-4, 0, zoom); //��kacak metnin konumu
    for (int i = 0; finaltext[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, finaltext[i]); //metni yazd�rma
    glutSwapBuffers(); //ekran de�i�tirme

}


void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //oyun bitince ekran� render'lamay� b�rak, final sahnesini g�ster. 
    if (isGameEnd)
    {
        finalscreen();
        return;
    }

    // Topu tan�mla(konumu, rengi, �ekli, vs)
    glPushMatrix(); //topun hareketli oldu�unu g�sterme
    glTranslatef(ball.x, ball.y, zoom);
    glColor3f(0, 0.4, 0.6);
    glutSolidSphere(ball.r, 20, 20);
    glPopMatrix();

    // Alt �ubu�u tan�mla(konumu, rengi, �ekli)
    glPushMatrix();
    glTranslatef(bar.x, bar.y, zoom);
    glBegin(GL_QUADS); //d�rtgen
        glColor3f(1, 0, 1);
        glVertex2f(bar.w, bar.h);
        glColor3f(1, 0, 1);
        glVertex2f(bar.w, -bar.h);
        glColor3f(1, 0, 1);
        glVertex2f(-bar.w, -bar.h);
        glColor3f(1, 0, 1);
        glVertex2f(-bar.w, bar.h);
    glEnd();
    glPopMatrix();

    // �st �ubu�u tan�mla(konumu, rengi, �ekli)
    glPushMatrix();
    glTranslatef(top_bar.x, top_bar.y, zoom);
    glBegin(GL_QUADS); //d�rtgen
        glColor3f(1, 0, 0.6);
        glVertex2f(top_bar.w, top_bar.h);
        glColor3f(1, 0, 0.6);
        glVertex2f(top_bar.w, -top_bar.h);
        glColor3f(1, 0, 0.6);
        glVertex2f(-top_bar.w, -top_bar.h);
        glColor3f(1, 0, 0.6);
        glVertex2f(-top_bar.w, top_bar.h);
    glEnd();
    glPopMatrix();

    glutSwapBuffers(); //her render'da ekran de�i�ecek. 
}

void init(void)
{
    //Arka plan rengi
    glClearColor(1, 1, 1, 1);
    glClearDepth(1.0);
  
    glEnable(GL_DEPTH_TEST);
    //ekran ge�i�lerini daha yumu�atmak i�in: 
    glShadeModel(GL_SMOOTH); 
    //objeleri renklendirmek i�in:
    glEnable(GL_COLOR_MATERIAL);
}

void reshape(int w, int h)
{
    float aspectRatio;
    h = (h == 0) ? 1 : h;
    w = (w == 0) ? 1 : w;

    // Pencere boyutunu g�r�n�m alan� olarak ayarlamak i�in:
    glViewport(0, 0, w, h);
    aspectRatio = (float)w / (float)h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Perspektifi ayarlama
    gluPerspective(45, aspectRatio, 1.0, 100.0);

    //model g�r�n�m� i�in:
    glMatrixMode(GL_MODELVIEW);
}

void idle()
{
    Sleep(100); 

    
    if (ball.y <= ball_maxd) // Topun y de�eri max a�a�� de�erine e�it ise �ubu�a dokunup dokunmad���n� kontrol et.
    {
        
        if (ball.x <= bar.x - bar.w or ball.x >= bar.x + bar.w) // Top �ubu�a de�miyorsa kaybet
        {
            glClearColor(0, 0, 0, 1); // Arka plan rengini siyah yap
            isGameEnd = true;
            strcpy_s(finaltext, "Kaybettin :("); // ekrana yaz�lacak metni tan�mla. 
            
        }
    }

    

    // Top max sa� s�n�ra de�diyse:
    if (ball.x > ball_maxr)
        isReachedXMax = true;
    // Top max sol s�n�ra de�diyse:
    else if (ball.x < ball_maxl)
        isReachedXMax = false;

    // Top max yukar� s�n�ra de�diyse:
    if (ball.y > ball_maxu)
        isReachedYMax = true;
    //Top max a�a�� s�n�ra de�diyse:
    else if (ball.y < ball_maxd)
        isReachedYMax = false;

    // Topu ad�m ve h�za g�re hareket ettir: sola, sa�a veya yukar�ya �arpt�ysa sektir. 
    if (isReachedXMax)
        ball.x -= ball.xadim * ball.hiz;
    else
        ball.x += ball.xadim * ball.hiz;
    if (isReachedYMax)
        ball.y -= ball.yadim * ball.hiz;
    else
        ball.y += ball.yadim * ball.hiz;

    glutPostRedisplay();
}


void specialkey(int key, int x, int y)
{
    // sa� ve sol ok tu�lar�yla �ubu�u hareket ettir. 
    if (key == GLUT_KEY_LEFT and bar.x > bar_maxl)
    {
        bar.x -= bar.adim;
    }
    else if (key == GLUT_KEY_RIGHT and bar.x < bar_maxr)
    {
        bar.x += bar.adim;
    }
}

int main(int argc, char** argv)
{
    //ekran� tan�mlama
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Top Sektirme Oyunu");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(render);
    glutSpecialFunc(specialkey);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

