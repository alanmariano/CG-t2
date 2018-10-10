//
// INF390 - Computação Gráfica
//
// Trabalho 2
//
// Alan Mariano da Silva - 04/10/2018
//

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm> 
#include <math.h> 
#include <cstdlib>      // std::rand, std::srand

using namespace std;

struct ObstaculosOrdenados{
    float xe, xd, yi, ys;
} ob_ord[10];



float xPersonagemEsquerdo = 0.0;
float xPersonagemDireito = 5.0;
float yPersonagemInferior = 12.0;
float yPersonagemSuperior = 17.0;


float yPersonagemInferiorNivel1 = 21.5;
float yPersonagemSuperiorNivel1 = 26.5;

int ilimitado = 0;
int quadros = 30;
bool colisao = false;
bool flagNivel1 = true;
int obstaculoAtual = 0;
float posPersonagemDireito;
float velocidadeY = 17.0;
float g = 9.8;
float tFinalY = 2*velocidadeY/g;
float tempoTranscorridoPulo = 0; 
float posYPulo;

int nivel1 = false;

int gameMode = 0;

int pulando  = false;
float tempoPulo = 0;



void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void desenha_cenario();
void desenha_chao();
void desenha_personagem();
void escolhe_obstaculos();
void ordena_obstaculos();
bool sortByPos(const ObstaculosOrdenados &ob1, const ObstaculosOrdenados &ob2);



//tamanho da janela em pixels
int largurajanela = 500, alturajanela = 500;
float adder = 0.0;
int obstaculos[8] = {0};




float cores[6][3] = {{0.93, 0.0, 0.0},{1.0, 0.58, 0.0},{0.95, 1.0, 0.0},{0.0, 0.56, 0.14},
                    {0.38, 0.0, 1.0},{0.51, 0.0, 0.53}};


bool sortByPos(const ObstaculosOrdenados &ob1, const ObstaculosOrdenados &ob2){ return ob1.xe < ob2.xe; }



/*
TIPOS DE OBSTACULO
1 - simples (posicoes 0, 1 e 2 do array obstaculos)
2 - duplo horizontal (posicoes 3 e 4 do array obstaculos)
3 - duplo vertical (posicoes 5, 6 e 7 do array obstaculos)
*/
void escolhe_obstaculos(){
    int pos;
    bool flag;
    for(int i=0; i<8; i++){
        flag = false;
        pos = rand()%82 + 8;
        for(int j=0; j<i; j++){
            if((pos > (obstaculos[j] - 7)) && (pos < (obstaculos[j]+7))){
                flag = true;
                break;
            }
        }
        if(flag){
            i--;
        }else{
            obstaculos[i] = pos;
            if(i<3){
                ob_ord[i].xe = 5.0 * pos;
                ob_ord[i].xd = (5.0 * pos) + 5;
                ob_ord[i].yi = 12.0;
                ob_ord[i].ys = 12.0 + 5;
            }else if(i<5){
                ob_ord[i].xe = 5.0 * pos;
                ob_ord[i].xd = (5.0 * pos) + 10;
                ob_ord[i].yi = 12.0;
                ob_ord[i].ys = 12.0 + 5;
            }else{
                ob_ord[i].xe = 5.0 * pos;
                ob_ord[i].xd = (5.0 * pos) + 5;
                ob_ord[i].yi = 12.0;
                ob_ord[i].ys = 12.0 + 10;
            }
        }
    }


    // degrau para subir no nivel1 de chegada como ultimo obstaculo
    ob_ord[8].xe = 500.0;
    ob_ord[8].xd = 505.0;
    ob_ord[8].yi = 12.0;
    ob_ord[8].ys = 21.4;


    // ponto de chegada como ultimo obstaculo
    ob_ord[9].xe = 545.0;
    ob_ord[9].xd = 546.0;
    ob_ord[9].yi = 21.5;
    ob_ord[9].ys = 36.5;


    sort(ob_ord, ob_ord+8, sortByPos);

}

void desenha_cenario(){
    //terra
    glColor3f(0.59, 0.47, 0.43);
    glBegin(GL_POLYGON);
        glVertex2f(0.0, -50.0);
        glVertex2f(502.6, -50.0);
        glVertex2f(502.6, 10.0);
        glVertex2f(0.0, 10.0);
    glEnd();

    glBegin(GL_POLYGON);
        glVertex2f(502.6, -50.0);
        glVertex2f(550.0, -50.0);
        glVertex2f(550.0, 19.5);
        glVertex2f(502.6, 19.5);
    glEnd();

    if(!gameMode){
        //sol
        glColor3f(1.0, 0.76, 0.18);
    }else{
        //lua
        glColor3f(0.76, 0.76, 0.76);
    }
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glVertex2f(10.0 + adder, 34.0 + (posYPulo/2)); // center of circle
    for (int i = 0; i <= 20; i++)   {
        glVertex2f (
            (10.0 + adder + (3.0 * cos(i * 3.14 * 2 / 20))), (34.0 + (posYPulo/2) + (3.0 * sin(i * 3.14 * 2 / 20)))
            );
    }
    glEnd(); //END



}

// desenha uma face do cubo
// a, b, c e d sao indices no vetor de vertices
// ncolor eh um indice no vetor de cores
void desenha_chao() {

    //chao nivel 0
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(0.0, 10.0);
        glVertex2f(500.0, 10.0);
        glVertex2f(500.0, 12.0);
        glVertex2f(0.0, 12.0);
    glEnd();

    //parede entre chao nivel 0 e chao nivel 1
    glBegin(GL_POLYGON);
        glVertex2f(500.0, 10.0);
        glVertex2f(502.5, 10.0);
        glVertex2f(502.5, 19.5);
        glVertex2f(500.0, 19.5);
    glEnd();

    //chao nivel 1
    glBegin(GL_POLYGON);
        glVertex2f(500.0, 19.5);
        glVertex2f(550.0, 19.5);
        glVertex2f(550.0, 21.5);
        glVertex2f(500.0, 21.5);
    glEnd();

    //ponto de chegada
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2f(545.0, 21.5);
        glVertex2f(546.0, 21.5);
        glVertex2f(546.0, 36.5);
        glVertex2f(545.0, 36.5);
    glEnd();

    //bandeira
    for(int i=0; i<6; i++){
        glColor3fv(cores[i]);
        glBegin(GL_POLYGON);
            glVertex2f(546.0, 35.5 - i);
            glVertex2f(555.0, 35.5 - i);
            glVertex2f(555.0, 36.5 - i);
            glVertex2f(546.0, 36.5 - i);
        glEnd();
    }


//OBSTACULOS
/////////////////////////////////////////////////////////////
    if(!gameMode){
        glColor3f(0.0, 0.0, 0.0);
    }else{
        glColor3f(1.0, 1.0, 1.0);
    }    
    for (int i=0; i<8; i++){
        glBegin(GL_TRIANGLES);
            glVertex2f(obstaculos[i]*5, 12.0);
            glVertex2f(obstaculos[i]*5 + 5, 12.0);
            glVertex2f(obstaculos[i]*5 + 2.5, 17.0);
        glEnd();

        if(i == 3 || i == 4){
            glBegin(GL_TRIANGLES);
                glVertex2f(obstaculos[i]*5 + 5, 12.0);
                glVertex2f(obstaculos[i]*5 + 10, 12.0);
                glVertex2f(obstaculos[i]*5 + 5 + 2.5, 17.0);
            glEnd();
        }
        if (i>4){
            glBegin(GL_TRIANGLES);
                glVertex2f(obstaculos[i]*5, 17.0);
                glVertex2f(obstaculos[i]*5 + 5, 17.0);
                glVertex2f(obstaculos[i]*5 + 2.5, 22.0);
            glEnd();
        }
    }
    



}

void desenha_personagem() {


    glColor3f(0.83, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(xPersonagemEsquerdo + adder, yPersonagemInferior);
        glVertex2f(xPersonagemDireito + adder, yPersonagemInferior);
        glVertex2f(xPersonagemDireito + adder, yPersonagemSuperior - 2.5);
        glVertex2f(xPersonagemEsquerdo + adder, yPersonagemSuperior - 2.5);
    glEnd();

    glColor3f(0.97, 0.93, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(xPersonagemEsquerdo + adder, yPersonagemInferior + 2.5);
        glVertex2f(xPersonagemDireito + adder, yPersonagemInferior + 2.5);
        glVertex2f(xPersonagemDireito + adder, yPersonagemSuperior);
        glVertex2f(xPersonagemEsquerdo + adder, yPersonagemSuperior);
    glEnd();
}


void init(void) {
    //game mode (dia = 0, noite = 1)
    gameMode = rand() % 2;

    if(!gameMode){
        glClearColor(0.0, 0.44, 0.61,1.0); //azul escuro (dia)
    }else{
        glClearColor(0.0, 0.0, 0.0, 1.0); //preto (noite)
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0.0, 50.0, -10.0, 50.0);

    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela

    escolhe_obstaculos();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0 + adder, 50.0 + adder, -10.0 + posYPulo, 40.0 + posYPulo);
    
    desenha_cenario();
    desenha_chao();
    desenha_personagem();

    glMatrixMode(GL_MODELVIEW);
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {

    if(key == 32 && !pulando && !nivel1){
        cout << "espaço <> PUla" << endl;
        /*yPersonagemInferior+= 15;
        yPersonagemSuperior+=15;*/
        tempoPulo = glutGet(GLUT_ELAPSED_TIME);
        pulando = true;
    }

    //display();
}


int main(int argc, char **argv) {
    srand(time(NULL));
    glutInit(&argc,argv); //inicializa a glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //tipo de buffer/cores
    glutInitWindowSize(largurajanela, alturajanela); //dimens�es da janela
    glutInitWindowPosition(100, 100); //posicao da janela
    glutCreateWindow("Trabalho 2"); //cria a janela
    init();
    glutDisplayFunc(display); //determina fun��o callback de desenho
    glutKeyboardFunc(keyboard); //determina fun��o callback de teclado

    //glutMainLoop();
    
    glutMainLoopEvent();
    
    int tempo_novo = glutGet(GLUT_ELAPSED_TIME);// tempo atual em milissegundos
    int tempo_antigo = tempo_novo;
    int variacao_tempo = 0;
    
    while (!colisao){ //ENQUANTO o programa não encerrar FAÇA
        tempo_novo = glutGet(GLUT_ELAPSED_TIME); //tempo atual em milissegundos
        variacao_tempo = tempo_novo - tempo_antigo;
        if(ilimitado || variacao_tempo > 1000/quadros){
            tempo_antigo = tempo_novo;
            adder += 1.0;
            glutMainLoopEvent();

            if(adder>495){
                nivel1 = true;
            }
            
            ////CHECAGEM DE COLISAO
            posPersonagemDireito = xPersonagemDireito+adder-1.5;

            colisao = false;
            if( ob_ord[obstaculoAtual].xe  <= posPersonagemDireito && posPersonagemDireito <= ob_ord[obstaculoAtual].xd && ((ob_ord[obstaculoAtual].yi <= yPersonagemInferior && yPersonagemInferior <= ob_ord[obstaculoAtual].ys) || (ob_ord[obstaculoAtual].yi <= yPersonagemSuperior && yPersonagemSuperior <= ob_ord[obstaculoAtual].ys)  ) ){
                colisao = true;

                if(obstaculoAtual==9){
                    cout << "ganhou" <<endl;
                }else{
                    cout << "perdeu" << endl;
                }

            }else if( adder > ob_ord[obstaculoAtual].xe ){
                obstaculoAtual+=1;
            }

        }
        if(pulando){
            tempoTranscorridoPulo = 3* (glutGet(GLUT_ELAPSED_TIME) - tempoPulo)/1000;

            if(tempoTranscorridoPulo >= tFinalY){
                    pulando = false;
            }

            if(nivel1 && yPersonagemInferior > yPersonagemInferiorNivel1){
                posYPulo = (velocidadeY*tempoTranscorridoPulo) - (0.5 * g * (tempoTranscorridoPulo * tempoTranscorridoPulo)); 
                yPersonagemInferior = 12.0 + posYPulo;
                yPersonagemSuperior = 17.0 + posYPulo;
                if(yPersonagemInferior <= yPersonagemInferiorNivel1 ){
                    yPersonagemInferior = yPersonagemInferiorNivel1;
                    yPersonagemSuperior = yPersonagemSuperiorNivel1;
                    pulando = false;
                }
            }else{
                if(tempoTranscorridoPulo >= tFinalY){
                    posYPulo = 0.0;
                }else{
                    posYPulo = (velocidadeY*tempoTranscorridoPulo) - (0.5 * g * (tempoTranscorridoPulo * tempoTranscorridoPulo)); 
                }

                yPersonagemInferior = 12.0 + posYPulo;
                yPersonagemSuperior = 17.0 + posYPulo;
            }

            

       
        }

        

    } 

    
    

    return 0;
}