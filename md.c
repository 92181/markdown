#include <stdlib.h>
#include <unistd.h>

#include <stdio.h> // debugging only

// Write String To Memory Location;
static inline void stw(char *o,char *s,char **f)
{
  while(o<s)
  {
    **f=*o;o+=1;*f+=1;
  };
};

// Prevent Memory Overflow;
static inline void pmo(char **f,char **e,char **t,unsigned int s)
{
  if(*f+s>*e)
  {
    unsigned int q=*f-*t,c=(*e-*t)+1024*1024;

    *t=realloc(*t,c);*f=*t+q;*e=*t+c;
  };
};

// Convert Integer To String (Max: 10 Characters);
static inline unsigned int its(unsigned int u,char *f)
{
  unsigned int t=u,d=0;

  while(u>0)
  {
    u/=10;d+=1;
  };

  char *r=f+d-1;

  while(t>0)
  {
    *r='0'+t%10;t/=10;r-=1;
  };

  return d;
};

// Handle Text Emphasis;
static inline void sit(char **f,char **b,char c,unsigned int *d,unsigned int *x)
{
  *b+=1;

  if(**b==c)
  {
    // Handle Strong;
    if(*d)
    {
      *d=0;char *i="</strong>";stw(i,i+9,f);*b+=1;
    }
    else
    {
      *d=1;char *i="<strong>";stw(i,i+8,f);*b+=1;
    };
  }
  else
  {
    // Handle Italic;
    if(*x)
    {
      *x=0;char *i="</em>";stw(i,i+5,f);
    }
    else
    {
      *x=1;char *i="<em>";stw(i,i+4,f);
    };
  };
};

// Handle Lists;
//static inline void lis(char i,char *sn,int *x,int **j,int *w,int *c,char **f,char *b)
static inline void lis(char i,char *sn,unsigned int *x,unsigned int **j,unsigned int *w,unsigned int *c,char **f,char *b)
{
  // Get Offset;
  unsigned *m=*j;*x=b-sn;

  // Handle Special Case (Other Type, Same Offset);
  if(*(*j-2)!=i&&*(*j-1)==*x)
  {
    *(*j-1)=100000;
  }

  // Terminate Menu;
  while(*j>c&&*x<*(*j-1))
  {
    char *e="</ul>",*k="</ol>";*j-=2;

    // Get Closing Type;
    if(**j)
    {
      stw(k,k+5,f);
    }
    else
    {
      stw(e,e+5,f);
    };

    printf("Shift down, Offset: %u, Type: %u\n",*x,**j);
  };
  
  m=*j; // update m, needed!

  // New Menu On Offset Change;
  if(*j==c||*x>*(m-1))
  {
    // Save Type And Offset To Buffer;
    if(*j<w)
    {
      //*m=*(i+1)-'o'==0; // OLD
      *m=i; // NEW;
      *(m+1)=*x;
      
      *j+=2;
    };

    printf("New Menu, Offset: %u, Type: %u\n",*x,*m);

    //stw(i,i+4,f); // OLD

    if(i==0) // NEW;
    {
      char *e="<ul>";stw(e,e+4,f);
    }
    else if(i==1)
    {
      char *e="<ol>";stw(e,e+4,f);
    }
    else
    {
      char *e="<blockquote>";stw(e,e+12,f);
    };
  };

  char *k="<li>";stw(k,k+4,f);
};

// Remove Spaces In Front Of (Fake) New Line;
static inline void lds(char **b,char **sn,char **o)
{
  // Remove Dead Spaces;
  while(**b==' ')
  {
    *b+=1;
  };

  // Save Pos;
  *o=*b;
};


// First FIX lists!!!!
// Termination types are wrong

// Remove pmo in some cases?, mem * 100, only check for inf;

// Then publish!
// Later WASM!

// Parse Markdown;
extern inline char *psr(char **r,unsigned int *u,char *b,unsigned int s) // Remove r later!
{
  // Define State Variables;
  char *q=b,*a=b+s,*m=0,*o=b,hx[]="<hx>",*ho=hx+2,hd[]="</hx>";

  unsigned int z=0,x=10000,g=0;

  // Simulated New Line & Blockquote Variables;
  unsigned int v=0,sw=0;

  char *sn=b; // new!

  // Allocate Buffer Memory;
  char *t=malloc(5),*e=t+5,*f=t,*y=b+s; // was 1024*1024 (5 for testing only!)



  
  unsigned int st=0,sm=0,sx=0,sc=0,sg=0;

  // List State Buffer;
  unsigned int *c=malloc(256*sizeof(unsigned int)),*j=c,*w=c+256;*(c+1)=0;

  // Table Variables;
  char sa[]="<tr><th>",sq[]="</th></tr>",sz[]="</th><th>";unsigned int sl=0,sj=0;

  // Code, Link, Checkbox & Image Variables;
  unsigned int sv=0;char *su=0;

  // Loop Over Data;
  while(b<y)
  {
    switch(*b)
    {
      case '#':
      {
        // Check For New Line;
        if(b-o==0)
        {
          // Prevent Memory Overflow;
          pmo(&f,&e,&t,4);

          // Get Header Type;
          char *n=b;

          while(b<a&&*b=='#')
          {
            b+=1;
          };

          // Write Header And Save It;
          *ho='0'+(b-n);stw(hx,hx+4,&f);
        }
        else
        {
          // Pass-through;
          *f=*b;f+=1;
        };

        b+=1;break;
      };
      case '_':
      {
        // Prevent Memory Overflow;
        pmo(&f,&e,&t,14);

        // Handle Text Emphasis;
        sit(&f,&b,'_',&sx,&sc);

        break;
      }
      case '~':
      {
        // Prevent Memory Overflow;
        pmo(&f,&e,&t,6);

        if(*(b+1)=='~')
        {
          // Handle Text Strike-Through;
          if(st)
          {
            st=0;char *i="</del>";stw(i,i+6,&f);
          }
          else if(*(b+2)>' ')
          {
            st=1;char *i="<del>";stw(i,i+5,&f);
          };

          b+=2;
        }
        else
        {
          // Handle Subscript;
          if(sm)
          {
            sm=0;char *i="</sub>";stw(i,i+6,&f);
          }
          else if(*(b+1)>' ')
          {
            sm=1;char *i="<sub>";stw(i,i+5,&f);
          };

          b+=1;
        };

        break;
      }
      case '^':
      {
        // Prevent Memory Overflow;
        pmo(&f,&e,&t,6);

        // Handle Text Strike-Through;
        if(sm)
        {
          sm=0;char *i="</sup>";stw(i,i+6,&f);
        }
        else
        {
          sm=1;char *i="<sup>";stw(i,i+5,&f);
        };

        b+=1;break;
      }
      case '.':
      case ')':
      {
        // Prevent Memory Overflow;
        pmo(&f,&e,&t,1);

        // Get Numbers In Front;
        char *n=b-1;

        while(n>q&&*n>='0'&&*n<='9')
        {
          n-=1;
        };

        n+=1;

        // Check For Number And New Line;
        if(n-o==0&&b!=n)
        {
          f-=b-n;

          //printf("ORDER CALL!\n");
          lis(1,sn,&x,&j,w,c,&f,b);

          // Handle New Line;
          b+=2;o=b;
        }
        else
        {
          // Pass-through;
          *f=*b;b+=1;f+=1;
        };
        
        break;
      };
      case '*':
      {
        // Prevent Memory Overflow;
        pmo(&f,&e,&t,14);

        if(*(b+1)=='*')
        {
          // Handle Text Emphasis;
          sit(&f,&b,'*',&sx,&sc);
        }
        else if(*(b+1)==' '&&b-o==0)
        {
          // Handle Unordered Lists;
          lis(0,sn,&x,&j,w,c,&f,b);

          // We can not mix them BY Default.. KEEP buffer of SPACES for each NEW LEVEL;
          // TYPE:LINE_SPACES,TYPE:LINE_SPACES,TYPE:LINE_SPACES (+=2); 

          // Get Offset;
          /*x=b-sn;

          // New Menu On Offset Change;
          if(j==c||x>*(j-1))
          {
            printf("New Menu, Offset: %u\n",x);

            // Add to list state buffer;
            if(j<w)
            {
              *j=0;*(j+1)=x;j+=2;
            };

            char *i="<ul>";stw(i,i+4,&f);
          }
          else if(x<*(j-1))
          {
            // Close List;
            while(j>c&&x<*(j-1))
            {
              char *i="</ul>";stw(i,i+5,&f);j-=2;

              printf("Shift down!\n");
            };
          };

          char *i="<li>";stw(i,i+4,&f);*/

          // Handle New Line;
          b+=2;o=b;
        }
        else 
        {
          // Handle Text Emphasis;
          sit(&f,&b,'*',&sx,&sc);
        };

        break;
      };
      case '>':
      {
        // Prevent Memory Overflow;
        pmo(&f,&e,&t,13);

        // Check For New Line;
        if(b-o==0)
        {
          char *n=b;//,*i="<blockquote>",*h="</blockquote>";
          
          // Save Start Offset;
          /*if(v==0)
          {
            sw=b-sn;
          };*/

          // Get Consecutive Quote Amount;
          while(b<a&&*b=='>')
          {
            b+=1;
          };

          unsigned int u=b-n;


          //lis(2,sn,&x,&j,w,c,&f,b);
          
          // Write;
          /*while(v<u)
          {
            stw(i,i+12,&f);v+=1;
          };

          while(v>u)
          {
            stw(h,h+13,&f);v-=1;
          };*/

          // Handle Offset Spaces;
          lds(&b,&sn,&o);
        }
        else
        {
          // Pass-through;
          *f=*b;b+=1;f+=1;
        };

        break;
      };
      case '\r':
      {
        *f=*b;f++; // Line For Testing Only;
        b+=1;break;
      }
      case '\n':
      {
        // Handle Offset Spaces;
        b+=1;sn=b;lds(&b,&sn,&o);

        // Prevent Memory Overflow;
        pmo(&f,&e,&t,13);

        // Close Header Tag;
        if(*ho!='x')
        {
          // Get Type And Write;
          *(hd+3)=*ho;stw(hd,hd+5,&f);

          *f='\n';f+=1;*ho='x';
        };

        // Close Blockquote;
        if(v)
        {
          // Calculate Position;
          char *n=sn+sw,*y=n,*h="</blockquote>";

          // Get Quote Amount;
          while(n<a&&*n=='>')
          {
            n+=1;
          };

          unsigned int u=n-y;
          
          while(v>u)
          {
            stw(h,h+13,&f);v-=1;
          };
        };

        // Blockquote and Menu Terminate interfere!! (Only on LAST \n!!!)
        // Can terminate in wrong order!

        // merge them!!! use j for blockquotes! (nested support)

        // Close List;
        if(j>c)
        {
          char *n=b+1,*t=n+x+2,*i="</ul>",*k="</ol>";

          // Skip Over Offset;
          while(n<t&&*n>=' ')
          {
            n+=1;
          };

          // Check Success;
          if(n!=t)
          {
            // Terminate;
            while(j>c)
            {
              j-=2;x=10000;

              if(*j)
              {
                stw(k,k+5,&f);
              }
              else
              {
                stw(i,i+5,&f);
              };
            };

            printf("Terminated List!\n\n");
          };
        };

        // Break-Line If Enabled;
        if(0)
        {
          char *i="<br>";stw(i,i+4,&f);
        };

        break;
      };
      case '|':
      {
        // Prevent Memory Overflow;
        pmo(&f,&e,&t,512);

        // Check For Active Table;
        if(sl>0)
        {
          // Handle Table Alignement;
          if(sl==2)
          {
            unsigned int u=0,aq=0;

            // Change Row Type;
            *(sa+6)=*(sq+3)=*(sz+3)=*(sz+7)='d';

            // Write Style Type;
            char *i="<style>";stw(i,i+7,&f);

            // Detect Alignement;
            while(*b>=' ')
            {
              if(*b==':')
              {
                if(*(b+1)=='-')
                {
                  u+=1;
                }
                else
                {
                  if(u>0){u=3;}else{u=2;};
                };
              }
              else if(*b=='|') 
              {
                // Write To Table Style;
                if(u>1)
                {
                  // Prevent Memory Overflow;
                  pmo(&f,&e,&t,512);

                  // Handle Process;
                  char *h="#t",*z=">td:nth-child(",*k="){text-align:",*e="center;}",*l="right;}";

                  stw(h,h+2,&f);f+=its(sg,f);

                  // Write Integer And Remaining Text;
                  stw(z,z+14,&f);f+=its(aq,f);stw(k,k+13,&f);
                  
                  // Write Alignement (Center, Right);
                  if(u==3)
                  {
                    stw(e,e+8,&f);
                  }
                  else
                  {
                    stw(l,l+7,&f);
                  };
                };

                u=0;aq+=1;
              };

              b+=1;
            };

            char *ii="</style>";stw(ii,ii+8,&f);

            sl+=1;
          }
          else
          {
            // DETECT TABLE LINE START
            if(sj==b-o)
            {
              stw(sa,sa+8,&f);
            }
            else if(*(b+1)<' ')
            {
              // Handle Table Line End;
              stw(sq,sq+10,&f);

              b+=1;
              
              // account for \r\n or \n
              if(b+1<a&&*(b+1)<' ')
              {
                b+=1;
              };

              // Do NOT EXIT HERE! check for '|' AT OFFSET(sj)!

              b+=sj;

              if(b<a&&*b=='|')
              {

              }
              else
              {
                // table end
                char *i="</table>\n\n";stw(i,i+10,&f);
              };

              // Count line ends! in sl!!!!
              sl+=1;
            }
            else
            {
              // Must be inbetween (normal);
              stw(sz,sz+9,&f);
            };
          };
        }
        else
        {
          // Check For Line Start, Otherwise Pass Through;
          if(b-o==0)
          {
            char i[256]="<table id='t",*k="'>";sl=1;sg+=1;

            // Write ID;
            stw(i,i+12,&f);f+=its(sg,f);
            
            stw(k,k+2,&f);stw(sa,sa+8,&f);
            
            // Save Start Offset;
            sj=b-o;
          }
          else
          {
            *f=*b;f+=1;
          };
        };

        b+=1;break;
      };
      case '[':
      {
        // Prevent Memory Overflow;
        pmo(&f,&e,&t,512);

        // Handle Image, Checkbox Or Link;
        if(b-1>q&&*(b-1)=='!')
        {
          f-=1;b+=1;
  
          char *i="<img alt='",*y="' src='",*g="'>";stw(i,i+10,&f);

          // Pass-Through Alt Text;
          while(b<a&&*b!=']')
          {
            pmo(&f,&e,&t,10);*f=*b;f+=1;b+=1;        
          };

          stw(y,y+7,&f);b+=2;

          // Write Image Source;
          while(b<a&&*b!=')')
          {
            pmo(&f,&e,&t,5);*f=*b;f+=1;b+=1;  
          };

          stw(g,g+2,&f);
        }
        else if(b+3<a&&*(b+2)==']'&&*(b+3)!='(')
        {
          // Write Checkbox;
          char *i="<input disabled type='checkbox'",*y=" checked>";stw(i,i+31,&f);

          // Check If Active;
          if(*(b+1)=='x')
          {
            stw(y,y+9,&f);
          }
          else
          {
            *f='>';f+=1;
          };

          b+=2;
        }
        else
        {
          char *n=b;

          while(n+2<a&&*n>=' ')
          {
            if(*n==']'&&*(n+1)=='(')
            {
              // Start Link;
              char *i="<a src=\"",*h="\">";stw(i,i+8,&f);n+=2;

              // Write Link Source;
              while(n<a&&*n!=')')
              {
                *f=*n;f+=1;
                
                n+=1; 
              };

              // Save Link Size And Write;
              su=n;stw(h,h+2,&f);
            };

            n+=1;
          };
        };

        b+=1; // new;
        break;
      }
      case ']':
      {
        // Handle Memory;
        pmo(&f,&e,&t,4);

        // Link Content Ended;
        if(su!=0)
        {
          char *i="</a>";b=su;su=0;stw(i,i+4,&f);
        }
        else
        {
          // Pass-Through;
          *f=']';b+=1;f+=1;
        };

        b+=1; // new;
        break;
      }
      case '`':
      {
        // Handle Memory;
        char *u="</code>";pmo(&f,&e,&t,128);

        // Handle Code Block;
        if(b+3<a&&*(b+2)=='`')
        {
          char *i="<code language=\"";b+=3;stw(i,i+16,&f);

          // Pass-Through Type;
          while(b<a&&*b>=' ')
          {
            pmo(&f,&e,&t,10);*f=*b;f+=1;b+=1;        
          };

          *f='"';*(f+1)='>';f+=2;

          // Pass-Through Code;
          while(b<a&&*b!='`')
          {
            pmo(&f,&e,&t,10);*f=*b;f+=1;b+=1;        
          };

          // Write End Tag;
          stw(u,u+7,&f);b+=2;
        }
        else
        {
          // Single Line;
          if(sv==0)
          {
            char *i="<code>";stw(i,i+6,&f);sv=1;
          }
          else
          {
            stw(u,u+7,&f);sv=0;
          };
        };

        b+=1; // new;
        break;
      }
      case '\\':
      {
        // Escape Characters;
        b+=1;char i=*b;

        while(*b==i)
        {
          b+=1;
        };

        break;
      };
      default:
      {
        pmo(&f,&e,&t,1);

        //printf("%c\n",*b);

        // Pass-through;
        *f=*b;f+=1;
        
        b+=1; // moved
      };
    };

    //b+=1;
  };

  // Set Return Pointers;
  *u=(f-t);*r=t;

  return *r; // For now;
};
