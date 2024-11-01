#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif


//end of inclusions

//start of global variables
#define N 10000//input size limit
char A[N+1]={0};
//end of global variables

//start of declaring functions

int checkexfile(char*);

int checkexdir(char*);

// end of declaring functions

//start of structs
struct linkedlist{
    char *c;
    long long len;
    long long pl;
    int mode;
    struct linkedlist *next;
    struct linkedlist *prev;
};

struct linkedlist* create_linkedlist(){
    struct linkedlist *list=(struct linkedlist *)malloc(sizeof(struct linkedlist));
    list->len=0;
    list->next= NULL;
    list->prev= NULL;
    list->mode=0;
    list->pl=-1;
    return list;
};

void add(struct linkedlist* list, char text[], int len, int pl, int mode){
    while(list->next!=NULL){
        list=list->next;
    }
    list->next=create_linkedlist();
    list->next->c=calloc(len,sizeof(char));
    for(int i=0; i<len; i++){
        *(list->next->c+i)=text[i];
    }
    list->next->prev=list;
    list->next->len=len;
    list->next->pl=pl;
    list->next->mode=mode;
}

void movelist(struct linkedlist *list, int n){
    if(n>0){
        for(int i=0; i<n; i++){
            list=list->next;
        }
    }
    if(n<0){
        for(int i=0; i<-n; i++){
            list=list->prev;
        }
    }
    return;
}

void insertlist(struct linkedlist *list, int n, char text[]){
    movelist(list,n);
    int len=strlen(text);
    struct linkedlist *nex=list->next;
    list->next=create_linkedlist();
    list->next->next=nex;
    list->next->c=calloc(len,sizeof(char));
    for(int i=0; i<len; i++){
        *(list->next->c+i)=text[i];
    }
    list->next->prev=list;
    nex->prev=list->next;
}

void removelist(struct linkedlist *list, int n){
    movelist(list,n);
    list->prev->next=list->next;
    list->next->prev=list->prev;
}

//end of structs


//start of functions

int *pathmaker(char path[], int pathdash[]){
    int count=0;
    for(int i=0; i<N; i++){
        if(path[i]=='/'){
            count+=1;
            pathdash[count]=i;
        }
    }
    int pathlen=count;
    return pathdash;
}

int createsave(char name[]){
    if(checkexfile(name)==1){
        return 0;
    }
    int pathdash[N+1]={0};
    pathmaker(name, pathdash);
    int i=1;
    while(pathdash[i]!=0){
		char cur_path[N]={0};
        strncpy(cur_path,name, pathdash[i]);
        if(checkexdir(cur_path)==0){
            mkdir(cur_path,ACCESSPERMS);
        }
        i++;
    }
    FILE *f = fopen(name, "w");
    fclose(f);
    return 1;
}


//1 mirize to 2
void fill_file(char name1[],char name2[]){
    FILE *file1=fopen(name1, "r");
    FILE *file2=fopen(name2, "w");
    char c=fgetc(file1);
    while(c!=EOF){
        putc(c, file2);
        c=fgetc(file1);
    }
    fclose(file1);
    fclose(file2);
}

char *namesake(char name[],char save[]){
    strcpy(save,name);
    save[2]='s';
    save[3]='a';
    save[4]='v';
    save[5]='e';
    return save;
}

void saver(char name[]){
    char save[N]={0};
    strcpy(save,namesake(name,save));
	createsave(save);
    fill_file(name,save);
    return;
}



void swap(char name1[], char name2[]){
    fill_file(name1,"part1.txt");
    fill_file(name2,name1);
    fill_file("part1.txt",name2);
}

void mem_wordtochar(char name[]){
    FILE *file=fopen(name,"r");
    FILE *fmem=fopen("fmem.txt","r");
    FILE *mem2=fopen("mem2.txt","w");
    long long chartype=0, wcount=0, i=0, word;
    int ex=fscanf(fmem,"%lld", &word);
    char c=fgetc(file);
    while(ex>0){
        i++;
        if((chartype==0)&&(c!='\n')&&(c!=' ')){
            chartype=1;
            wcount+=1;
        }
        else if((chartype==1)&&((c=='\n')||(c==' '))){
            chartype=0;
        }
        if (wcount==word){
            fprintf(mem2,"%lld ", i);
            ex=fscanf(fmem,"%lld", &word);
        }
        c=getc(file);
    }
    fclose(file);
    fclose(fmem);
    fclose(mem2);
    fill_file("mem2.txt","fmem.txt");
    return;
}

void mem_chartoword(char name[]){
    FILE *file=fopen(name,"r");
    FILE *fmem=fopen("fmem.txt","r");
    FILE *mem2=fopen("mem2.txt","w");
    long long chartype=0, wcount=0, i=0, start, end;
    int ex=fscanf(fmem,"%lld %lld", &start, &end);
    char c=fgetc(file);
    while(ex>0){
        i++;
        if((chartype==0)&&(c!='\n')&&(c!=' ')){
            chartype=1;
            wcount+=1;
        }
        else if((chartype==1)&&((c=='\n')||(c==' '))){
            chartype=0;
        }
        if (start==i){
            fprintf(mem2,"%lld %lld\n", wcount, wcount);
            ex=fscanf(fmem,"%lld %lld", &start, &end);
        }
        c=getc(file);
    }
    fclose(file);
    fclose(fmem);
    fclose(mem2);
    fill_file("mem2.txt","fmem.txt");
    return;
}

long long wordtochar(char name[], int word){
    FILE *file=fopen(name,"r");
    int chartype=0, wcount=0, i=0;
    char c=fgetc(file);
    while(c!=EOF){
        i++;
        if((chartype==0)&&(c!='\n')&&(c!=' ')){
            chartype=1;
            wcount+=1;
        }
        else if((chartype==1)&&((c=='\n')||(c==' '))){
            chartype=0;
        }
        if (wcount==word){
            fclose(file);
            return i;
        }
        c=getc(file);
    }
    fclose(file);
    return -1;
}

long long chartoword(char name[], int cn){
    FILE *file=fopen(name,"r");
    int chartype=0, wcount=0, i=0;
    char c=fgetc(file);
    while(c!=EOF){
        i++;
        if((chartype==0)&&(c!='\n')&&(c!=' ')){
            chartype=1;
            wcount+=1;
        }
        else if((chartype==1)&&((c=='\n')||(c==' '))){
            chartype=0;
        }
        if (i==cn){
            fclose(file);
            return wcount;
        }
        c=getc(file);
    }
    fclose(file);
    return -1;
}

long long standardize(char name[], int line, int dep){
    FILE *file=fopen(name,"r");
    int newline_counter=0, lastline=-1, i=0;
    if((line==1)&&(dep==0)){
        fclose(file);
        return 0;
    }
    char c=fgetc(file);
    while(1){
        i++;
        if(newline_counter==line-1){
            lastline++;
        }
        if((newline_counter==line-1)&&(dep==lastline)){
            break;
        }
        if((c==EOF)){
            return -1;
        }
        if(c=='\n'){
            if(newline_counter==line-1){
                return -1;
            }
            newline_counter++;
        }
        c=fgetc(file);
    }
    return i-1;
}

char *fix_name(char name[]){
    char buf[N];
    snprintf(buf, sizeof(buf), ".%s", name);
    strcpy(name, buf);
    return name;
}

int checkexdir(char address[]){
    DIR* dir = opendir(address);
    if (dir) {
	closedir(dir);
        return 1;
    }
    if (ENOENT == errno) {
        return 0;
    }
}

int checkexfile(char address[]){
    FILE *file;
    if((file = fopen(address,"r"))!=NULL){
        fclose(file);
        return 1;
    }
    else {
        return 0;
    }
}

int createfile(char name[]){
	if(checkexfile(name)==1){
		printf("file already exists\n");
		return 0;
	}
	int pathdash[N+1]={0};
	pathmaker(name, pathdash);
	int i=1;
	while(pathdash[i]!=0){
		char cur_path[N]={0};
        strncpy(cur_path,name, pathdash[i]);
        if(checkexdir(cur_path)==0){
            mkdir(cur_path,ACCESSPERMS);
        }
        i++;
    }
    FILE *f = fopen(name, "w");
    fclose(f);
    saver(name);
    return 1;
}

int insertstr(char name[], char text[], long long place){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place==-1){
        printf("no such position\n");
        return 0;
    }
    saver(name);
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    FILE *part2w=fopen("part2.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    for(int i=0; i<place; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    while(c!=EOF){
        c=fgetc(file);
        putc(c, part2w);
    }
    fclose(part1w);
    fclose(part2w);
    fclose(file);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    FILE *part2r=fopen("part2.txt","r");
    FILE *mem=fopen("mem.txt","w");
    fprintf(mem,"%d\n%lld",3,place);
    fclose(mem);
    c=fgetc(part1r);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(part1r);
    }
    for(int i=0; i<strlen(text); i++){
        putc(text[i],filew);
    }
    c=fgetc(part2r);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(part2r);
    }
    fclose(part1r);
    fclose(part2r);
    fclose(filew);
    fill_file("mem.txt","permem.txt");
    return 1;
}

int cat(char name[]){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    FILE *output=fopen("output.txt","w");
    FILE *file=fopen(name,"r");
    char c=fgetc(file);
    while(c!=EOF){
        fprintf(output, "%c", c);
        c=fgetc(file);
    }
    fclose(file);
    fclose(output);
    return 1;
}

//mode ghable vooroodi baresi mishavad va voroodi agar -b bood place-size+1 dadeh mishavad
//baraye removestr copystr cutstr pastestr hanooz anjam nashode
//khar yadet nare
//gav
int removestr(char name[], int place, int size){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place<0){
        printf("no such position\n");
        return 0;
    }
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    FILE *mem=fopen("mem.txt","w");
    fprintf(mem,"%d\n%d %d\n",4,place,size);
    for(int i=0; i<place-1; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    for(int i=0; i<size; i++){
        c=fgetc(file);
        if(c==EOF){
            printf("no such position\n");
            fclose(part1w);
            fclose(file);
            fclose(mem);
            return 0;
        }
        putc(c, mem);
    }
    while((c!=EOF)){
        c=fgetc(file);
        if(c==EOF){
            break;
        }
        putc(c, part1w);
    }
    fclose(part1w);
    fclose(file);
    saver(name);
    fclose(mem);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    c='a';
    while((c!=EOF)){
        c=fgetc(part1r);
        if(c==EOF){
            break;
        }
        putc(c,filew);
    }
    fclose(filew);
    fclose(part1r);
    fill_file("mem.txt","permem.txt");
    return 1;
}

int copystr(char name[], int place, int size){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place<0){
        printf("no such position");
        return 0;
    }
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    FILE *mem=fopen("mem.txt","w");
    FILE *tempboard=fopen("tempboard.txt","w");
    fprintf(mem,"%d\n%d %d\n",5,place,size);
    for(int i=0; i<place-1; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    for(int i=0; i<size; i++){
        c=fgetc(file);
        if(c==EOF){
            printf("no such position");
            fclose(mem);
            fclose(tempboard);
            fclose(file);
            return 0;
        }
        putc(c, mem);
        putc(c, tempboard);
    }
    while((c!=EOF)){
        c=fgetc(file);
        if(c==EOF){
            break;
        }
        putc(c, part1w);
    }
    fclose(part1w);
    fclose(file);
    fclose(mem);
    fclose(tempboard);
    fill_file("tempboard.txt","clipboard.txt");
    fill_file("mem.txt","permem.txt");
    return 1;
}

int cutstr(char name[], int place, int size){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place<0){
        printf("no such position");
        return 0;
    }
    FILE *file=fopen(name,"r");
    FILE *part1w=fopen("part1.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    FILE *mem=fopen("mem.txt","w");
    FILE *tempboard=fopen("tempboard.txt","w");
    fprintf(mem,"%d\n%d %d\n",6,place,size);
    for(int i=0; i<place-1; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    for(int i=0; i<size; i++){
        c=fgetc(file);
        if(c==EOF){
            printf("no such position");
            fclose(mem);
            fclose(tempboard);
            fclose(part1w);
            fclose(file);
            return 0;
        }
        putc(c, mem);
        putc(c, tempboard);
    }
    while((c!=EOF)){
        c=fgetc(file);
        if(c==EOF){
            break;
        }
        putc(c, part1w);
    }
    fclose(part1w);
    fclose(file);
    saver(name);
    fclose(mem);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    c='a';
    while((c!=EOF)){
        c=fgetc(part1r);
        if(c==EOF){
            break;
        }
        putc(c,filew);
    }
    fclose(filew);
    fclose(part1r);
    fclose(tempboard);
    fill_file("tempboard.txt","clipboard.txt");
    fill_file("mem.txt","permem.txt");
    return 1;
}

int pastestr(char name[], int place){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return 0;
    }
    if(place==-1){
        printf("no such position");
        return 0;
    }
    saver(name);
    FILE *file=fopen(name,"r");
    FILE *pst=fopen("clipboard.txt","r");
    FILE *part1w=fopen("part1.txt","w");
    FILE *part2w=fopen("part2.txt","w");
    int newline_counter=0, lastline=-1;
    char c;
    for(int i=0; i<place; i++){
        c=fgetc(file);
        putc(c, part1w);
    }
    while(c!=EOF){
        c=fgetc(file);
        putc(c, part2w);
    }
    fclose(part1w);
    fclose(part2w);
    fclose(file);
    FILE *filew=fopen(name,"w");
    FILE *part1r=fopen("part1.txt","r");
    FILE *part2r=fopen("part2.txt","r");
    FILE *mem=fopen("mem.txt","w");
    fprintf(mem,"%d\n%d",3,place);
    fclose(mem);
    c=fgetc(part1r);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(part1r);
    }
    c=fgetc(pst);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(pst);
    }
    c=fgetc(part2r);
    while(c!=EOF){
        putc(c, filew);
        c=fgetc(part2r);
    }
    fclose(part1r);
    fclose(part2r);
    fclose(filew);
    fclose(pst);
    fill_file("mem.txt","permem.txt");
    return 1;
}

int moder(char str[], int len){
    if(str[0]=='*'){
        return 1;
    }
    if(str[len-1]=='*'){
        int i=1;
        while((str[len-1-i]=='\\')&&(len>=i+1)){
            i++;
        }
        if(i%2==1){
            return 2;
        }
    }
    return 0;
}

char *cleaner(char str[], int len, int mode){
    int n=len-!(mode==0);
    int m=(mode==1);
    char *a=calloc(n,sizeof(char));
    int count=0;
    for(int i=0; i<n; i++){
        if(str[i+m]=='\\'){
            if((i+m<n-1)&&(str[i+m+1]=='*')){
                *(a+count)='*';
                i++;
            }
            else if((i+m<n-1)&&(str[i+m+1]=='\\')){
                *(a+count)='\\';
                i++;
            }
            else if((i+m<n-1)&&(str[i+m+1]=='n')){
                *(a+count)='\n';
                i++;
            }
            else if((i+m<n-1)&&(str[i+m+1]=='\"')){
                *(a+count)='\"';
                i++;
            }
            count++;
        }
        else{
            *(a+count)=str[i+m];
            count++;
        }
    }
    return a;
}

//mode: 0=a, 1=*a, 2=a*
//return: -1=no match, i= match&placein word;

int comparator_start(char a[], char b[], int m){
    long long la=strlen(a), lb=strlen(b);
    if(la>lb){
        return 0;
    }
    if (m==0){
        if (strcmp(a,b)==0){
            return 1;
        }
        return 0;
    }
    if((m==2)||(m==3)){
        for(int i=0; i<la; i++){
            if(a[i]!=b[i]){
                return 0;
            }
        }
        return 1;
    }
    if((m==1)||(m==5)){
        for(int i=0; i<la; i++){
            if(a[la-i-1]!=b[lb-i-1]){
                return 0;
            }
        }
        if(m==1){
            return 1;
        }
        if(m==5){
            return lb-la+1;
        }
    }
    if(m==4){
        int c=0;
        for(int i=0;i<=lb-la; i++){
            c=0;
            for(int j=0; j<la;j++){
                if(a[j]!=b[j+i]){
                    c=1;
                    break;
                }
            }
            if(c==0){
                return i+1;
            }
        }
        return 0;
    }
    if(m==6){
        int c=0;
        for(int i=0;i<=lb-la; i++){
            c=0;
            for(int j=0; j<la;j++){
                if(a[la-1-j]!=b[lb-1-j-i]){
                    c=1;
                    break;
                }
            }
            if(c==0){
                return 1;
            }
        }
        return 0;
    }
}

int comparator_end(char a[], char b[], int m){
    long long la=strlen(a), lb=strlen(b);
    if(la>lb){
        return 0;
    }
    if (m==0){
        if (strcmp(a,b)==0){
            return la;
        }
        return 0;
    }
    if((m==2)||(m==3)){
        for(int i=0; i<la; i++){
            if(a[i]!=b[i]){
                return 0;
            }
        }
        if(m==2){
            return lb;
        }
        if(m==3){
            return la;
        }
    }
    if((m==1)||(m==5)){
        for(int i=0; i<la; i++){
            if(a[la-i-1]!=b[lb-i-1]){
                return 0;
            }
        }
        return lb;
    }
    if(m==4){
        int c=0;
        for(int i=0;i<=lb-la; i++){
            c=0;
            for(int j=0; j<la;j++){
                if(a[j]!=b[j+i]){
                    c=1;
                    break;
                }
            }
            if(c==0){
                return lb;
            }
        }
        return 0;
    }
    if(m==6){
        int c=0;
        for(int i=0;i<=lb-la; i++){
            c=0;
            for(int j=0; j<la;j++){
                if(a[la-1-j]!=b[lb-1-j-i]){
                    c=1;
                    break;
                }
            }
            if(c==0){
                return lb-i;
            }
        }
        return 0;
    }
}

int grepper(char a[],char b[]){
    long long la=strlen(a), lb=strlen(b);
    int c=0;
    for(int i=0;i<=lb-la; i++){
        c=0;
        for(int j=0; j<la;j++){
            if(a[j]!=b[j+i]){
                c=1;
                break;
            }
        }
        if(c==0){
            return 1;
        }
    }
    return 0;

}

void find_single_init(char name[], char text[]){
    FILE *file=fopen(name,"r");
    FILE *mem=fopen("fmem.txt","w");
    struct linkedlist *stream=create_linkedlist();
    char c;
    long long len=strlen(text);
    c=fgetc(file);
    long long f=0;
    while(c!=EOF){
        f++;
        add(stream,&c,1,f,-1);
        c=fgetc(file);
    }
    stream=stream->next;
    for(int i=0; i<=f-len; i++){
        struct linkedlist *a=stream;
        int check=0;
        for(int j=0; j<len; j++){
            if(*(a->c)!=text[j]){
                check=1;
                break;
            }
            a=a->next;
        }
        if(check==0){
            fprintf(mem,"%d %lld\n", i+1, i+len);
        }
        stream=stream->next;
    }
    fclose(file);
    fclose(mem);
    return;
}

void find_lot_init(char name[], char text[]){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return;
    }
    int len=strlen(text);
    FILE *file=fopen(name,"r");
    FILE *help=fopen("part1.txt","w");
    long long chartype=0, wcount=0;
    for(int i=0; i<len; i++){
        if((chartype==0)&&(text[i]!='\n')&&(text[i]!=' ')){
            chartype=1;
            wcount+=1;
        }
        else if((chartype==1)&&((text[i]=='\n')||(text[i]==' '))){
            chartype=0;
        }
        putc(text[i], help);
    }
    struct linkedlist *check=create_linkedlist(), *src=create_linkedlist();
    fclose(help);
    FILE *part1r=fopen("part1.txt","r");
    int fcount=0, ccount=0;
    char a[N]={0};
    int ex=fscanf(part1r, "%s", a);
    int single=0;
    while(ex>0){
        ccount++;
        int mm=moder(a,strlen(a));
        //tartib moheme
        if((wcount==1)&&(mm==0)){
            single=1;
        }
        //printf("%s %d\n", a, mm);
        strcpy(a,cleaner(a,strlen(a),mm));
        //printf("%s %d\n", a, mm);
        if(ccount==1){
            if(mm==0){
                mm=5;
            }
            if(mm==2){
                mm=4;
            }
        }
        if(ccount==wcount){
            if(mm==0){
                mm=3;
            }
            if(mm==1){
                mm=6;
            }
        }
        // !!!
        //printf("%s %d\n", a, mm);
        add(check,a,strlen(a),-1,mm);
        ex=fscanf(part1r, "%s", a);
    }
    if(single==1){
        fclose(part1r);
        fclose(file);
        find_single_init(name,check->next->c);
        return;
    }
    char b[N]={0};
    ex=fscanf(file, "%s", b);
    while(ex>0){
        fcount++;
        strcpy(b,cleaner(b,strlen(b),0));
        add(src,b,strlen(b), fcount, 0);
        ex=fscanf(file, "%s", b);
    }
    fclose(file);
    fclose(help);
    FILE *fmem=fopen("fmem.txt","w");
    src=src->next;
    check=check->next;
    for(int i=0; i<=fcount-ccount; i++){
        int amogus=0;
        struct linkedlist *aaa=check, *bbb=src;
        int khar=0;
        int start,end;
        for(int j=0; j<ccount; j++){
            start=comparator_start(aaa->c,bbb->c,aaa->mode);
            end=comparator_end(aaa->c,bbb->c,aaa->mode);
            if(start==0){
                amogus=1;
                break;
            }
            if (j!=ccount-1){
                aaa=aaa->next;
                bbb=bbb->next;
            }
        }
        if(amogus==0){
            long long pl1=wordtochar(name,src->pl)+comparator_start(check->c,src->c,check->mode)-1;
            long long pl2=wordtochar(name,bbb->pl)+comparator_end(aaa->c,bbb->c,aaa->mode)-1;
            fprintf(fmem,"%lld %lld\n",pl1, pl2);
        }
        src=src->next;
    }
    fclose(fmem);
    return;
}

//mode: 0=aadi, 1=count, 2=at, 3=all
//outtype: 0=char, 1=word
// agar count nabashad, ifcount=0 voroodi migirim

void find_exe(char name[], int mode, int outtype, int ifat){
    FILE *fmem=fopen("fmem.txt","r");
    FILE *output=fopen("output.txt","w");
    if(outtype==1){
        mem_chartoword(name);
    }
    long long start, end, count=0, ex=0;
    ex=fscanf(fmem,"%lld %lld", &start, &end);
    if(ex<=0){
        printf("no such expression\n");
        fclose(fmem);
        return;
    }
    while(ex>0){
        count++;
        if(mode==0){
            fprintf(output, "%lld\n", start);
            fclose(fmem);
            fclose(output);
            return;
        }
        else if(mode==2){
            if(count==ifat){
                fprintf(output, "%lld\n", start);
                fclose(fmem);
                fclose(output);
                return;
            }
        }
        else if(mode==3){
            fprintf(output, "%lld\n", start);
        }
        ex=fscanf(fmem,"%lld %lld", &start, &end);
    }
    fclose(fmem);
    if(mode==2){
        printf("expression appears fewer times");
        fclose(output);
        return;
    }
    if(mode==1){
        fprintf(output, "%lld\n", count);
        fclose(output);
        return;
    }
    fclose(output);
    return;

}


//mode: 0=at, 1=all;
//text1=ooni ke bar midaarim
//text2=ooni ke jash mizarim
void replace_exe(char name[], int mode, int ifat, char new_text[]){
    saver(name);
    FILE *fmem=fopen("fmem.txt","r");
    long long start, end, count=0, shift=0, slide=0, len=strlen(new_text);
    int ex=fscanf(fmem,"%lld %lld", &start, &end);
    if(ex<=0){
        printf("no such expression\n");
        return;
    }
    while(ex>0){
        count++;
        if(mode==0){
            if(count==ifat){
                removestr(name,shift+start, end-start+1);
                insertstr(name,new_text,shift+start-1);
                fclose(fmem);
                return;
            }
        }
        else if(mode==1){
            removestr(name,start+shift,end-start+1);
            insertstr(name,new_text,start+shift-1);
            shift+=-(end-start+1)+len;
        }
        ex=fscanf(fmem,"%lld %lld", &start, &end);
    }
    if(mode==0){
        printf("expression appears fewer times\n");
    }
    fclose(fmem);
    return;
}



// dar voroodi gereftan file ha, esm tamam file ha ro too grepfiles.txt mmizarim
//mode: 0=print all lines, 1=count, 2=print all files
void grep_exe(char pattern[], int mode){
    FILE *names=fopen("grepfiles.txt","r");
    FILE *output=fopen("output.txt","w");
    char name[N], line[N];
    int ex=fscanf(names,"%[^\n]s", name);
    fgetc(names);
    int ex2=1;
    long long count=0;
    while(ex>0){
        int filecheck=0;
        if(checkexfile(name)==0){
            printf("file does not exist\n");
            return;
        }
        FILE *now=fopen(name,"r");
        ex2=fscanf(now,"%[^\n]s", line);
        fgetc(now);
        while(ex2>0){
            if (grepper(pattern, line)){
                count++;
                if(mode==0){
                    fprintf(output, "%s: %s\n", name, line);
                }
                if((mode==2)&&(filecheck==0)){
                    fprintf(output, "%s\n", name);
                    filecheck=1;
                }
            }
            ex2=fscanf(now,"%[^\n]s", line);
            fgetc(now);
        }
        ex=fscanf(names,"%[^\n]s", name);
        fgetc(names);
    }
    if(mode==1){
        fprintf(output, "%lld", count);
    }
    fclose(output);
    return;
}

void closingpairs(char name[]){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return;
    }
    saver(name);
    FILE *file=fopen(name,"r");
    struct linkedlist *stream=create_linkedlist();
    struct linkedlist *putter=stream;
    char c=fgetc(file);
    while(c!=EOF){
        add(stream, &c, 1, 0, 0);
        c=fgetc(file);
    }
    c='\0';
    add(stream, &c, 1, 0, 0);
    fclose(file);
    int depth=0;
    stream=stream->next;
    while(1){
        if(*(stream->c)=='{'){
            depth++;
            while((*(stream->next->c)==' ')||(*(stream->next->c)=='\n')||(*(stream->next->c)=='\t')){
                removelist(stream->next,0);
            }/*
            while((*(stream->prev->c)==' ')||(*(stream->prev->c)=='\n')||(*(stream->prev->c)=='\t')){
                removelist(stream->prev,0);
            }*/
            if(!((*(stream->prev->c)==' ')||(*(stream->prev->c)=='\n')||(*(stream->prev->c)=='\t'))){
                insertlist(stream->prev,0," ");
            }
            if(*(stream->next->c)=='}'){
                insertlist(stream,0,"\n");
                for(int i=0; i<4*depth; i++){
                    insertlist(stream->next,0," ");
                }
                stream=stream->next;
            }
            if(*(stream->next->c)!='\n'){
                insertlist(stream,0,"\n");
            }
        }
        else if(*(stream->c)=='}'){
            depth--;
            while(*(stream->prev->c)==' '){
                removelist(stream->prev,0);
            }
            if(*(stream->prev->c)!='\n'){
                insertlist(stream->prev,0,"\n");
            }
            for(int i=0; i<4*depth; i++){
                insertlist(stream->prev,0," ");
            }
            while(*(stream->next->c)==' '){
                removelist(stream->next,0);
            }
            if(*(stream->next->c)!='\n'){
                insertlist(stream,0,"\n");
            }
        }
        if(*(stream->c)=='\n'){
            while(*(stream->next->c)==' '){
                removelist(stream->next,0);
            }
            for(int i=0; i<4*depth; i++){
                insertlist(stream,0," ");
            }
        }
        if(stream->next==NULL){
            break;
        }
        stream=stream->next;
    }
    FILE *now=fopen(name,"w");
    putter=putter->next;
    while(1){
        fputc(*(putter->c),now);
        if(*(putter->next->c)=='\0'){
            break;
        }
        putter=putter->next;
    }
    fclose(now);
    return;
}

void compare(char name1[], char name2[]){
    if(checkexfile(name1)==0){
        printf("file 1 does not exist\n");
        return;
    }
    if(checkexfile(name2)==0){
        printf("file 2 does not exist\n");
        return;
    }
    FILE *file1=fopen(name1,"r");
    FILE *file2=fopen(name2,"r");
    char c1[N],c2[N];
    int ex1=fscanf(file1,"%[^\n]s", c1);
    int ex2=fscanf(file2,"%[^\n]s", c2);
    fgetc(file1);
    fgetc(file2);
    int c=0,i=0, lock=0;
    while((ex1>0)&&(ex2>0)){
        i++;
        c=2*(ex1<=0)+1*(ex2<=0);
        if(strcmp(c1,c2)!=0){
            printf("========= #%d =========\n", i);
            printf("%s\n", c1);
            printf("%s\n", c2);
        }
        ex1=fscanf(file1,"%[^\n]s", c1);
        ex2=fscanf(file2,"%[^\n]s", c2);
        fgetc(file1);
        fgetc(file2);
    }
    int st=i+1;
    if(ex1>0){
        FILE *help=fopen("part1.txt", "w");
        while(ex1>0){
            i++;
            fprintf(help,"%s\n", c1);
            ex1=fscanf(file1,"%[^\n]s", c1);
            fgetc(file1);
        }
        fclose(help);
        FILE *spill=fopen("part1.txt", "r");
        printf("<<<<<<< #%d-#%d <<<<<<<\n", st, i);
        ex1=fscanf(spill,"%[^\n]s", c1);
        fgetc(spill);
        while(ex1>0){
            i++;
            printf("%s\n", c1);
            ex1=fscanf(spill,"%[^\n]s", c1);
            fgetc(spill);
        }
        fclose(spill);
    }
    if(ex2>0){
        FILE *help=fopen("part1.txt", "w");
        while(ex2>0){
            i++;
            fprintf(help,"%s\n", c2);
            ex2=fscanf(file1,"%[^\n]s", c2);
            fgetc(file1);
        }
        fclose(help);
        FILE *spill=fopen("part1.txt", "r");
        printf(">>>>>>> #%d-#%d >>>>>>>\n", st, i);
        ex2=fscanf(spill,"%[^\n]s", c2);
        fgetc(spill);
        while(ex2>0){
            i++;
            printf("%s\n", c2);
            ex2=fscanf(spill,"%[^\n]s", c2);
            fgetc(spill);
        }
        fclose(spill);
    }
}

//ghablesh output ro khali bayad kard
void tree(char *basePath, const int root, int depth){
    FILE *output=fopen("output.txt","a");
    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir){
        return;
    }
    while ((dp = readdir(dir)) != NULL){
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            for (i=0; i<root; i++){
                if (i%2 == 0 || i == 0){
                    printf("%c", 179);
                }
                else{
                    printf(" ");
                }
            }
            printf("%c%c%s\n", 195, 196, dp->d_name);
            if((depth==-1)||(depth>=root/2+1)){
                strcpy(path, basePath);
                strcat(path, "/");
                strcat(path, dp->d_name);
                tree(path, root + 2, depth);
            }
        }
    }
    fclose(output);
    closedir(dir);
}

void undo(char name[]){
    if(checkexfile(name)==0){
        printf("file does not exist\n");
        return;
    }
    char save[N]={0};
    swap(name, namesake(name,save));
    return;
}

char waste(){
    char c=getchar();
    while((c==' ')){
        c=getchar();
    }
    return c;
}

char *getstr(char output[]){
    strcpy(output,A);
    int i=-1, mode=0;
    char c,b=waste();
    if(b=='\n'){
        char ll[1]={0};
        strcpy(output,ll);
        return output;
    }
    if(b!='\"'){
        i++;
        output[i]=b;
        mode=1;
    }
    while(1){
        i++;
        c=b;
        b=getchar();
        if(((b==' ')||(b=='\n')||(b==EOF))&&(mode==1)){
            break;
        }
        if((mode==0)&&(b=='\"')){
            break;
        }
        if(b=='\\'){
            c=b;
            b=getchar();
            if(b==' '){
                break;
            }
            if(b=='n'){
                output[i]='\n';
            }
            else if(b=='\\'){
                output[i]='\\';
            }
            else if(b=='\"'){
                output[i]='\"';
            }
            else{
                output[i]=b;
            }
        }
        else{
            output[i]=b;
        }

    }
    if(mode==0){
        getchar();
    }
    return output;
}

char *getstr2(){
    char *output=calloc(N,sizeof(char));
    strcpy(output,A);
    int i=-1, mode=0;
    char c,b=waste();
    if(b=='\n'){
        char ll[1]={0};
        strcpy(output,ll);
        return output;
    }
    if(b!='\"'){
        i++;
        *(output+i)=b;
        mode=1;
    }
    while(1){
        i++;
        c=b;
        b=getchar();
        if(((b==' ')||(b=='\n')||(b==EOF))&&(mode==1)){
            break;
        }
        if((mode==0)&&(b=='\"')){
            break;
        }
        if(b=='\\'){
            c=b;
            b=getchar();
            if(b==' '){
                break;
            }
            if(b=='n'){
                *(output+i)='\n';
            }
            else if(b=='\\'){
                *(output+i)='\\';
            }
            else if(b=='\"'){
                *(output+i)='\"';
            }
            else{
                *(output+i)=b;
            }
        }
        else{
            *(output+i)=b;
        }

    }
    if(mode==0){
        getchar();
    }
    return output;
}

char *getstr_4f(char output[]){
    int i=-1, mode=0;
    char c,b=waste();
    if(b!='\"'){
        i++;
        output[i]=b;
        mode=1;
    }
    while(1){
        i++;
        c=b;
        b=getchar();
        if(((b==' ')||(b=='\n')||(b==EOF))&&(mode==1)){
            break;
        }
        if((mode==0)&&(b=='\"')){
            break;
        }
        if(b=='\\'){
            output[i]=b;
            i++;
            c=b;
            b=getchar();
            if((b==' ')&&(mode==0)){
                break;
            }
            output[i]=b;
        }
        else{
            output[i]=b;
        }

    }
    return output;
}


void output_printer(){
    FILE *output=fopen("output.txt","r");
    char c=fgetc(output);
    while(c!=EOF){
        printf("%c", c);
        c=fgetc(output);
    }
    fclose(output);
    return;
}

void nchar(int n){
    for(int i=0; i<n; i++){
        printf("%c", getchar());
    }
}

// end of functions


int main(){
    printf("Powered by <<Bario>>\n");
    printf("This program is made for the windows operating system\nplease test on windows\n\n");
    char a[100]="amo\nrmio";
    //scanf("%s", a);
    char b[]="./root/amo.txt";
    char c[]="./root/sina.txt";
    char s[]="a* *b *c";
    char t[]="khar yes";
    char input[N]={0};
    int arman=0;
    while(1){
        printf(">>>>>");
        scanf("%s", input);
        if(strcmp("exit",input)==0){
            printf("\n======= Goodbye =======");
            break;
        }
        else if(strcmp("createfile",input)==0){
            char *fname=calloc(N,sizeof(char));
            getstr(fname);
            createfile(fname);
        }
        else if(strcmp("insertstr",input)==0){
            char *fname=calloc(N,sizeof(char));
            char *str=calloc(N,sizeof(char));
            int line=0, dep=0;
            getstr(fname);
            getstr(str);
            scanf("%d:%d", &line, &dep);
            long long pl=standardize(fname,line,dep);
            insertstr(fname,str,pl);
        }
        else if(strcmp("cat",input)==0){
            char *fname=calloc(N,sizeof(char));
            getstr(fname);
            cat(fname);
            output_printer();
            printf("\n");
        }
        else if(strcmp("removestr",input)==0){
            char *fname=calloc(N,sizeof(char));
            int line=0, dep=0, size, mode=0;
            char flag;
            getstr(fname);
            scanf("%d:%d", &line, &dep);
            scanf("%d", &size);
            waste();
            flag=getchar();
            long long pl=standardize(fname,line,dep);
            if(flag=='b'){
                pl=pl-size+1;
            }
            removestr(fname,pl,size);
        }
        else if(strcmp("copystr",input)==0){
            char *fname=calloc(N,sizeof(char));
            int line=0, dep=0, size, mode=0;
            char flag;
            getstr(fname);
            scanf("%d:%d", &line, &dep);
            scanf("%d", &size);
            waste();
            flag=getchar();
            long long pl=standardize(fname,line,dep);
            if(flag=='b'){
                pl=pl-size+1;
            }
            copystr(fname,pl,size);
        }
        else if(strcmp("cutstr",input)==0){
            char *fname=calloc(N,sizeof(char));
            int line=0, dep=0, size, mode=0;
            char flag;
            getstr(fname);
            scanf("%d:%d", &line, &dep);
            scanf("%d", &size);
            waste();
            flag=getchar();
            long long pl=standardize(fname,line,dep);
            if(flag=='b'){
                pl=pl-size+1;
            }
            cutstr(fname,pl,size);
        }
        else if(strcmp("pastestr",input)==0){
            char *fname=calloc(N,sizeof(char));
            int line=0, dep=0;
            getstr(fname);
            scanf("%d:%d", &line, &dep);
            long long pl=standardize(fname,line,dep);
            pastestr(fname,pl);
        }
        else if(strcmp("find",input)==0){
            char *fname=calloc(N,sizeof(char));
            char *str=calloc(N,sizeof(char));
            int mode=0, at=0, outtype=0, g=0;
            getstr(fname);
            getstr_4f(str);
            char c=waste();
            if(c=='\n'){
                g=1;
            }
            char *flag=calloc(6,sizeof(char));
            if((c=='-')&&(g==0)){
                scanf("%s", flag);
                if(strcmp(flag,"count")==0){
                    mode=1;
                }
                if(strcmp(flag,"at")==0){
                    mode=2;
                    scanf("%d", &at);
                }
                if(strcmp(flag,"all")==0){
                    mode=3;
                }
                if(strcmp(flag,"byword")==0){
                    outtype=1;
                }
            }
            if(g==0){
                c=waste();
            }
            char *outt=calloc(9,sizeof(char));
            if((c=='-')&&(g==0)){
                scanf("%s", outt);
                if(strcmp(outt,"byword")==0){
                    outtype=1;
                }
            }
            find_lot_init(fname,str);
            find_exe(fname,mode,outtype,at);
            output_printer();
        }
        else if(strcmp("replace",input)==0){
            char *fname=calloc(N,sizeof(char));
            char *str1=calloc(N,sizeof(char));
            char *str2=calloc(N,sizeof(char));
            int mode=0, at=0, outtype=0, g=0;
            getstr(fname);
            getstr_4f(str1);
            getstr(str2);
            char c=waste();
            if(c=='\n'){
                g=1;
            }
            char *flag=calloc(6,sizeof(char));
            if((c=='-')&&(g==0)){
                scanf("%s", flag);
                if(strcmp(flag,"at")==0){
                    mode=0;
                    scanf("%d", &at);
                }
                if(strcmp(flag,"all")==0){
                    mode=1;
                }
            }
            find_lot_init(fname,str1);
            replace_exe(fname,mode,at,str2);
        }
        else if((strcmp("grep",input)==0)){
            char *pattern=calloc(N,sizeof(char));
            char *str=calloc(N,sizeof(char));
            FILE *grepfiles=fopen("grepfiles.txt","w");
            int mode=0;
            getstr(pattern);
            getchar();
            char c=getchar();
            if(c=='r'){mode=0;}
            if(c=='c'){mode=1;}
            if(c=='l'){mode=2;}
            str=getstr2();
            while(*str!=0){
                fprintf(grepfiles, "%s\n", str);
                strcpy(str,A);
                str=getstr2();
            }
            fclose(grepfiles);
            grep_exe(pattern,mode);
            output_printer();
        }
        else if((strcmp("undo",input)==0)){
            char *fname=calloc(N,sizeof(char));
            getstr(fname);
            undo(fname);
        }
        else if((strcmp("auto-indent",input)==0)){
            char *fname=calloc(N,sizeof(char));
            getstr(fname);
            closingpairs(fname);
        }
        else if((strcmp("compare",input)==0)){
            char *fname1=calloc(N,sizeof(char));
            char *fname2=calloc(N,sizeof(char));
            getstr(fname1);
            getstr(fname2);
            compare(fname1,fname2);
        }
        else if((strcmp("tree",input)==0)){
            int depth;
            scanf("%d", &depth);
            FILE *f=fopen("output.txt","w");
            fclose(f);
            tree("./root/", 0, depth);
            //output_printer();
        }
        else{
            while(getchar()!='\n');
            printf("no such function\n");
        }
    }
}

