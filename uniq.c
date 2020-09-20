#include "types.h"
#include "stat.h"
#include "user.h"

typedef struct laList laList;
laList* incList(laList*, char);
int compareList(laList*, laList*);
void delList(laList*);
char* strList(laList* list);

char buf[512];

void uniq(int fd) {
  int n, i;
  laList *current = 0, *next = 0;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {    
      next = incList(next, buf[i]);
      if (buf[i] == '\n') {
        if (current) {
          if (compareList(current, next)) delList(next);
          else {
            char* out = strList(next); 
            printf(1, "%s", out);
            free(out);
            delList(current);
            current = next;  
          }
        }
        else {
          current = next;
          char* out = strList(current);
          //printf(1,"made a array\n");
          printf(1, "%s", out);
          //printf(1,"printed array\n");
          free(out);
          //printf(1, "freed array\n");
        }
        next = 0;
      }			 
    }
  }
  if (n < 0){
    printf(1, "read error\n");
    exit();
  }    
}

int
main(int argc, char *argv[])
{
  int fd;
  if (argc <= 1) {
  uniq(0);
  exit();
  }

  if ((fd = open(argv[1], 0)) < 0) {
      printf(1, "uniq: cannot open %s\n", argv[1]);
      exit();    
  }
  uniq(fd);
  close(fd);
/*
  int fd, i;

  if(argc <= 1){
    cat(0);
    exit();
  }

  for(i = 1; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
    cat(fd);
    close(fd);
  }
*/
  exit();
}

struct laList {
  char val;
  laList* prev;
  laList* next;
};

laList* incList(laList* list, char value) 
{
  laList* p = (laList*)malloc(sizeof(struct laList));
  p->val = value;
  p->next = 0;
  if (list) {
    p->prev = list;
    list->next = p;
  }
  else p->prev = 0;
  return p;
}

int compareList(laList* list1, laList* list2) 
{
  while (list1 && list2) {
    if (list1->val != list2->val)
      return 0;
    list1 = list1->prev;
    list2 = list2->prev;
  }
  return ((!list1) && (!list2)) ? 1 : 0;
}

void delList(laList* list) 
{
  laList* temp;
  while (list) {
    temp = list->prev;
    free(list);
    list = temp; 
  }  
}

char* strList(laList* list)
{
  int size = 1, i;
  while (list->prev) {
    list = list->prev;
    size++;
  }	
  char* Arr = (char*)malloc(sizeof(char) * (size + 1));
  for (i = 0; i < size; i++) {
    Arr[i] = list->val;
    list = list->next;
  }
  Arr[size] = '\0';
  return Arr;
}







