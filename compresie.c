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



void parcurgere(node *root, int *j)
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



void vector (node *root, qnode *v, int n,int index, int arr)
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
int main(int argc, char *argv[4])
{   int n1,n2,n3;
    int prag =atoi(argv[2]);
    node *root = NULL;
    char *string=malloc(100);
    FILE *f;
    f = fopen(argv[3],"rb");
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
    
    

}