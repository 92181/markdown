#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../md.c"

struct stat d;

// Main Logic;
int main(int e,char **h)
{
  // Check For Valid Usage;
  if(e<2)
  {
    char i[]="Correct Usage: ./main page.md\n";write(1,i,sizeof(i));

    return 0;
  };

  // Load Markup File;
  unsigned int f=open(*(h+1),0),u,s,l;

  if(f>>31)
  {
    return 1;
  };

  fstat(f,&d);s=d.st_size;
  char *b=malloc(s),*r;
  
  read(f,b,s);

  // Parse File And Write Result To File;
  r=psr(&u,b,s);l=open("index.html",O_CREAT|O_TRUNC|O_WRONLY,420);

  if(l>>31)
  {
    return 1;
  };

  write(l,r,u);

  // Return Success;
  return 0;

  // Free Memory;
  close(f);close(l);free(b);
};
