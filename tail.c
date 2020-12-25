#include "types.h"
#include "stat.h"
#include "user.h"

int isNumeric(char* str);
int toInt(char* str);
char* expandStr(char* line, int counter, int* size);

char buf[512];

void tail(int fd, int nline)
{  
    if (nline == 0) exit();
    int i, n, lp, cp;
    int arrSize = 128;
    i = n = lp = cp = 0;
    char** lines = (char**)malloc(sizeof(char*) * nline);
    for (i = 0; i < nline; i++) lines[i] = 0;
    char* line = (char*)malloc(sizeof(char) * arrSize);
    
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (i = 0; i < n; i++) {
            line[cp] = buf[i];
            if (buf[i] == '\n') {
                line[cp+1] = '\0';
                if (lines[lp % nline]) free(lines[lp % nline]);
                lines[lp++ % nline] = line;
                arrSize = 128;
                line = (char*)malloc(sizeof(char) * arrSize);   
                cp = 0; 
                continue;  
            }
            else if (cp == arrSize - 2) {
                line = expandStr(line, cp, &arrSize);
            }
            cp++;               
        }
    }
    free(line);
    if (lp < nline) {
        for (i = 0; i < lp; i++) printf(1, "%s", lines[i]);
        for (i = 0; i < lp; i++) free(lines[i]);
    }
    else {
        
        for (i = lp; i < lp + nline; i++) {
            printf(1, "%s", lines[i % nline]); 
        } 
        for (i = 0; i < nline; i++) free(lines[i]);
    }
    
}

int main(int argc, char *argv[]) 
{
    int fd, i, nline = 10;
    char* file = 0;
    for (i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (arg[0] == '-') {
            if (isNumeric(&(arg[1]))) {
                nline = toInt(&(arg[1]));
            }
            else {
                printf(1, "tail: invalid option -- '%c'\n", arg[1]);
                exit();
            }
        }
        else file = argv[i];       
    }
    if (!file) {
        tail(0, nline);
        exit();
    }
    if ((fd = open(file, 0)) < 0) {
        printf(1, "tail: cannot open %s\n", file);
        exit();    
    } 
    tail(fd, nline);
    close(fd);
    exit(); 
}

char* expandStr(char* line, int counter, int* size) 
{
    *size *= 2;
    char* new = malloc(sizeof(char) * (*size));
    int i;
    for (i = 0; i <= counter; i++) {
        new[i] = line[i];
    }
    free(line);
    return new;
}

int isNumeric(char* str)
{
    while (*str != '\0') {
        if (*str > '9' || *str < '0') return 0;
        str++;
    }
    return 1;
} 

int toInt(char* str)
{
    int result = 0;
    while (*str != '\0') {
        result = result * 10 + (*str - '0');
        str++;      
    }
    return result;   	
}

//printf(1, "i is %d! position is %d!\n", i, i % nline); 


