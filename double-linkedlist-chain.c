#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
// #include<conio.h>
#define ID_DIGITS 1

//struct declaration========================================================================================
struct buku//menampung informasi buku
{
    char judul[20];
    char genre[20];
    char hal[20];
    char namaPenerbit[20];
    struct buku *next,*prev;
}*tempBuku;

struct genre//menampung head linkedlist buku setiap genre
{
    struct buku *head;
    char genre[20];
    struct genre *next;
}*headGenre,*tempGenre,*tailGenre;

//Fungsi mempermudah kehidupan==============================================================================

struct queueBuku{
    struct buku* array[100];
    int rear,front,size;
};

struct queueBuku* buatQueue(){
    struct queueBuku* z=(struct queueBuku*)malloc(sizeof(struct queueBuku));
    z->rear=-1;
    z->front=0;
    z->size=0;
    return z;
}

void push(struct queueBuku* Queue,struct buku* bk){
    if (Queue->size!=100)
    {
        Queue->size++;
        Queue->array[++Queue->rear]=bk;
    }
}

struct buku* pop(struct queueBuku* Queue){
    if (Queue->size!=0)
    {
        Queue->size--;
        return Queue->array[Queue->front++];
    }
    return NULL;
}

//Function Prototype==========================================================================================

struct buku* buatBuku(char judul[],char genre[],char hal[],char penerbit[]);
struct genre* buatGenre(char genre[]);
int stringMatchKMP(char text[],char pattern[]);
struct genre* searchGenre(char genre[]);
void searchBuku(char s[],struct genre* gnr,struct queueBuku* q);
void searchingBuku(char s[]);
void insertGenre(char genre[],struct buku* b);
void insert(char judul[],char genre[],char hal[],char penerbit[]);
int menu();
void tableHeader();
void makeSpace(unsigned n);
void cetakBuku(struct buku* bk);
void cetakBukuGenre(struct genre* gnr);
void cetakSemuaBuku();
void simpan();
void bacaDataSave();
void hapusTranversal(char key[]);
void hapus();
void hapusGenre(struct genre* gnr);

//Main function=========================================================================================

int main()
{
    int menuku;
    char judulInput[20];
    char penerbitInput[20];
    char genreInput[20];
    char kataKunci[20];
    char halInput[20];

    FILE *myFile = fopen("save.bin","rb+");
    if(myFile!=NULL)
    {
        bacaDataSave();
        fclose(myFile);
    }
    

    do
    {
        fflush(stdin);
        system("cls");
        menuku=menu();
        switch (menuku){
        case 1:
            system("cls");
            printf("|-----------------------------------------------------------------------------------|\n");
            printf("|                                     Daftar Buku                                   |\n");
            printf("|-----------------------------------------------------------------------------------|\n");
            cetakSemuaBuku();
            printf("press enter\n");
            break;

        case 2:
            system("cls");
            printf("|-----------------------------------------------------------------------------------|\n");
            printf("|                                     Tambah Buku                                   |\n");
            printf("|-----------------------------------------------------------------------------------|\n");
            char judulInput[20];
            printf("Tambahkan Buku\n");
            printf("Judul :");
            fflush(stdin);
            //gets(judulInput);
            scanf("%s",judulInput);
            fflush(stdin);
            printf("Penerbit :");
            //gets(penerbitInput);
            scanf("%s",penerbitInput);
            fflush(stdin);
            printf("Halaman :");
            //gets(halInput);
            scanf("%s",halInput);
            fflush(stdin);
            printf("Genre :");
            //gets(genreInput);
            scanf("%s",genreInput);
            fflush(stdin);
            insert(judulInput,genreInput,halInput,penerbitInput);
            printf("press enter\n");
            break;
        case 3:
            system("cls");
            printf("|-----------------------------------------------------------------------------------|\n");
            printf("|                                      Cari Buku                                    |\n");
            printf("|-----------------------------------------------------------------------------------|\n");
            printf("Cari Buku\n");
            printf("Katakunci :");
            scanf("%s",kataKunci);

            searchingBuku(kataKunci);
            printf("press enter\n");
            break;

        case 4:
            system("cls");
            printf("|-----------------------------------------------------------------------------------|\n");
            printf("|                            Tampilkan Buku per-genre                               |\n");
            printf("|-----------------------------------------------------------------------------------|\n");
            printf("Cetak Buku berdasarkan Genre\n");
            printf("Genre :");
            scanf("%s",genreInput);
            struct genre* z = searchGenre(genreInput);
            if (z!=NULL)
            {
                tableHeader();
                cetakBukuGenre(z);
            }else
            {
                printf("genre tidak ditemukan\n");
            }
            printf("press enter\n");
            break;

        case 5:
            system("cls");
            printf("|-----------------------------------------------------------------------------------|\n");
            printf("|                                      Hapus Buku                                   |\n");
            printf("|-----------------------------------------------------------------------------------|\n");
            printf("Hapus Buku\n");
            printf("Keyword: ");
            scanf("%s",kataKunci);
            hapusTranversal(kataKunci);
            printf("Press enter\n");
            break;
        }
        
    system("pause");
    } while (menuku!=6);

    simpan(headGenre);
    printf("    [Data Tersimpan]\n");

    return 0;
}

//Function ==========================================================================================

struct buku* buatBuku(char judul[],char genre[],char hal[],char penerbit[]){
    struct buku* z = (struct buku*)malloc(sizeof(struct buku));
    strcpy(z->genre,genre);
    strcpy(z->namaPenerbit,penerbit);
    strcpy(z->judul,judul);
    strcpy(z->hal,hal);
    z->next=NULL;
    z->prev=NULL;
    return z;
}

struct genre* buatGenre(char genre[]){
    struct genre* z = (struct genre*)malloc(sizeof(struct genre));
    strcpy(z->genre,genre);
    z->next=NULL;
    return z;
}

int stringMatchKMP(char text[],char pattern[]){
    int t = strlen(text);
    int p = strlen(pattern);
    int prefix[p];

    prefix[0]= 0;

    for(int i = 0; i < p; i++)//buat prefix table
    {
        for(int j = 0; j < p; j++)
        {
            if((pattern[i]==pattern[j]) && (i<j))
            {
                prefix[j]=i;
            }else if(i<j)
            {
                prefix[j]=0;
            }
            //printf("%d, %d\n",i,j);
        }
    }

    int j=0;
    int flag;

    //membandingkan  string dengan pattern
    char t1,p1;
    for (int i = 0; i < t; i++)
    {
        while (j<p)
        {
            t1 = toupper(text[i]);
            p1 = toupper(pattern[j]);
            if (t1==p1){
                //printf("%d-%d",j,p);
                j++;
                i++;
            }else{
                j=prefix[j];
                break;
            }
        }
        //printf("h%d",j);
        if (j == p)
        {
            return 1;
        }
    }
    return -1;
}

struct genre* searchGenre(char genre[]){
    tempGenre = headGenre;
    while (tempGenre!=NULL)
    {
        if (strcmp(tempGenre->genre,genre)==0)
        {
            return tempGenre;
        }
        tempGenre=tempGenre->next;
    }
    return NULL;
}

void searchBuku(char s[],struct genre* gnr,struct queueBuku* q){
    tempBuku = gnr->head;
    do{
        if(stringMatchKMP(tempBuku->judul,s)==1 || stringMatchKMP(tempBuku->namaPenerbit,s)==1){
            push(q,tempBuku);
        }

        tempBuku=tempBuku->next;

    }while (tempBuku!=NULL);
}

void searchingBuku(char s[]){

    if (headGenre!=NULL)
    {
        struct buku* z=NULL;
        struct queueBuku* qn = buatQueue();
        tempGenre=headGenre;
        tableHeader();
        while (tempGenre!=NULL)
        {
            searchBuku(s,tempGenre,qn);
            
            do{
                tempBuku=pop(qn);
                if (tempBuku!=NULL)
                {
                    cetakBuku(tempBuku);
                    printf("\n");
                }
                
            }while (tempBuku!=NULL);
            
            tempGenre=tempGenre->next;
        }
       
        // free(qn);
    }else
    {
        printf("No Book Available\n");
    }
}

void insertGenre(char genre[],struct buku* b){//masuk ke insert
    struct genre* z=buatGenre(genre);
    z->head=b;
    if (headGenre==NULL){
        headGenre = z;
        tailGenre = z;
    }else{
        tailGenre->next=z;
        tailGenre=z;
    }
}

void insert(char judul[],char genre[],char hal[],char penerbit[]){//fungsi insert general
     struct buku* z=buatBuku(judul,genre,hal,penerbit);
    tempGenre = searchGenre(genre);
    if (tempGenre==NULL)
    {
        insertGenre(genre,z);
    }else
    {
        tempBuku=tempGenre->head;
        while (tempBuku->next!=NULL)
        {
            tempBuku=tempBuku->next;
        }
        z->prev=tempBuku;
        tempBuku->next=z;
    }
}

int menu(){
    int menuku;
    printf("====================================================================================\n");
    printf("||                                                                                ||\n");
    printf("||                                 librar.io                                      ||\n");
    printf("||                                                                                ||\n");
    printf("||                           [1] Tampilkan Daftar Buku                            ||\n");
    printf("||                           [2] Tambahkan  Buku                                  ||\n");
    printf("||                           [3] Cari Buku                                        ||\n");
    printf("||                           [4] Tampilkan Buku Per-Genre                         ||\n");
    printf("||                           [5] Hapus Buku                                       ||\n");
    printf("||                           [6] Exit                                             ||\n");
    printf("||                                                                                ||\n");
    printf("====================================================================================\n");
    printf("\n    Masukkan Menu :");
    scanf("%d",&menuku);
    return menuku;
}

void makeSpace(unsigned n){
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
    printf("|");
}

void tableHeader(){
    printf("|-----------------------------------------------------------------------------------|\n");
    printf("|        Judul       |       Halaman      |      Penerbit      |        Genre       |\n");
    printf("|-----------------------------------------------------------------------------------|\n");
}

void cetakBuku(struct buku* bk){
    printf("|");
    printf("%s",bk->judul);
    makeSpace(20-strlen(bk->judul));
    printf("%s",bk->hal);
    makeSpace(20-strlen(bk->hal));
    printf("%s",bk->namaPenerbit);
    makeSpace(20-strlen(bk->namaPenerbit));
    printf("%s",bk->genre);
    makeSpace(20-strlen(bk->genre));
}

void cetakBukuGenre(struct genre* gnr){
    tempBuku = gnr->head;
    while (tempBuku!=NULL)
    {
        cetakBuku(tempBuku);
        printf("\n");
        tempBuku=tempBuku->next;
    }
}

void cetakSemuaBuku(){
    if (headGenre!=NULL)
    {
        tableHeader();
        tempGenre = headGenre;
        while (tempGenre!=NULL)
        {
            cetakBukuGenre(tempGenre);
            tempGenre=tempGenre->next;
        }

    }else
    {
        printf("buku kosong\n");
    }
}

void simpan(struct genre* gnrhead){
    FILE *myfile;
    myfile = fopen("save.bin","wb");
    if (gnrhead!=NULL)
    {
        tempGenre = gnrhead;
        while (tempGenre!=NULL)
        {
            tempBuku = tempGenre->head;
            while (tempBuku!=NULL)
            {
                fwrite(tempBuku,sizeof(struct buku),ID_DIGITS,myfile);
                tempBuku=tempBuku->next;
            }
            tempGenre=tempGenre->next;
        }
    }
    fclose(myfile);
}

void bacaDataSave(){
    FILE *myfile;
    struct buku z;
    int i =0;
    myfile = fopen("save.bin","rb");
    // unsigned nn = fread(&z,sizeof(struct buku),ID_DIGITS,myfile);
    // printf("hello%d %s %s %s %s",nn,z.judul,z.genre,z.hal,z.namaPenerbit);
    //fseek(myfile,i*sizeof(struct buku),SEEK_SET);
    
        while(1)
        {
            if (fread(&z,sizeof(struct buku),ID_DIGITS,myfile)!=ID_DIGITS)
            {
                break;
            }
            insert(z.judul,z.genre,z.hal,z.namaPenerbit);
            //fseek(myfile,i*sizeof(struct buku),SEEK_CUR);
        }
        
    fclose(myfile);
    
}

void hapusTranversal(char key[]){
    char hps;
    if (headGenre!=NULL)
    {
        struct buku* z;
        struct queueBuku* qn = buatQueue();
        tempGenre=headGenre;
        tableHeader();
        while (tempGenre!=NULL)
        {
            searchBuku(key,tempGenre,qn);
            do{
                z=pop(qn);
                if (z!=NULL)
                {
                    cetakBuku(z);
                    printf("  Hapus ? [Y/N] :");
                    scanf(" %c",&hps);
                    fflush(stdin);
                    if (hps=='Y'||hps=='y')
                    {
                        hapus(z,tempGenre);
                        printf("  Kembali Ke Menu ? [Y/N] :");
                       scanf(" %c",&hps);
                       if ((hps=='Y'||hps=='y'))
                       {
                        return;
                       }
                    }   
                }
                
            }while (z!=NULL);

            tempGenre=tempGenre->next;
        }

        free(qn);
    }else
    {
        printf("Tidak ada buku tersedia\n");
    }
}

void hapus(struct buku* bk,struct genre* gnr){
    if (bk!=NULL)
    {
        if (bk==gnr->head)
        {

            if (bk->next==NULL)
            {
                hapusGenre(gnr);
            }else
            {
                gnr->head=bk->next;
            }
            
            
            free(bk);
        }else if (bk->next==NULL){
            bk->prev->next=NULL;
            free(bk);
        }else
        {
            bk->prev->next=bk->next;
            free(bk);
        }
    }
}

void hapusGenre(struct genre* gnr){
    if (headGenre==gnr)
    {
        headGenre=gnr->next;
    }else if (gnr->next==NULL)
    {
        gnr=NULL;
    }else
    {
        struct genre* t,*p;
        t=headGenre;
        while (t!=NULL)
        {
            p=t;
            t=t->next;
            if (t==gnr)
            {
                p->next=t->next;
            }
            
        }
    }
}
