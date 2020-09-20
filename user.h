struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(char*, int);
int mknod(char*, short, short);
int unlink(char*);
int fstat(int fd, struct stat*);
int link(char*, char*);
int mkdir(char*);
int chdir(char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);

// ulib.c
int stat(char*, struct stat*);	//returns file attributes about a inode(file or directory.)
char* strcpy(char*, char*);	//copy 2nd string to first.
void *memmove(void*, void*, int);	//copy first n char from 2nd to 1st array, replacing.
char* strchr(const char*, char c);	//return pointer to 1st char c in string.
int strcmp(const char*, const char*);	//compare 2 strings, return 0 if same.
void printf(int, char*, ...);	//print to file descriptor(1st arg) all following strings.
char* gets(char*, int max);	//read max char from stdin and place into pointer array, form a string.
uint strlen(char*);	//return string length.
void* memset(void*, int, uint);	//set first n places to value uint.
void* malloc(uint);
void free(void*);
int atoi(const char*);	//convert number string to int.
