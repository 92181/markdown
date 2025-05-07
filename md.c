#include <stdlib.h>
#include <unistd.h>

// Remove pmo in some cases?, mem * 100, only check for inf;

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
static inline void lis(char i,char *sn,unsigned int *x,unsigned int **j,unsigned int *w,unsigned int *c,char **f,char *b)
{
  // Get Offset;
  unsigned *m=*j;*x=b-sn;

  // Terminate Menu;
  while(*j>c&&*x<*(*j-1))
  {
    char *e="</ul>",*k="</ol>",*n="</blockquote>";*j-=2;

    // Get Closing Type;
    if(**j==0)
    {
      stw(e,e+5,f);
    }
    else if(**j==1)
    {
      stw(k,k+5,f);
    }
    else
    {
      stw(n,n+13,f);
    };
  };
  
  m=*j; // update m, needed!

  // New Menu On Offset Change;
  if(*j==c||*x>*(m-1))
  {
    // Save Type And Offset To Buffer;
    if(*j<w)
    {
      *m=i;*(m+1)=*x;*j+=2;
    };

    if(i==0)
    {
      char *e="<ul>";stw(e,e+4,f);
    }
    else if(i==1)
    {
      char *e="<ol>";stw(e,e+4,f);
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

// Parse Markdown;
extern inline char *psr(unsigned int *u,char *b,unsigned int s)
{
  // Define State Variables;
  char *q=b,*a=b+s,*m=0,*o=b,hx[]="<hx>",*ho=hx+2,hd[]="</hx>";

  unsigned int z=0,x=10000,g=0;

  // Simulated New Line & Blockquote Variables;
  unsigned int v=0,sw=0;

  char *sn=b; // new!

  // Allocate Buffer Memory;
  char *t=malloc(1024*1024),*e=t+5,*f=t,*y=b+s;

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
          *f=*b;b+=1;f+=1;
        };

        break;
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
          char *n=b,*i="<blockquote>",*h="</blockquote>";
          
          // Save Start Offset;
          if(v==0)
          {
            sw=b-sn;
          };

          x=b-sn;

          // Get Consecutive Quote Amount;
          while(b<a&&*b=='>')
          {
            b+=1;
          };

          unsigned int u=b-n;

          // Write;
          if(v>u)
          {
            // printf("Block Shiftdown!\n");

            // terminate ul, ol & block here too!
            while(j>c&&x<=*(j-1))
            {
              char *e="</ul>",*k="</ol>",*n="</blockquote>";j-=2;

              // Get Closing Type;
              if(*j==0)
              {
                stw(e,e+5,&f);
              }
              else if(*j==1)
              {
                stw(k,k+5,&f);
              }
              else
              {
                stw(h,h+13,&f);

                v-=1;break;
              };
            };
          };

          while(v<u&&j<w)
          {
            // add to j!
            *j=2;*(j+1)=x;j+=2;

            stw(i,i+12,&f);v+=1;
          };

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
        b+=1;break;
      }
      case '\n':
      {
        // Handle Offset Spaces;
        b+=1;sn=b;lds(&b,&sn,&o);

        // Prevent Memory Overflow;
        pmo(&f,&e,&t,20);

        // Close Header Tag;
        if(*ho!='x')
        {
          // Get Type And Write;
          *(hd+3)=*ho;stw(hd,hd+5,&f);*ho='x';
        };

        // Close Blockquote;
        char *e="</ul>",*k="</ol>",*h="</blockquote>";

        // Terminate Lists And Blockquotes;
        if(j>c)
        {
          unsigned int o=100000;char *n,*t;

          // Handle Blockquote;
          if(v)
          {
            n=sn+sw;

            // If First Blockquote Is Not There;
            if(n>a||*n!='>')
            {
              o=sw;
            };
          };

          // Handle List;
          n=b;t=n+x+1;

          // Last Offset Reach Check;
          while(n<t&&*n>=' ')
          {
            n+=1;
          };

          // If Not Reached Get Termination Cutoff (New);
          if(n!=t)
          {
            if(n-b<o)
            {
              o=n-b;
            };
          };

          if(o!=100000)
          {
            while(j>c&&*(j-1)>=o)
            {
              j-=2;

              // Get Closing Type;
              if(*j==0)
              {
                stw(e,e+5,&f);
              }
              else if(*j==1)
              {
                stw(k,k+5,&f);
              }
              else
              {
                stw(h,h+13,&f);v-=1;
              };
            };
          };
        };

        // Break-Line If Enabled;
        if(0)
        {
          char *i="<br>";stw(i,i+4,&f);
        }
        else
        {
          //*f=' ';f+=1;
          *f='\n';f+=1;
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
            if(b-o==sj)
            {
              stw(sa,sa+8,&f);
              b+=1;
            }
            else if(*(b+1)<' ')
            {
              // Handle Table Line End;
              stw(sq,sq+10,&f);b+=1;

              // Handle CRLF;
              while(b<a&&*b<=' ')
              {
                b+=1;
              };

              b+=sj;

              // Check For Termination;
              if(b>a||*b!='|')
              {
                char *i="</table>\n\n";stw(i,i+10,&f);
              };

              // Count Table Lines;
              sl+=1;o=b;
            }
            else
            {
              // In-between Table Line;
              stw(sz,sz+9,&f);b+=1;
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
            stw(i,i+12,&f);f+=its(sg,f);stw(k,k+2,&f);stw(sa,sa+8,&f);
            
            // Save Start Offset;
            sj=b-o;
          }
          else
          {
            *f=*b;f+=1;
          };

          b+=1;
        };

        break;
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
        if(b+3<a&&*(b+1)=='`'&&*(b+2)=='`')
        {
          char *i="<code language=\"";b+=3;stw(i,i+16,&f);

          // Pass-Through Type;
          while(b<a&&*b>=' ')
          {
            pmo(&f,&e,&t,10);*f=*b;f+=1;b+=1;        
          };

          *f='"';*(f+1)='>';f+=2;

          // Pass-Through Code;
          while(b<a&&*b<=' ')
          {
            b+=1;
          };

          while(b<a&&*b!='`')
          {
            pmo(&f,&e,&t,10);*f=*b;f+=1;b+=1;        
          };

          // Write End Tag;
          stw(u,u+7,&f);b+=3;
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

          b+=1;
        };

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
        // Pass-through;
        pmo(&f,&e,&t,1);

        *f=*b;f+=1;b+=1;
      };
    };
  };

  // Free Memory And Get Size;
  free(c);*u=(f-t);

  // Null-Terminate;
  pmo(&f,&e,&t,1);*f=0;f+=1;

  // Return;
  return t;
};
