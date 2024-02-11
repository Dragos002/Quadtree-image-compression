#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include<string.h>

typedef struct qnode //structura data
{
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
}__attribute__((packed)) qnode;

typedef struct node // structura folosita pentru arbore
{
    unsigned char blue,green,red;
    struct node *first;
    struct node *second;
    struct node *third;
    struct node *forth;
    int nr;
    int size;
}node;

typedef struct square // structura pentru memorarea matricei
{
    unsigned char blue,green,red;

}__attribute__((packed)) square;


 node *create(unsigned char blue, unsigned char green, unsigned char red) // functia creeaza un nou nod 
{
    node *new =(node *)malloc(sizeof(node));
    new->blue = blue;
    new->green = green;
    new->red = red;
    new->first = new->second = new->third = new->forth = NULL;
    return (new);
}

square color ( square **M, int x, int y, int n) //functia face culoarea medie pentru un "square" din matrice
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

int check ( square **M, int n, int x, int y, unsigned char blue, unsigned char green, unsigned char red, int prag) //functia verifica daca "square-ul" mai trebuie sau nu divizat
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

node **arbore( qnode *v, int n, node **nod, int i, int matrixsize) // functia creeaza arborele din vectorul primit
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
        (*nod)->second = NULL;    
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


node **divide(square **M , int n, int x, int y,unsigned char blue, unsigned char green, unsigned char red, int prag, node **nod, uint32_t *k, uint32_t *c)
{   *k=*k+1; // functia creeaza un arbore din matrice (impartind matricea recursiv in cate 4 sub matrici)
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

void horizontal(node *root) // functia modifica arborele conform oglindirii verticale
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

void matrix(square **H, int n, node **root,int x, int y) //functia creeaza matricea pe baza arborelui
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

void parcurgere(node *root, int *j) // parcurge si noteaza fiecare nod din arbore
{   
    if (root == NULL)
        return;
    if ( *j== 0 )
        root->nr = *j;
    if(root->first != NULL)
    {
        *j=*j+1;
        root->first->nr = *j;
        parcurgere(root->first,j);
    }
    if(root->second != NULL)
    {
        *j=*j+1;
        root->second->nr = *j;
        parcurgere(root->second,j);
    }
    if(root->third != NULL)
    {
        *j=*j+1;
        root->third->nr = *j;
        parcurgere(root->third,j);
    }
    if(root->forth != NULL)
    {
        *j=*j+1;
        root->forth->nr = *j;
        parcurgere(root->forth,j);
    }
}



void vector (node *root, qnode *v, int n,int index, int arr) // adauga in vector nodurile arborelui
{   
    v[index].red = root->red;
    v[index].blue=root->blue;
    v[index].green=root->green;
    v[index].area = (n*n)/pow(4, arr);
    if (root->first != NULL)
         {v[index].top_left = root->first->nr;
         vector(root->first,v,n,root->first->nr,arr + 1);}
         else
         {
             v[index].top_left = -1;
         }
    if (root->second != NULL)
        {v[index].top_right = root->second->nr;
        vector(root->second,v,n,root->second->nr,arr + 1);}
        else
        {
            v[index].top_right = -1;
        }
    if (root->forth != NULL)
        {v[index].bottom_left= root->forth->nr;
        vector(root->forth,v,n,root->forth->nr,arr + 1);}
        else
        {
            v[index].bottom_left = -1;
        }
    if (root->third != NULL)
        {v[index].bottom_right = root->third->nr;
        vector(root->third,v,n,root->third->nr,arr + 1);}
        else
        {
            v[index].bottom_right = -1;
        }
}

void clean(node* root) // elibereaza memoria alocata arborelui
{
    if(root ==NULL)
        return;
    clean(root->first);
    clean(root->second);
    clean(root->third);
    clean(root->forth);
    free(root);
}

int main(int argc, char *argv[])
{   int x,y;
    int n1,n2,n3;
    node *root = NULL;
    if(strcmp(argv[1],"-m") == 0 || strcmp(argv[1],"-c") ==0)
    {
        int prag;
        if(strcmp(argv[1],"-c") ==0)
            { prag = atoi(argv[2]);}
        else
            { prag = atoi(argv[3]);}
    char *string=malloc(100);
    FILE *f;
    if(strcmp(argv[1],"-c") ==0)
        {f = fopen(argv[3],"rb");}
    else
        {f = fopen(argv[4],"rb");}
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
        if(strcmp(argv[1],"-m")==0)
     {   
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
    else 
    {
        qnode *v = malloc(sizeof(qnode)*k);
    int l = 0;
    parcurgere(root,&l);
    vector(root,v,n1,0,0);
     FILE *g;
   g = fopen(argv[4],"wb");
   
   fwrite(&c,sizeof(uint32_t),1,g);
   fwrite(&k,sizeof(uint32_t),1,g);
   for(int p=0;p<k;p++)
        { 
            fwrite(&v[p],sizeof(qnode),1,g);}
    fclose(g);
    free(v);
    for(int i=0; i<n1; i++)
        free(M[i]);
    free(M);
    clean(root);
    }}
    else
    {
        if(strcmp(argv[1],"-d") == 0)
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
    
    
    }
}