#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>

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
    int size;
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

node **arbore( qnode *v, int n, node **nod, int i, int matrixsize)
{
    int a,b,c,d;
    unsigned char blue,green,red;
    blue = v[i].blue;
    red = v[i].red;
    green = v[i].green;
    *nod = create(blue,green,red);
    (*nod)->nr = i;
    a = v[i].top_left;
    b = v[i].top_right;
    c = v[i].bottom_right;
    d = v[i].bottom_left;
    matrixsize = sqrt((double)v[0].area);
    (*nod)->size = matrixsize; 
    if( a == -1 )
    {
        (*nod)->first = NULL;    
    }
    else
    {
        arbore(v,n,&(*nod)->first,a,matrixsize);
        (*nod)->first->nr = a;
        (*nod)->first->size = matrixsize/4;
    }
    if( b == -1 )
    {
        (*nod)->second=NULL;    
    }
    else
    {
        arbore(v,n,&(*nod)->second,b,matrixsize);
        (*nod)->second->nr = b;
        (*nod)->second->size = matrixsize/4;
    }
    if( c == -1 )
    {
        (*nod)->third = NULL;    
    }
    else
    {
        arbore(v,n,&(*nod)->third,c,matrixsize);
        (*nod)->third->nr = c;
        (*nod)->third->size = matrixsize/4;
    }
    if( d == -1 )
    {
        (*nod)->forth = NULL;    
    }
    else
    {
        arbore(v,n,&(*nod)->forth,d,matrixsize);
        (*nod)->forth->nr = d;
        (*nod)->forth->size = matrixsize/4;
    }
    return nod;       
}


void matrix(square **M, int n, node **root,int x, int y)
{
    int a,b;
    if( (*root)->first == NULL)
    {
        
        for(a=x;a<n+x;a++)
            {for(b=y;b<y+n;b++)
                {
                    M[a][b].blue=(*root)->blue;
                    M[a][b].green=(*root)->green;
                    M[a][b].red=(*root)->red;
                }}
    }
    else
    {   
        matrix(M,n/2,&(*root)->first,x,y);
        matrix(M,n/2,&(*root)->second,x,y+n/2);
        matrix(M,n/2,&(*root)->third,x+n/2,y+n/2);
        matrix(M,n/2,&(*root)->forth,x+n/2,y);
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

int main(int argc, char *argv[3])
{
    int i;
    int a,b;
    uint32_t k,c;
    node *root = NULL;
    FILE *f;
    f = fopen(argv[2],"rb");
    fread(&c,sizeof(uint32_t),1,f);
    fread(&k,sizeof(uint32_t),1,f);
    qnode *v = malloc(sizeof(qnode)*k);
    for(i=0;i<k;i++)
        {
            fread(v + i,sizeof(qnode),1,f);    
        }
    fclose(f);
    int size = sqrt((double)v[0].area);
        square **M = (square **)malloc(size*sizeof(square*));
    for(int k=0;k<size;k++)
        {M[k] = (square *)malloc(size*sizeof(square));}
    arbore(v,k,&root,0,size);
    matrix(M,size,&root,0,0);
    FILE *g;
    g = fopen(argv[3],"wb");
    fprintf(g,"P6\n");
    fprintf(g,"%d %d\n%d\n", size,size,255);
    for(a=0;a<size;a++)
        for(b=0;b<size;b++)
            {fwrite(&M[a][b].red,1,1,g);
            fwrite(&M[a][b].green,1,1,g);
            fwrite(&M[a][b].blue,1,1,g);
            }
    fclose(g);
    free(v);
    for(int i=0; i<size; i++)
        free(M[i]);
    free(M);
    clean(root);
    
}