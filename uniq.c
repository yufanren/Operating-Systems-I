#include "types.h"
#include "stat.h"
#include "user.h"

typedef struct laList laList;
laList* incList(laList*, char);
int compareList(laList*, laList*, int);
void delList(laList*);
char* strList(laList* list);
int caseChar(char*, char*);

char buf[512];

void printList(laList* list, int count, int c, int d) {
  char* out = strList(list); 
  if (count > 1 || !d) {
    if (c) printf(1, "\t%d ", count);
    printf(1, "%s", out);
  }
  free(out);
  delList(list);
}

void uniq(int fd, int* conditions) {
  int n, i, c;
  laList *current = 0, *next = 0;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {    
      next = incList(next, buf[i]);
      if (buf[i] == '\n') {
        if (current && compareList(current, next, conditions[0])) {
          delList(next);
          c++;
        }  
        else {
            if (current) printList(current, c, conditions[1], conditions[2]);
            current = next;
            c = 1;   
        }
        next = 0;
      }			 
    }
  }
  printList(current, c, conditions[1], conditions[2]);
  if (n < 0){
    printf(1, "read error\n");
    exit();
  }    
}

int
main(int argc, char *argv[])
{
  int fd, i;
  char* file;
  int conditions[3] = {0};
  if (argc <= 1) {
    uniq(0, conditions);
    exit();
  }

  for (i = 1; i < argc; i++) {
    char* arg = argv[i];
    if (arg[0] == '-') {
      int c = 0;
      while (arg[++c] != '\0') {
        switch (arg[c]) {
          case 'i':
            conditions[0] = 1;
            break;
          case 'c':
            conditions[1] = 1;
            break;
          case 'd':
            conditions[2] = 1;
            break;
          default:
            printf(1, "uniq: invalid option -- '%c'\n", arg[c]);
            exit();
        }
      }
    }
    else file = arg;
  }

  if ((fd = open(file, 0)) < 0) {
      printf(1, "uniq: cannot open %s\n", file);
      exit();    
  }
  uniq(fd, conditions);
  close(fd);
  exit();
}

struct laList {
  char val;
  laList* prev;
  laList* next;
};

laList* incList(laList* list, char value) 
{
  laList* p = (laList*)malloc(sizeof(laList));
  p->val = value;
  p->next = 0;
  if (list) {
    p->prev = list;
    list->next = p;
  }
  else p->prev = 0;
  return p;
}

int compareList(laList* list1, laList* list2, int iCase) 
{
  while (list1 && list2) {
    if (iCase ? !caseChar(&(list1->val), &(list2->val)) : (list1->val != list2->val))
      return 0;
    list1 = list1->prev;
    list2 = list2->prev;
  }
  return (!list1) && (!list2);
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

int caseChar(char* a, char* b) {
  if (*a == *b) return 1;
  else if ((*a - 'a') >= 0 && (*a - 'a') < 26) return (*a - *b) == 32;
  else if ((*a - 'A') >= 0 && (*a - 'A') < 26) return (*b - *a) == 32;
  return 0;
}







