/****
Andre Ramos Neves - 41445368
Andre Augusto Miguel - 41425340
*****/

#ifdef __linux__
    #include <X11/Xlib.h>
#elif WIN32
    #include <windows.h>
#endif



#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <string.h>
#endif


#include <stdlib.h>
#include <stdio.h>
#include "include/engine_game.h"

#include "include/glm.h"


#define dimensao_quadrado 60
#define x_tabuleiro 10

#define TIMEOUT 60

GLfloat angle, fAspect;
GLuint character;
float charot;
float charot1;


int pontuacao = 40;
int linhaClick = -1;
int ColunaClick = -1;
int embaralhou = 0;
int TempoResolver;
//const int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
//const int fonte2=(int)GLUT_BITMAP_HELVETICA_18;
GLMmodel *pmodel = NULL;

int fTime;
int iResetTime = 0;


float* Normals;
float* Faces_Triangles;
float* Vertex_Buffer;


long TotalConnectedPoints;
long TotalConnectedTriangles;

void drawGoodMonkey();
void drawBadMonkey();

void loadObj2(char *fname);

void loadObj(char *fname);

GLuint LoadTexture( const char * filename );

void renderBitmapString(float x, float y, void *font_old,const char *string){
//
const int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
//
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

static float altura=(float)dimensao_quadrado*3.0f;
static float largura=(float)dimensao_quadrado*5.0f;


static void	Inicializa	(void)
{	//cor de fundo
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor"
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;

 	// Especifica que a cor de fundo da janela será preta
    glClearColor(1.0f,	1.0f,	0.0f,	0.0f);
    glClearAccum(0.0, 0.0, 0.0, 0.0);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
    //glutFullScreen();
	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);

//    pmodel = glmReadSDL("image.bmp",GL_TRUE,10,10,10);


//    SDL_Surface *image = SDL_LoadBMP("image.bmp");
    //SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
	// Habilita o depth-buffering
	//glEnable(GL_DEPTH_TEST);

    angle=45;

    loadObj2("macaco3.obj");
	glmUnitize(pmodel);
	glmVertexNormals(pmodel, 90.0, GL_TRUE);

    geraMatrizAleatoria();
}

static void	AlteraTamanhoJanela(GLsizei	w,	GLsizei	h)
{
    if(h	==	0)	h	=	1;
    glViewport(0,	0,	w,	h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if	(w	<=	h)
        gluOrtho2D	(0.0f,	250.0f,	0.0f,	250.0f*h/w);
    else
        gluOrtho2D	(0.0f,	250.0f*w/h,	0.0f,	250.0f);

}


static void Tabuleiro(int dimensao){

    glBegin(GL_QUADS);

    glColor3f(0.0f,	0.0f, 1.0f);
    glVertex2i(x_tabuleiro - 5 , x_tabuleiro - 5);

    glVertex2i(x_tabuleiro -5, altura + 10);

    glVertex2i(largura + 10, altura + 10);

    glVertex2i(largura + 10, x_tabuleiro - 5);

    glEnd();

}

static void CuboNumeral(int numero, float cor[]){
//
const int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
//

    glBegin(GL_QUADS);

    glColor3f(cor[0],cor[1],cor[2]);

    glVertex2i(x_tabuleiro, x_tabuleiro);

    glVertex2i(x_tabuleiro, dimensao_quadrado);

    glVertex2i(dimensao_quadrado , dimensao_quadrado);

    glVertex2i(dimensao_quadrado , x_tabuleiro);

    glEnd();

    glColor3f(1.0f,	1.0f,0.0f);

    char *buffer;
    buffer = (char *)malloc(15*sizeof(char));
    sprintf(buffer,"%d",numero);

    renderBitmapString((dimensao_quadrado/2),(dimensao_quadrado/2),(void *)font,buffer);

    free(buffer);
}


static void CuboOperador(char* op, float cor[]){
//
const int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
//

    glBegin(GL_QUADS);

    glColor3f(cor[0],cor[1],cor[2]);

    glVertex2i(x_tabuleiro, x_tabuleiro);

    glVertex2i(x_tabuleiro, dimensao_quadrado);

    glVertex2i(dimensao_quadrado , dimensao_quadrado);

    glVertex2i(dimensao_quadrado , x_tabuleiro);

    glEnd();


    glColor3f(1.0f,	1.0f,0.0f);

    renderBitmapString((dimensao_quadrado/2),(dimensao_quadrado/2),(void *)font,op);

}

static void Desenha(void)
{
//
const int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
const int fonte2=(int)GLUT_BITMAP_HELVETICA_18;
const int fonte3=(int)GLUT_BITMAP_TIMES_ROMAN_10;
//

    float cor_verde_escuro[3] = {0.0, 0.6, 0.0};
    float cor_verde_claro[3] = {0.0, 1.0, 0.0};
    float cor_azul_claro[3] = {0.0,0.9,1.0};
    float cor_azul_escuro[3] = {0.0,0.4,1.0};
    float cor_vermelho_claro[3] = {1.0,0.5,0.5};
    float cor_vermelho_escuro[3] = {1.0,0.1,0.2};
    float cor_preto[3] = {0.0,0.0,0.0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    glPushMatrix();
        Tabuleiro(dimensao_quadrado);
    glPopMatrix();

    glPushMatrix();
       drawBadMonkey();
    glPopMatrix();

    glPushMatrix();
       drawGoodMonkey();
    glPopMatrix();


    glPushMatrix();
        if (linhaClick == 3 && ColunaClick == 1){
            CuboNumeral(eng_matriz[2][0], cor_azul_claro);
        }else{
            CuboNumeral(eng_matriz[2][0], cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(60.0,0.0,0.0);
        if (linhaClick == 3 && ColunaClick == 2){
            CuboOperador(getSinal(2,1), cor_azul_claro);
        }else{
            CuboOperador(getSinal(2,1), cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(120.0,0.0,0.0);
        if (linhaClick == 3 && ColunaClick == 3){
            CuboNumeral(eng_matriz[2][2], cor_azul_claro);
        }else{
            CuboNumeral(eng_matriz[2][2], cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(180.0,0.0,0.0);
        if (linhaClick == 3 && ColunaClick == 4){
            CuboNumeral(eng_matriz[2][3], cor_vermelho_claro);
        }else{
            if(corRespostaCertaErrada(2) == CORRETA)
                CuboNumeral(eng_matriz[2][3], cor_verde_escuro);
            else
                CuboNumeral(eng_matriz[2][3], cor_vermelho_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(240.0,0.0,0.0);
        if (linhaClick == 3 && ColunaClick == 5){
            CuboNumeral(eng_matriz[2][4], cor_verde_claro);
        }else{
            CuboNumeral(eng_matriz[2][4], cor_verde_escuro);
        }
    glPopMatrix();


    glPushMatrix();
        glTranslatef(0.0,60.0,0.0);
        if (linhaClick == 2 && ColunaClick == 1){
            CuboNumeral(eng_matriz[1][0], cor_azul_claro);
        }else{
            CuboNumeral(eng_matriz[1][0], cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(60.0,60.0,0.0);
        if (linhaClick == 2 && ColunaClick == 2){
            CuboOperador(getSinal(1,1), cor_azul_claro);
        }else{
            CuboOperador(getSinal(1,1), cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(120.0,60.0,0.0);
        if (linhaClick == 2 && ColunaClick == 3){
            CuboNumeral(eng_matriz[1][2], cor_azul_claro);
        }else{
            CuboNumeral(eng_matriz[1][2], cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(180.0,60.0,0.0);
        if (linhaClick == 2 && ColunaClick == 4){
            CuboNumeral(eng_matriz[1][3], cor_vermelho_claro);
        }else{
            if(corRespostaCertaErrada(1) == CORRETA)
                CuboNumeral(eng_matriz[1][3], cor_verde_escuro);
            else
                CuboNumeral(eng_matriz[1][3], cor_vermelho_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(240.0,60.0,0.0);
        if (linhaClick == 2 && ColunaClick == 5){
            CuboNumeral(eng_matriz[1][4], cor_verde_claro);
        }else{
            CuboNumeral(eng_matriz[1][4], cor_verde_escuro);
        }
    glPopMatrix();


    glPushMatrix();
        glTranslatef(0.0,120.0,0.0);
        if (linhaClick == 1 && ColunaClick == 1){
            CuboNumeral(eng_matriz[0][0], cor_azul_claro);
        }else{
            CuboNumeral(eng_matriz[0][0], cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(60.0,120.0,0.0);
        if (linhaClick == 1 && ColunaClick == 2){
            CuboOperador(getSinal(0,1), cor_azul_claro);
        }else{
            CuboOperador(getSinal(0,1), cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(120.0,120.0,0.0);
        if (linhaClick == 1 && ColunaClick == 3){
            CuboNumeral(eng_matriz[0][2], cor_azul_claro);
        }else{
            CuboNumeral(eng_matriz[0][2], cor_azul_escuro);
        }
    glPopMatrix();

    glPushMatrix();
        glTranslatef(180.0,120.0,0.0);
        if (linhaClick == 1 && ColunaClick == 4){
            CuboNumeral(eng_matriz[0][3], cor_vermelho_claro);
        }else{
            if(corRespostaCertaErrada(0) == CORRETA)
                CuboNumeral(eng_matriz[0][3], cor_verde_escuro);
            else
                CuboNumeral(eng_matriz[0][3], cor_vermelho_escuro);
        }
    glPopMatrix();


    glPushMatrix();
        glTranslatef(240.0,120.0,0.0);
        if (linhaClick == 1 && ColunaClick == 5){
            CuboNumeral(eng_matriz[0][4], cor_verde_claro);
        }else{
            CuboNumeral(eng_matriz[0][4], cor_verde_escuro);
        }
    glPopMatrix();

    char teste[40];
    glPushMatrix();
        glColor3f(cor_preto[0], cor_preto[1], cor_preto[2]);
        if(fTime >= TempoResolver){
                timeOut();
                renderBitmapString(5,210,(void *)font,"Digite 'R' para resetar o jogo!");
                renderBitmapString(5,192,(void *)font,"Acabou o tempo! Voce Perdeu! Ha Ha Ha!!!");
        }else{
            if(verificaResposta() == COMPLETED){
                resetGame();
                iResetTime = fTime=(glutGet(GLUT_ELAPSED_TIME)/1000);
                pontuacao += 10;
                if(TempoResolver >= 10)
                    TempoResolver-=5;
                renderBitmapString(5,192,(void *)font,"Uauuu! Voce Conseguiu!!!");
            }
            else if(fTime >= TempoResolver/2 && TempoResolver > 5){
                if(embaralhou == 0){
                    geraMatrizAleatoria();
                    embaralhou = 1;
                }
                sprintf(teste,"%d%s",TempoResolver, " segundos!");
                renderBitmapString(5,210,(void *)font,"Arrume as posicoes dos quadrados verdes");
                renderBitmapString(5,200,(void *)font,"para seu lugar correto em ");
                renderBitmapString(105,200,(void *)font,teste);

                renderBitmapString(5,192,(void *)font,"Vamos deixar a coisa mais divertida! Ha Ha Ha Ha!");
            }
            else if(fTime <= TempoResolver){
                sprintf(teste,"%d%s",TempoResolver, " segundos!");
                glColor3f(cor_preto[0], cor_preto[1], cor_preto[2]);
                renderBitmapString(5,210,(void *)font,"Arrume as posicoes dos quadrados verdes");
                renderBitmapString(5,200,(void *)font,"para seu lugar correto em ");
                renderBitmapString(105,200,(void *)font,teste);
                renderBitmapString(180,200,(void *)fonte3,"A quarta coluna deve ter o resultado");
                renderBitmapString(180,192,(void *)fonte3,"da expressao da linha");
            }
        }
    glPopMatrix();

    glPushMatrix();
    char buffer[100] ;
    sprintf(buffer,"Tempo = %d",fTime);
    renderBitmapString(240,240,(void *)fonte2,buffer);

    sprintf(buffer,"Pontuacao = %d",pontuacao);
    renderBitmapString(240,230,(void *)fonte2,buffer);

    glPopMatrix();




    glutSwapBuffers();
}

void anim(void)
{

    if(verificaResposta() == UNCOMPLETED && isFinalizado() == UNCOMPLETED){
        fTime=(glutGet(GLUT_ELAPSED_TIME)/1000) - iResetTime;

        glutPostRedisplay();
    }
}

void mouseClick(int button, int state,int x, int y){

        char buffer[100] ;
        sprintf(buffer,"x = %d, y = %d,state = %d, button = %d",x,y,state,button);
    if (state == 1 && fTime <= TempoResolver && isFinalizado() == UNCOMPLETED) {

        /* Click na primeira coluna*/
        if((x + x_tabuleiro) >=15 && x <= 155){
            if(y >= 494 && y <= 624 ){
                if(linhaClick == 3 & ColunaClick == 1){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 3;
                    ColunaClick = 1;
                }
            }else if(y >= 338 && y <= 468 ){
                if(linhaClick == 2 & ColunaClick == 1){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 2;
                    ColunaClick = 1;
                }
            } else if(y >= 182 && y <= 312 ){
                if(linhaClick == 1 & ColunaClick == 1){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 1;
                    ColunaClick = 1;
                }
            }
        }else if(x >= 180 && x <= 310 ){
            if(y >= 494 && y <= 624 ){
                if(linhaClick == 3 & ColunaClick == 2){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 3;
                    ColunaClick = 2;
                }
            }else if(y >= 338 && y <= 468 ){
                if(linhaClick == 2 & ColunaClick == 2){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 2;
                    ColunaClick = 2;
                }
            } else if(y >= 182 && y <= 312 ){
                if(linhaClick == 1 & ColunaClick == 2){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 1;
                    ColunaClick = 2;
                }
            }
        }else if(x >= 338 && x <= 468 ){
            if(y >= 494 && y <= 624 ){
                if(linhaClick == 3 & ColunaClick == 3){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 3;
                    ColunaClick = 3;
                }
            }else if(y >= 338 && y <= 468 ){
                if(linhaClick == 2 & ColunaClick == 3){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 2;
                    ColunaClick = 3;
                }
            } else if(y >= 182 && y <= 312 ){
                if(linhaClick == 1 & ColunaClick == 3){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    linhaClick = 1;
                    ColunaClick = 3;
                }
            }
        }
        else if(x >= 495 && x <= 625){
            if(y >= 494 && y <= 624 ){
                if(linhaClick == 3 & ColunaClick == 4){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    if(linhaClick != -1 && ColunaClick != -1 && ColunaClick > 3){
                        trocaIndices(linhaClick - 1,ColunaClick - 1,3 - 1,4 - 1);
                        pontuacao -= 1;

                        linhaClick = -1;
                        ColunaClick = -1;
                    }else {
                        linhaClick = 3;
                        ColunaClick = 4;
                    }
                }
            }else if(y >= 338 && y <= 468 ){
                if(linhaClick == 2 & ColunaClick == 4){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    if(linhaClick != -1 && ColunaClick != -1 && ColunaClick > 3){
                        trocaIndices(linhaClick - 1,ColunaClick - 1,2 - 1,4 - 1);
                        pontuacao -= 1;

                        linhaClick = -1;
                        ColunaClick = -1;

                    }else {
                        linhaClick = 2;
                        ColunaClick = 4;
                    }
                }
            } else if(y >= 182 && y <= 312 ){
                if(linhaClick == 1 & ColunaClick == 4){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    if(linhaClick != -1 && ColunaClick != -1 && ColunaClick > 3){
                        trocaIndices(linhaClick - 1,ColunaClick - 1,1 - 1,4 - 1);
                        pontuacao -= 1;
                        linhaClick = -1;
                        ColunaClick = -1;

                    }else {
                        linhaClick = 1;
                        ColunaClick = 4;
                    }
                }
            }
        }else if(x >= 650 && x <= 780 ){
            if(y >= 494 && y <= 624 ){
                if(linhaClick == 3 & ColunaClick == 5){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    if(linhaClick != -1 && ColunaClick != -1 && ColunaClick > 3){
                        trocaIndices(linhaClick - 1,ColunaClick - 1,3 - 1,5 - 1);
                        pontuacao -= 1;
                        linhaClick = -1;
                        ColunaClick = -1;

                    }else {
                        linhaClick = 3;
                        ColunaClick = 5;
                    }
                }
            }else if(y >= 338 && y <= 468 ){
                if(linhaClick == 2 & ColunaClick == 5){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    if(linhaClick != -1 && ColunaClick != -1 && ColunaClick > 3){
                        trocaIndices(linhaClick - 1,ColunaClick - 1,2 - 1,5 - 1);
                        pontuacao -= 1;
                        linhaClick = -1;
                        ColunaClick = -1;

                    }else{
                        linhaClick = 2;
                        ColunaClick = 5;
                    }
                }
            } else if(y >= 182 && y <= 312 ){
                if(linhaClick == 1 & ColunaClick == 5){
                    linhaClick = -1;
                    ColunaClick = -1;
                }else{
                    if(linhaClick != -1 && ColunaClick != -1 && ColunaClick > 3){
                        trocaIndices(linhaClick - 1,ColunaClick - 1,1 - 1,5 - 1);
                        pontuacao -= 1;
                        linhaClick = -1;
                        ColunaClick = -1;
                    }else{
                        linhaClick = 1;
                        ColunaClick = 5;
                    }
                }
            }
        }
        /*MessageBox(0,buffer,"Q",1);/*/
        glutPostRedisplay();
    }

}


void drawBadMonkey()
{

    glPushMatrix();
    glTranslatef(160.0,220.0,0.0);
    glRotatef(0,0,0,1);
    glColor3f(1.0,0.20,0.27);
    glScalef(32,32,1);
    glRotatef(charot,0,1,0);
//    glCallList(character);
    if (pmodel)
        glmDraw(pmodel, GLM_COLOR);

    glPopMatrix();
    glFlush();

    charot=charot+0.2;

}

void drawGoodMonkey()
{
    glPushMatrix();

    glTranslatef(80.0,220.0,0.0);
    glRotatef(0,0,0,1);
    glColor3f(0.0,1.0,0.0);
    glScalef(32,32,1);
    glRotatef(charot1,0,1,0);
    //glCallList(character);
	glmDraw(pmodel, GLM_COLOR);

    charot1=charot1+0.1;

    glPopMatrix();

//    if(charot>360)charot=charot-360;
}
void NormalInput(unsigned char key, int x, int y){
    switch(key)
    {
    case 'r':
    case 'R':
        if(isFinalizado() == COMPLETED){
            pontuacao = 40;
            iResetTime = glutGet(GLUT_ELAPSED_TIME)/1000;
            TempoResolver = TIMEOUT;
            resetGame();
            break;
        }
    }

}
void SpecialInput(int key, int x, int y)
{
    if(isFinalizado() == UNCOMPLETED)
    switch(key)
    {
    case GLUT_KEY_UP:
        if(linhaClick != -1 && ColunaClick != -1 && linhaClick != 1){
            trocaIndices(linhaClick - 1 ,ColunaClick - 1, linhaClick - 2 ,ColunaClick - 1);
            linhaClick = -1;
            ColunaClick = -1;
            pontuacao -= 1;
        }
    //do something here
    break;
    case GLUT_KEY_DOWN:
        if(linhaClick != -1 && ColunaClick != -1 && linhaClick != 3){
            trocaIndices(linhaClick - 1 ,ColunaClick - 1, linhaClick ,ColunaClick - 1);
            linhaClick = -1;
            ColunaClick = -1;
            pontuacao -= 1;

        }
    break;
    case GLUT_KEY_LEFT:
        if(linhaClick != -1 && ColunaClick != -1 && ColunaClick != 4){
            trocaIndices(linhaClick - 1 ,ColunaClick - 1, linhaClick - 1 ,ColunaClick - 2);
            linhaClick = -1;
            ColunaClick = -1;
            pontuacao -= 1;

        }
    break;
    case GLUT_KEY_RIGHT:
        if(linhaClick != -1 && ColunaClick != -1 && ColunaClick != 5){
            trocaIndices(linhaClick - 1 ,ColunaClick - 1, linhaClick - 1 ,ColunaClick);
            linhaClick = -1;
            ColunaClick = -1;
            pontuacao -= 1;

        }
    break;
    }
    glutPostRedisplay();
}
void loadObj2(char *fname){

 if (!pmodel) {		/* load up the model*/
	pmodel = glmReadOBJ(fname);
	glmUnitize(pmodel);
	glmVertexNormals(pmodel, 90.0, GL_TRUE);

 }

}

void loadObj(char *fname)
{
    FILE *fp;
    int read;
    GLfloat x, y, z;
    char ch;
    character=glGenLists(1);
    fp=fopen(fname,"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(2.0);
    glNewList(character, GL_COMPILE);
    {
        glPushMatrix();

        glBegin(GL_TRIANGLE_STRIP);
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v')
            {
                glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    fclose(fp);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    TempoResolver = TIMEOUT;
    glutInitWindowSize(850,650);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Mackaco Travesso");

    glutReshapeFunc(AlteraTamanhoJanela);
    glutDisplayFunc(Desenha);
    glutSpecialFunc(SpecialInput);
    glutMouseFunc(mouseClick);
    glutKeyboardFunc(NormalInput);
    glutIdleFunc(anim);
    Inicializa();
    glutMainLoop();

    return EXIT_SUCCESS;
}
