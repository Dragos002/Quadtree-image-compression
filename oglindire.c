#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include<string.h>

typedef struct qnode
{
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
}__attribute__((packed)) qnode;

typedef struct node
{
    unsigned char blue,green,red;
    struct node *first;
    struct node *second;
    struct node *third;
    struct node *forth;
    int nr;
}node;

typedef struct square
{
    unsigned char blue,green,red;

}__attribute__((packed)) square;


 node *create(unsigned char blue, unsigned char green, unsigned char red)
{
    node *new =(node *)malloc(sizeof(node));
    new->blue = blue;
    new->green = green;
    new->red = red;
    new->first = new->second = new->third = new->forth = NULL;
    return (new);
}

square color ( square **M, int x, int y, int n)
{
    int i,j;
    square culoare;
    unsigned long long blue=0,green=0,red=0;
    for(i=x;i<(x+n);i++)
        {for(j=y;j<(y+n);j++)
            {   blue = blue + M[i][j].blue;
                green = green + M[i][j].green;
                red = red + M[i][j].red;}}
    culoare.blue = blue/pow(n,2);
    culoare.green = green/pow(n,2);
    culoare.red = red/pow(n,2);
    return culoare;
}

int check ( square **M, int n, int x, int y, unsigned char blue, unsigned char green, unsigned char red, int prag)
{  
   square culoare = color (M,x,y,n);
    blue = culoare.blue;
    green = culoare.green;
    red = culoare.red;
    
    int i,j;
    unsigned long long mean=0;
    for(i=x; i<x+n; i++)
        {for(j=y;j<y+n; j++){
            mean += ( pow(red - M[i][j].red,2) + pow(green - M[i][j].green,2) + pow(blue - M[i][j].blue,2) );}}
        mean = mean / (3 * n * n);
    if (mean <= prag)
        return 1; //nu mai este nevoie de divizare
        else
        return 0; // este nevoie de divizare
}

node **divide(square **M , int n, int x, int y,unsigned char blue, unsigned char green, unsigned char red, int prag, node **nod, uint32_t *k, uint32_t *c)
{   *k=*k+1;
    square culoare = color(M,x,y,n);
    blue = culoare.blue;
    green = culoare.green;
    red = culoare.red;
    if (check(M,n,x,y,blue,green,red,prag) == 0)
    {   *nod = create(blue,green,red);
     divide(M,n/2,x,y,blue,green,red,prag,&(*nod)->first,k,c);
     divide(M,n/2,x,y+n/2,blue,green,red,prag,&(*nod)->second,k,c);
    divide(M,n/2,x+n/2,y+n/2,blue,green,red,prag,&(*nod)->third,k,c);
       divide(M,n/2,x+n/2,y,blue,green,red,prag,&(*nod)->forth,k,c);
        return nod;
        
    }
    else
    {   *c = *c +1;
        *nod = create(blue,green,red);
        return nod;
    }
    
}

void horizontal(node *root)
{
    if(root->first!=NULL)
    {node *a,*b,*c,*d;
    horizontal(root->first);
    horizontal(root->second);
    horizontal(root->third);
    horizontal(root->forth);
    a = root->first;
    b = root->second;
    c = root->third;
    d = root->forth;
    root->first = b;
    root->second = a;
    root->third = d;
    root->forth = c;}
}

void vertical(node *root)
{
    if(root->first != NULL)
    {
        node *a,*b,*c,*d;
        vertical(root->first);
        vertical(root->second);
        vertical(root->third);
        vertical(root->forth);
        a = root->first;
        b = root->second;
        c = root->third;
        d = root->forth;
        root->first = c;
        root->second = d;
        root->third = a;
        root->forth = b;

    }
}

void matrix(square **H, int n, node **root,int x, int y)
{
    int a,b;
    if( (*root)->first == NULL)
    {
        
        for(a=x;a<n+x;a++)
            {for(b=y;b<y+n;b++)
                {
                    H[a][b].blue=(*root)->blue;
                    H[a][b].green=(*root)->green;
                    H[a][b].red=(*root)->red;
                }}
    }
    else
    {   
        matrix(H,n/2,&(*root)->first,x,y);
        matrix(H,n/2,&(*root)->second,x,y+n/2);
        matrix(H,n/2,&(*root)->third,x+n/2,y+n/2);
        matrix(H,n/2,&(*root)->forth,x+n/2,y);
    }

}

void clean(node* root)
{
    if(root ==NULL)
        return;
    clean(root->first);
    clean(root->second);
    clean(root->third);
    clean(root->forth);
    free(root);
}

int main(int argc, char *argv[5])
{   int x,y;
    int n1,n2,n3;
    int prag = atoi(argv[3]);
    node *root = NULL;
    char *string=malloc(100);
    FILE *f;
    f = fopen(argv[4],"rb");
    fscanf(f,"%s", string);
    free(string);
    char a;
    fscanf(f,"%d%d%d", &n1,&n2,&n3);
    fscanf(f, "%c", &a);
    square **M = (square **)malloc(n1*sizeof(square*));
    for(int k=0;k<n1;k++)
        {M[k] = (square *)malloc(n1*sizeof(square));}
    int i,j;
    for(i=0;i<(n1);i++)
        {for(j=0;j<(n2);j++)
            {fread(&M[i][j].red,sizeof(unsigned char),1,f);
             fread(&M[i][j].green,sizeof(unsigned char),1,f);            
             fread(&M[i][j].blue,sizeof(unsigned char),1,f);}}
    fclose(f);
    square culoare = color(M,0,0,n1);
    unsigned char blue,red,green;
    blue = culoare.blue;
    red= culoare.red;
    green = culoare.green;
    uint32_t k = 0;
    uint32_t c = 0;
    divide(M,n1,0,0,blue,green,red,prag,&root,&k, &c);
    square **H = (square **)malloc(n1*sizeof(square*));
    for(int k=0;k<n1;k++)
        {H[k] = (square *)malloc(n1*sizeof(square));}
    if(strcmp(argv[2],"v") == 0 )
        {horizontal(root);
        vertical(root);}
    else
        {horizontal(root);}
    matrix(H,n1,&root,0,0);
    FILE *g;
    g = fopen(argv[5],"wb");
    fprintf(g,"P6\n");
    fprintf(g,"%d %d\n%d\n", n1,n1,255);
    for(x=0;x<n1;x++)
        for(y=0;y<n1;y++)
            {fwrite(&H[x][y].red,1,1,g);
            fwrite(&H[x][y].green,1,1,g);
            fwrite(&H[x][y].blue,1,1,g);
            }
    fclose(g);
    for(int i=0; i<n1; i++)
        free(M[i]);
    free(M);
    for(int i=0; i<n1; i++)
        free(H[i]);
    free(H);
    clean(root);


}