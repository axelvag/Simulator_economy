

// apps/LIFAMI/TPIceScream.cpp
#include <Grapic.h>
#include <math.h>
#include <iostream>

using namespace grapic;
using namespace std;

const int DIMW=350;
const int MAX=50;
const float S=150.0;
const float P0=150.0;
const float Q0=150.0;
const int nbmar=6;

struct Complex
{
    float x,y;
};

Complex make_complex(float x,float y)
{
    Complex z;
    z.x=x;
    z.y=y;
    return z;
}

struct Color
{
    int r,g,b;
};

Color make_color(int r,int g, int b )
{
    Color z;
    z.r=r;
    z.g=g;
    z.b=b;
    return z;
}

struct Marchand
{
    Complex position;
    float prix;
    Color couleur;
    int nbClients;
    int stock=(DIMW*DIMW)/nbmar;
};

struct LesMarchands
{
    Marchand mar[MAX];
    int nm=nbmar;                     //NB DE MARCHAND
};

float Qproduite()
{
    return rand()%5+8;
}

float Qconsomme()
{
    return rand()%5+10;
}

float prixBaril(int n)
{
    float P=P0;
    float Q=Q0;
    for(int i=0;i<n;i++)
    {
        float dQ = Qproduite()-Qconsomme();
        Q = Q + dQ;
        P = P + Q-S;
    }
    return P;
}


void init(LesMarchands & lm)
{
    for(int i=0;i<lm.nm ; i++)
    {
        lm.mar[i].position=make_complex(rand()%DIMW,rand()%DIMW);
        lm.mar[i].prix=rand()%5+2;
        lm.mar[i].couleur=make_color(rand()%256,rand()%256,rand()%256);
        lm.mar[i].nbClients=0;
    }
}

void drawMarchands(LesMarchands lm)
{
    //color(255,0,0)
    for(int i=0;i<lm.nm;i++)
    {
        color(255-lm.mar[i].couleur.r,255-lm.mar[i].couleur.g,255-lm.mar[i].couleur.b);
        circleFill(lm.mar[i].position.x,lm.mar[i].position.y,2);
        print(lm.mar[i].position.x+5,lm.mar[i].position.y,lm.mar[i].prix);
        print(lm.mar[i].position.x+45,lm.mar[i].position.y,"$");
        print(lm.mar[i].position.x+5,lm.mar[i].position.y-14,lm.mar[i].nbClients/100);
    }
}

float distance(Complex a, Complex b)
{
    return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

void TraiteVente(LesMarchands & lm, bool dessiner=false)
{
    for(int i=0;i<lm.nm;i++)
    {
        lm.mar[i].nbClients=0;
    }
    for(int y=0;y<DIMW;y++)
    {
        for(int x=0;x<DIMW;x++)
        {
            Complex pos=make_complex(x,y);
            int mi=0;

            float pi=distance(pos,lm.mar[0].position)+lm.mar[0].prix;
            for(int i=1;i<lm.nm;i++)
            {
                float p=distance(pos,lm.mar[i].position)+lm.mar[i].prix;
                if(p<pi)
                {
                    pi=p;
                    mi=i;
                }
            }
            lm.mar[mi].nbClients++;
            if(dessiner)
            {
                put_pixel(x,y,lm.mar[mi].couleur.r,lm.mar[mi].couleur.g,lm.mar[mi].couleur.b);
            }
        }
    }
}

void MiseAJour(LesMarchands & lm)
{
    //lm=etat a l'instant t-1, avec le bon nb de clients calcule
    LesMarchands finale=lm;
    for (int i=0;i<lm.nm;i++)
    {
        Marchand mi=lm.mar[i];
        LesMarchands lmt = lm;

        lmt.mar[i].prix*=1.1;               //regarde augmente
        TraiteVente(lmt);
        int nb1=lmt.mar[i].nbClients;

        if(lmt.mar[i].prix>10)
            lmt.mar[i].prix=lm.mar[i].prix;
        if(nb1*lmt.mar[i].prix>mi.nbClients*lm.mar[i].prix)
            mi=lmt.mar[i];


        lmt.mar[i].prix*=0.9;              //regarde descend
        TraiteVente(lmt);
        int nb2=lmt.mar[i].nbClients;
        if(nb2*lmt.mar[i].prix>mi.nbClients*lm.mar[i].prix)
            mi=lmt.mar[i];
        lmt.mar[i].prix=lm.mar[i].prix;
        //lmt.mar[i].prix-=(lmt.mar[i].stock-lmt.mar[i].nbClients)/1000;            //pour adapter le prix selon le stock



        lmt.mar[i].position.x+=1;          //regarde a droite
        TraiteVente(lmt);
        int nb3=lmt.mar[i].nbClients;
        if(nb3*lmt.mar[i].prix>mi.nbClients*lm.mar[i].prix)
            mi=lmt.mar[i];

        lmt.mar[i].position=lm.mar[i].position;

        lmt.mar[i].position.x-=1;         //regarde a gauche
        TraiteVente(lmt);
        int nb4=lmt.mar[i].nbClients;
        if(nb4*lmt.mar[i].prix>mi.nbClients*lm.mar[i].prix)
            mi=lmt.mar[i];

        lmt.mar[i].position=lm.mar[i].position;

        lmt.mar[i].position.y+=1;         //regarde en haut
        TraiteVente(lmt);
        int nb5=lmt.mar[i].nbClients;
        if(nb5*lmt.mar[i].prix>mi.nbClients*lm.mar[i].prix)
            mi=lmt.mar[i];

        lmt.mar[i].position=lm.mar[i].position;

        lmt.mar[i].position.y-=1;         //regarde en bas
        TraiteVente(lmt);
        int nb6=lmt.mar[i].nbClients;
        if(nb6*lmt.mar[i].prix>mi.nbClients*lm.mar[i].prix)
            mi=lmt.mar[i];

        lmt.mar[i].position=lm.mar[i].position;

        //mi=resultat de la meilleur action
        finale.mar[i]=mi;

    }
    lm=finale;

}

void draw(LesMarchands lm)
{
    TraiteVente(lm,true);
    drawMarchands(lm);
}

void update(LesMarchands & lm)
{
    MiseAJour(lm);
    //cout<<"oui"<<elapsedTime()<<endl;
}

int main(int argc, char**argv)                                                  //MAIN
{
    bool stop=false;
    winInit("GLACE",DIMW,DIMW);
    backgroundColor(255,255,255);

    LesMarchands LM;
    init(LM);


    while(!stop)
    {
        winClear();
        draw(LM);
        update(LM);
        stop=winDisplay();
    }
winQuit();
return 0;
}
