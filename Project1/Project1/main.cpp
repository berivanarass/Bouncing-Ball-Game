#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

// Çubuk deðerlerini yazalým. 
struct Cubuk
{
    float x; //çubuðun x (yatay) konumu
    float y; // çubuðun y konumu
    float w; //çubuk genisliði (x^le orantýlý)
    float h; //çubuk yüksekliði (y ile orantýlý)
    float adim; // ne kadarlýk hareket edeceði
};

// Top deðerlerini yazalým. 
struct Top
{
    float r; // top yarýçapý
    float x; //topun x konumu
    float y; // topun y konumu
    float hiz; // topun hýzý
    float xadim; // topun x düzlemindeki adým sayýsý
    float yadim; //topun y düzlemindeki adým sayýsý
};

//alt çubuk tanýmla
Cubuk bar = { 0, -11, 3, 1, 0.5 };
//üst çubuk tanýmla
Cubuk top_bar = { 0, 11, 16, 1, 0 };
//topu tanýmla
Top ball = { 0.5, 0, 0, 5.0, 0.10, 0.10 };

const float bar_maxr = 13; // çubuðun gideceði max sað uç sýnýrý
const float bar_maxl = -13; // çubuðun gideceði max sol uç sýnýrý

const float ball_maxu = 9.5; // topun gideceði max üst sýnýr
const float ball_maxd = -9.5; // topun gideceði max alt sýnýr
const float ball_maxr = 15; // topun gideceði max sað sýnýr
const float ball_maxl = -15; // topun gideceði max sol sýnýr

bool isReachedXMax = false; //en uç x nokt. ulaþtý mý kontrol, ulaþtýysa (true) top sekecek, çubuk sabit/hareket yok
bool isReachedYMax = false; // en uç y nokt. ulaþtý mý kontrol, ulaþtýysa (true) top sekecek, çubuk sabit/hareket yok 
bool isGameEnd = false; // kaybetti mi?


// sahneyi merkezden 30 birim geriye doðru baþlat
const int zoom = -30;


char finaltext[20]; //kaybedince ekranda çýkacak metnin max karakter sayýsý

void finalscreen()
{

    glColor3f(1, 0, 0); // kaybedince çýkan metnin rengi 

    glRasterPos3f(-4, 0, zoom); //Çýkacak metnin konumu
    for (int i = 0; finaltext[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, finaltext[i]); //metni yazdýrma
    glutSwapBuffers(); //ekran deðiþtirme

}


void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //oyun bitince ekraný render'lamayý býrak, final sahnesini göster. 
    if (isGameEnd)
    {
        finalscreen();
        return;
    }

    // Topu tanýmla(konumu, rengi, þekli, vs)
    glPushMatrix(); //topun hareketli olduðunu gösterme
    glTranslatef(ball.x, ball.y, zoom);
    glColor3f(0, 0.4, 0.6);
    glutSolidSphere(ball.r, 20, 20);
    glPopMatrix();

    // Alt çubuðu tanýmla(konumu, rengi, þekli)
    glPushMatrix();
    glTranslatef(bar.x, bar.y, zoom);
    glBegin(GL_QUADS); //dörtgen
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

    // Üst çubuðu tanýmla(konumu, rengi, þekli)
    glPushMatrix();
    glTranslatef(top_bar.x, top_bar.y, zoom);
    glBegin(GL_QUADS); //dörtgen
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

    glutSwapBuffers(); //her render'da ekran deðiþecek. 
}

void init(void)
{
    //Arka plan rengi
    glClearColor(1, 1, 1, 1);
    glClearDepth(1.0);
  
    glEnable(GL_DEPTH_TEST);
    //ekran geçiþlerini daha yumuþatmak için: 
    glShadeModel(GL_SMOOTH); 
    //objeleri renklendirmek için:
    glEnable(GL_COLOR_MATERIAL);
}

void reshape(int w, int h)
{
    float aspectRatio;
    h = (h == 0) ? 1 : h;
    w = (w == 0) ? 1 : w;

    // Pencere boyutunu görünüm alaný olarak ayarlamak için:
    glViewport(0, 0, w, h);
    aspectRatio = (float)w / (float)h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Perspektifi ayarlama
    gluPerspective(45, aspectRatio, 1.0, 100.0);

    //model görünümü için:
    glMatrixMode(GL_MODELVIEW);
}

void idle()
{
    Sleep(100); 

    
    if (ball.y <= ball_maxd) // Topun y deðeri max aþaðý deðerine eþit ise çubuða dokunup dokunmadýðýný kontrol et.
    {
        
        if (ball.x <= bar.x - bar.w or ball.x >= bar.x + bar.w) // Top çubuða deðmiyorsa kaybet
        {
            glClearColor(0, 0, 0, 1); // Arka plan rengini siyah yap
            isGameEnd = true;
            strcpy_s(finaltext, "Kaybettin :("); // ekrana yazýlacak metni tanýmla. 
            
        }
    }

    

    // Top max sað sýnýra deðdiyse:
    if (ball.x > ball_maxr)
        isReachedXMax = true;
    // Top max sol sýnýra deðdiyse:
    else if (ball.x < ball_maxl)
        isReachedXMax = false;

    // Top max yukarý sýnýra deðdiyse:
    if (ball.y > ball_maxu)
        isReachedYMax = true;
    //Top max aþaðý sýnýra deðdiyse:
    else if (ball.y < ball_maxd)
        isReachedYMax = false;

    // Topu adým ve hýza göre hareket ettir: sola, saða veya yukarýya çarptýysa sektir. 
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
    // sað ve sol ok tuþlarýyla çubuðu hareket ettir. 
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
    //ekraný tanýmlama
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

