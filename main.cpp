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
#include <cstdlib>      // std::rand, std::srand

using namespace std;

struct ObstaculosOrdenados{
    float xe, xd, yi, ys;
} ob_ord[10];


float xPersonagemDireito = 0.0;
float xPersonagemEsquerdo = 5.0;
float yPersonagemInferior = 12.0;
float yPersonagemSuperior = 17.0;



void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
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
            if((pos > (obstaculos[j] - 6)) && (pos < (obstaculos[j]+6))){
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
    ob_ord[8].xe = 500;
    ob_ord[8].xd = 505;
    ob_ord[8].yi = 12.0;
    ob_ord[8].ys = 21.4;


    // ponto de chegada como ultimo obstaculo
    ob_ord[9].xe = 545;
    ob_ord[9].xd = 546;
    ob_ord[9].yi = 21.5;
    ob_ord[9].ys = 36.5;


    sort(ob_ord, ob_ord+8, sortByPos);

    for (int i=0; i<8; i++){
        cout << i << "|| sem ordem: " << obstaculos[i] << " || com ordem: " << ob_ord[i].xe << endl;
    }

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
    glColor3f(0.0, 0.0, 1.0);
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
    glClearColor(0.0, 0.0, 0.0, 1.0); // cor para limpeza do buffer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0.0, 50.0, 0.0, 50.0);

  
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela

    escolhe_obstaculos();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa a janela
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0 + adder, 50.0 + adder, 0.0, 50.0);
    
    desenha_chao();
    desenha_personagem();
    
    //adder += 5.0;

    glMatrixMode(GL_MODELVIEW);
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    display();
}

int ilimitado = 1;
int quadros = 0;
bool colisao = false;
bool flagNivel1 = true;
int obstaculoAtual = 8;

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
            adder += 1.5;
            glutMainLoopEvent();
        }

        if(xPersonagemEsquerdo + adder >= 500 && flagNivel1){
            yPersonagemInferior += 9.5;
            yPersonagemSuperior += 9.5;
            flagNivel1 = false;
        }

        colisao = false;
        if( ob_ord[obstaculoAtual].xe  <= xPersonagemDireito+adder && xPersonagemDireito+adder <= ob_ord[obstaculoAtual].xd && ((ob_ord[obstaculoAtual].yi <= yPersonagemInferior && yPersonagemInferior <= ob_ord[obstaculoAtual].ys) || (ob_ord[obstaculoAtual].yi <= yPersonagemSuperior && yPersonagemSuperior <= ob_ord[obstaculoAtual].ys)  ) ){
            colisao = true;
        }
        else if( xPersonagemDireito+adder > ob_ord[obstaculoAtual].xe ){
            obstaculoAtual+=1;
        }

        cout << obstaculoAtual << endl;
        
        if(colisao == true && obstaculoAtual==9){
            cout << "ganhou" <<endl;
        }else if(colisao == true){
            cout << "perdeu" << endl;
        }
        

    } 

    
    

    return 0;
}