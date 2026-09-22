#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct image
{
    int largura;
    int altura;
    int** red;
    int** green;
    int** blue;

} image;

image* inicializa(int largura, int altura)
{
    int i,j;
    image* imagem;
    imagem=(image*)malloc(sizeof(image));
    imagem->altura=altura;
    imagem->largura=largura;
    imagem->red = (int **)malloc( altura * sizeof(int *));
    for (i=0; i<altura; i++)
        imagem->red[i] = (int *)malloc(largura * sizeof(int));

    imagem->green = (int **)malloc(altura  * sizeof(int *));
    for (i=0; i<altura; i++)
        imagem->green[i] = (int *)malloc(largura * sizeof(int));

    imagem->blue = (int **)malloc(altura  * sizeof(int *));
    for (i=0; i<altura; i++)
        imagem->blue[i] = (int *)malloc(largura * sizeof(int));

    for (i = 0; i <  imagem->altura; i++)
        for (j = 0; j < imagem->largura; j++)
        {
            imagem->red[i][j] = 0;
            imagem->green[i][j] = 0;
            imagem->blue[i][j] = 0;
        }
    return imagem;
}

image* apaga(image* imagem)
{
    int i;
    for (i=0; i<imagem->altura; i++)
    {
        free(imagem->red[i]);
        free(imagem->green[i]);
        free(imagem->blue[i]);
    }
    free(imagem->red);
    free(imagem->green);
    free(imagem->blue);
    free (imagem);
    return NULL;
}

int save(image* imagem, char* nome)
{

    FILE* arq;
    arq=fopen(nome,"wt");
    if (arq==NULL)return -1;
    int i,j;
    fprintf(arq,"P3\n%d %d\n255\n",imagem->largura, imagem->altura);
    for (i = 0; i <  imagem->altura; i++)
    {
        for (j = 0; j < imagem->largura; j++)
        {
            fprintf(arq,"%d %d %d ",imagem->red[i][j],imagem->green[i][j],imagem->blue[i][j]);

        }
        fprintf (arq,"\n");
    }

    fclose(arq);
    return 0;

}


image* load(char* nome)
{

    FILE* arq;

    char linha1[100];
    int i,j,width,height,maxcolor;
    image* buffer;

    arq=fopen(nome,"rt");
    fgets(linha1,sizeof linha1, arq);
    fgets(linha1,sizeof linha1, arq);
    fscanf(arq,"%i %i",&width,&height);
    fscanf(arq,"%i",&maxcolor);

    buffer = inicializa(width, height);

    for (i = 0; i <  buffer->altura; i++)
    {
        for (j = 0; j < buffer->largura; j++)
        {
            fscanf(arq,"%i",&(buffer->red[i][j]));
            fscanf(arq,"%i",&(buffer->green[i][j]));
            fscanf(arq,"%i",&(buffer->blue[i][j]));
        }
    }

    fclose(arq);
    return buffer;

}

image* negativo(image* buffer)
{
    int i, j;
    for (i = 0; i <  buffer->altura; i++)
    {
        for (j = 0; j < buffer->largura; j++)
        {
            buffer->red[i][j] = 255 - buffer->red[i][j];
            buffer->green[i][j] = 255 - buffer->green[i][j];
            buffer->blue[i][j] =  255 - buffer->blue[i][j];
        }
    }
    return buffer;

}

image* carimbo(image* im1, image* im2,int y, int x)
{
    int i,j;
    for (i=0; i<im2->altura; i++)
        for (j=0; j<im2->largura; j++)
        {
            if(im2->red[i][j]!=255 && im2->green[i][j] !=255 && im2->blue[i][j]!=255)
            {
                im1->red[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->red[i][j];
                im1->green[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->green[i][j];
                im1->blue[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->blue[i][j];
            }
        }

    return im1;

}

image* carimbo2(image* im1, image* im2,int y, int x,int c, int r, int g, int b)
{

    int digit=0;
    int i,j,col,flag,currcol,gap;
    col=0;
    currcol=0;

    while(digit!=c)
    {
        gap=0;
        for (j=col; j<im2->largura,gap==0; j++)
            for (i=0; i<im2->altura; i++)
            {
                if(im2->red[i][j]==0 && im2->green[i][j] ==0 && im2->blue[i][j]==0)
                {
                    col=j;
                    gap=1;
                    break;
                }
            }

        flag=1;
        for (j=col; j<im2->largura,flag==1; j++)
        {
            flag=0;
            currcol=j;
            for (i=0; i<im2->altura; i++)
            {
                {
                    if(im2->red[i][j]==0 && im2->green[i][j] ==0 && im2->blue[i][j]==0)
                    {
                        flag=1;

                    }
                }
            }
        }
        digit++;
        col=currcol;
    }
    gap=0;
    for (j=col; j<im2->largura,gap==0; j++)
        for (i=0; i<im2->altura; i++)
        {
            if(im2->red[i][j]==0 && im2->green[i][j] ==0 && im2->blue[i][j]==0)
            {
                col=j;
                gap=1;
                break;
            }
        }

    flag=1;
    for (j=col; j<im2->largura,flag==1; j++)
    {
        flag=0;
        for (i=0; i<im2->altura; i++)
        {
            {
                if(im2->red[i][j]==0 && im2->green[i][j] ==0 && im2->blue[i][j]==0)
                {
                    flag=1;
                    im1->red[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=r;//im2->red[i][j];
                    im1->green[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=g;//im2->green[i][j];
                    im1->blue[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=b;//im2->blue[i][j];
                }
            }
        }
    }

    return im1;

}



int main()
{


    int i,j,col;
    image* figura1;
    image* figura2;
    figura1 = load("imagens/impactfont.ppm");
    figura2 = load("imagens/whenIdo.ppm");

    int count = 0;
    char* up;
    char* bottom;
    char* indexes;
    up=malloc(50);
    bottom=malloc(50);
    indexes=malloc(100);
    strcpy(indexes,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ");
    strcpy(up,"I do not always code memes");
    strcpy(bottom,"But when I do it, I do in C");


    figura2=carimbo2(figura2,figura1,10, 10,23,255,255,255);
    figura2=carimbo2(figura2,figura1,50, 10,20,255,255,255);
    figura2=carimbo2(figura2,figura1,100, 10,13,255,255,255);

    //for (i=0;i<20;i++)
    //figura2=carimbo2(figura2,figura1,rand()%(figura2->largura), rand()%(figura2->altura),0);

    save(figura2,"teste.ppm" );

/// Para converter qualquer imagem para PPM no Linux utilize o comando abaixo:
/// convert photo123.jpg -compress none photo123.ppm

    //
    //  Inclua os free necessarios para liberar memoria ao final

    return 0;
}

