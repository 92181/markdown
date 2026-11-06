#include <stdlib.h>
#include <unistd.h>

typedef struct md
{
  char *b;
  char *b_pos;
  char *b_end;
} md_store;

// Guarantee Memory;
static inline int pmo(md_store *d, long extra_len)
{
  if (d->b_end - d->b_pos < extra_len)
  {
    long q = d->b_pos - d->b, c = (d->b_end - d->b) + extra_len + 1024;

    char *u = realloc(d->b, c);

    if (u == 0)
    {
      free(d->b);

      return 1;
    };

    d->b = u;
    d->b_pos = d->b + q;
    d->b_end = d->b + c;
  }

  return 0;
}

// Write To Memory Location;
static inline void stw(md_store *d, char *source, long len)
{
  if (pmo(d, len))
  {
    return;
  }

  __builtin_memcpy(d->b_pos, source, len);

  d->b_pos += len;
}

// Convert Integer To String (Max: 10 Characters);
static inline unsigned int its(md_store *d, unsigned int u)
{
  unsigned int t = u, n = 0;

  while (u > 0)
  {
    u /= 10;
    n += 1;
  };

  if (pmo(d, n))
  {
    return 0;
  }

  char *r = d->b_pos + n - 1;

  while (t > 0)
  {
    *r = '0' + t % 10;
    t /= 10;
    r -= 1;
  };

  return n;
}

// Handle Text Emphasis;
static inline void sit(md_store *d, char **b, char *end, char c, unsigned int *s, unsigned int *x)
{
  if (*b + 1 >= end)
  {
    *b += 1;
    return;
  }

  *b += 1;

  if (**b == c)
  {
    // Handle Strong;
    if (*s)
    {
      *s = 0;
      char *i = "</strong>";
      stw(d, i, 9);
      if (*b < end)
      {
        *b += 1;
      }
    }
    else
    {
      *s = 1;
      char *i = "<strong>";
      stw(d, i, 8);
      if (*b < end)
      {
        *b += 1;
      }
    }
  }
  else
  {
    // Handle Italic;
    if (*x)
    {
      *x = 0;
      char *i = "</em>";
      stw(d, i, 5);
    }
    else
    {
      *x = 1;
      char *i = "<em>";
      stw(d, i, 4);
    }
  }
}

// Handle Lists;
static inline void lis(char i, char *sn, unsigned int *x, unsigned int **j, unsigned int *w, unsigned int *c, md_store *d, char *b)
{
  // Get Offset;
  unsigned *m = *j;
  *x = b - sn;

  // Terminate Menu;
  while (*j > c && *x < *(*j - 1))
  {
    char *e = "</ul>", *k = "</ol>", *n = "</blockquote>";
    *j -= 2;

    // Get Closing Type;
    if (**j == 0)
    {
      stw(d, e, 5);
    }
    else if (**j == 1)
    {
      stw(d, k, 5);
    }
    else
    {
      stw(d, n, 13);
    };
  };

  m = *j; // update m, needed!

  // New Menu On Offset Change;
  if (*j == c || *x > *(m - 1))
  {
    // Save Type And Offset To Buffer;
    if (*j < w)
    {
      *m = i;
      *(m + 1) = *x;
      *j += 2;
    };

    if (i == 0)
    {
      char *e = "<ul>";
      stw(d, e, 4);
    }
    else if (i == 1)
    {
      char *e = "<ol>";
      stw(d, e, 4);
    };
  };

  char *k = "<li>";
  stw(d, k, 4);
};

// Remove Spaces In Front Of (Fake) New Line;
static inline void lds(char **b, char **o, char *end)
{
  // Remove Dead Spaces;
  while (*b < end && **b == ' ')
  {
    *b += 1;
  };

  // Save Pos;
  *o = *b;
};

// Parse Markdown;
extern inline char *parse_markdown(unsigned int *u, char *b, unsigned int s)
{
  // Define State Variables;
  char *q = b, *a = b + s, *o = b, *sn = b, hx[] = "<hx>", *ho = hx + 2, hd[] = "</hx>";

  unsigned int x = 10000;

  // Simulated New Line & Blockquote Variables;
  unsigned int v = 0, sw = 0, st = 0, sm = 0, sx = 0, sc = 0, sg = 0;

  // Allocate Buffer Memory (Input Size * Highest Set Size);
  long cap = (long)s * 4;

  if (cap < 1)
  {
    cap = 1;
  }

  char *t = malloc(cap), *y = b + s;

  if (t == 0)
  {
    return 0;
  }

  md_store d = {.b = t, .b_pos = t, .b_end = t + cap};

  // List State Buffer;
  unsigned int *c = malloc(256 * sizeof(unsigned int)), *j = c, *w = c + 256;

  if (c == 0)
  {
    free(t);
    return 0;
  };

  *(c + 1) = 0;

  // Table Variables;
  char sa[] = "<tr><th>", sq[] = "</th></tr>", sz[] = "</th><th>";
  unsigned int sl = 0, sj = 0;

  // Code, Link, Checkbox & Image Variables;
  unsigned int sv = 0;
  char *su = 0;

  // Loop Over Data;
  while (b < y)
  {
    if (d.b_end - d.b_pos < 64)
    {
      pmo(&d, 64);
    }

    switch (*b)
    {
    case '#':
    {
      // Check For New Line;
      if (b - o == 0)
      {
        // Get Header Type;
        char *n = b;

        while (b < a && *b == '#')
        {
          b += 1;
        };

        // Write Header And Save It;
        *ho = '0' + (b - n);
        stw(&d, hx, 4);
      }
      else
      {
        // Pass-through;
        *d.b_pos = *b;
        b += 1;
        d.b_pos += 1;
      };

      break;
    };
    case '_':
    {
      // Handle Text Emphasis;
      sit(&d, &b, a, '_', &sx, &sc);

      break;
    }
    case '~':
    {
      if (b + 1 < a && *(b + 1) == '~')
      {
        // Handle Text Strike-Through;
        if (st)
        {
          st = 0;
          char *i = "</del>";
          stw(&d, i, 6);
        }
        else if (b + 2 < a && *(b + 2) > ' ')
        {
          st = 1;
          char *i = "<del>";
          stw(&d, i, 5);
        };

        b += 2;
      }
      else
      {
        // Handle Subscript;
        if (sm)
        {
          sm = 0;
          char *i = "</sub>";
          stw(&d, i, 6);
        }
        else if (b + 1 < a && *(b + 1) > ' ')
        {
          sm = 1;
          char *i = "<sub>";
          stw(&d, i, 5);
        }
        else
        {
          *d.b_pos = *b;
          d.b_pos += 1;
        }

        b += 1;
      }

      break;
    }
    case '^':
    {
      // Handle SuperScript;
      if (sm)
      {
        sm = 0;
        char *i = "</sup>";
        stw(&d, i, 6);
      }
      else
      {
        sm = 1;
        char *i = "<sup>";
        stw(&d, i, 5);
      };

      b += 1;
      break;
    }
    case '.':
    case ')':
    {
      // Get Numbers In Front;
      char *n = b - 1;

      while (n > q && *n >= '0' && *n <= '9')
      {
        n -= 1;
      };

      n += 1;

      // Check For Number And New Line;
      if (n - o == 0 && b != n)
      {
        d.b_pos -= b - n;

        lis(1, sn, &x, &j, w, c, &d, b);

        // Handle New Line;
        b += 2;
        o = b;
      }
      else
      {
        // Pass-through;
        *d.b_pos = *b;
        b += 1;
        d.b_pos += 1;
      };

      break;
    };
    case '*':
    {
      if (b + 1 < a && *(b + 1) == '*')
      {
        // Handle Text Emphasis;
        sit(&d, &b, a, '*', &sx, &sc);
      }
      else if (b + 1 < a && *(b + 1) == ' ' && b - o == 0)
      {
        // Handle Unordered Lists;
        lis(0, sn, &x, &j, w, c, &d, b);

        // Handle New Line;
        b += 2;
        o = b;
      }
      else
      {
        // Handle Text Emphasis;
        sit(&d, &b, a, '*', &sx, &sc);
      };

      break;
    };
    case '>':
    {
      // Check For New Line;
      if (b - o == 0)
      {
        char *n = b, *i = "<blockquote>", *h = "</blockquote>";

        // Save Start Offset;
        if (v == 0)
        {
          sw = b - sn;
        };

        x = b - sn;

        // Get Consecutive Quote Amount;
        while (b < a && *b == '>')
        {
          b += 1;
        };

        unsigned int u = b - n;

        // Write;
        if (v > u)
        {
          // terminate ul, ol & block here too!
          while (j > c && x <= *(j - 1))
          {
            char *e = "</ul>", *k = "</ol>";
            j -= 2;

            // Get Closing Type;
            if (*j == 0)
            {
              stw(&d, e, 5);
            }
            else if (*j == 1)
            {
              stw(&d, k, 5);
            }
            else
            {
              stw(&d, h, 13);

              v -= 1;
              break;
            };
          };
        };

        while (v < u && j < w)
        {
          // Add To List Array;
          *j = 2;
          *(j + 1) = x;
          j += 2;

          stw(&d, i, 12);
          v += 1;
        };

        // Handle Offset Spaces;
        lds(&b, &o, a);
      }
      else
      {
        // Pass-through;
        *d.b_pos = *b;
        b += 1;
        d.b_pos += 1;
      };

      break;
    };
    case '\r':
    {
      b += 1;
      break;
    }
    case '\n':
    {
      // Handle Offset Spaces;
      b += 1;
      sn = b;
      lds(&b, &o, a);

      // Close Header Tag;
      if (*ho != 'x')
      {
        // Get Type And Write;
        *(hd + 3) = *ho;
        stw(&d, hd, 5);
        *ho = 'x';
      };

      // Close Blockquote;
      char *e = "</ul>", *k = "</ol>", *h = "</blockquote>";

      // Terminate Lists And Blockquotes;
      if (j > c)
      {
        unsigned int o = 100000;
        char *n, *t;

        // Handle Blockquote;
        if (v)
        {
          n = sn + sw;

          // If First Blockquote Is Not There;
          if (n >= a || *n != '>')
          {
            o = sw;
          };
        };

        // Handle List;
        n = b;

        if ((long)x + 1 > a - n)
        {
          t = a;
        }
        else
        {
          t = n + x + 1;
        };

        // Last Offset Reach Check;
        while (n < t && *n >= ' ')
        {
          n += 1;
        };

        // If Not Reached Get Cutoff;
        if (n != t)
        {
          if (n - b < o)
          {
            o = n - b;
          };
        };

        if (o != 100000)
        {
          while (j > c && *(j - 1) >= o)
          {
            j -= 2;

            // Get Closing Type;
            if (*j == 0)
            {
              stw(&d, e, 5);
            }
            else if (*j == 1)
            {
              stw(&d, k, 5);
            }
            else
            {
              stw(&d, h, 13);
              v -= 1;
            };
          };
        };
      };

      // Break-Line;
      *d.b_pos = ' ';
      d.b_pos += 1;

      break;
    };
    case '|':
    {
      // Check For Active Table;
      if (sl > 0)
      {
        if (sl == 2)
        {
          unsigned int u = 0, aq = 0;

          // Change Row Type;
          *(sa + 6) = *(sq + 3) = *(sz + 3) = *(sz + 7) = 'd';

          // Write Style Type;
          char *i = "<style>";
          stw(&d, i, 7);

          // Detect Alignement;
          while (b < a && *b >= ' ')
          {
            if (*b == ':')
            {
              if (b + 1 < a && *(b + 1) == '-')
              {
                u += 1;
              }
              else
              {
                if (u > 0)
                {
                  u = 3;
                }
                else
                {
                  u = 2;
                };
              };
            }
            else if (*b == '|')
            {
              // Write To Table Style;
              if (u > 1)
              {
                // Handle Process;
                char *h = "#t", *z = ">td:nth-child(", *k = "){text-align:", *e = "center;}", *l = "right;}";

                stw(&d, h, 2);
                d.b_pos += its(&d, sg);

                // Write Integer And Remaining Text;
                stw(&d, z, 14);
                d.b_pos += its(&d, aq);
                stw(&d, k, 13);

                // Write Alignement (Center, Right);
                if (u == 3)
                {
                  stw(&d, e, 8);
                }
                else
                {
                  stw(&d, l, 7);
                };
              };

              u = 0;
              aq += 1;
            };

            b += 1;
          };

          char *ii = "</style>";
          stw(&d, ii, 8);

          sl += 1;
        }
        else
        {
          // Line Start;
          if (b - o == sj)
          {
            stw(&d, sa, 8);
            b += 1;
          }
          else if (b + 1 < a && *(b + 1) < ' ')
          {
            // Handle Table Line End;
            stw(&d, sq, 10);
            b += 1;

            // Handle CRLF;
            while (b < a && *b <= ' ')
            {
              b += 1;
            };

            b += sj;

            // Check For Termination;
            if (b >= a || *b != '|')
            {
              char *i = "</table>\n\n";
              stw(&d, i, 10);
            };

            // Count Table Lines;
            sl += 1;
            o = b;
          }
          else
          {
            // In-between Table Line;
            stw(&d, sz, 9);
            b += 1;
          };
        };
      }
      else
      {
        // Check For Line Start, Otherwise Pass Through;
        if (b - o == 0)
        {
          char i[256] = "<table id='t", *k = "'>";
          sl = 1;
          sg += 1;

          // Write ID;
          stw(&d, i, 12);
          d.b_pos += its(&d, sg);
          stw(&d, k, 2);
          stw(&d, sa, 8);

          // Save Start Offset;
          sj = b - o;
        }
        else
        {
          *d.b_pos = *b;
          d.b_pos += 1;
        };

        b += 1;
      };

      break;
    };
    case '[':
    {
      // Handle Image, Checkbox Or Link;
      if (b - 1 > q && *(b - 1) == '!')
      {
        d.b_pos -= 1;
        b += 1;

        char *i = "<img alt='", *y = "' src='", *g = "'>";
        stw(&d, i, 10);

        // Pass-Through Alt Text;
        while (b < a && *b != ']')
        {
          if (pmo(&d, 12))
          {
            return 0;
          };

          *d.b_pos = *b;
          d.b_pos += 1;
          b += 1;
        };

        stw(&d, y, 7);
        b += 2;

        // Write Image Source;
        while (b < a && *b != ')')
        {
          if (pmo(&d, 8))
          {
            return 0;
          };

          *d.b_pos = *b;
          d.b_pos += 1;
          b += 1;
        };

        stw(&d, g, 2);
      }
      else if (b + 3 < a && *(b + 2) == ']' && *(b + 3) != '(')
      {
        // Write Checkbox;
        char *i = "<input disabled type='checkbox'", *y = " checked>";
        stw(&d, i, 31);

        // Check If Active;
        if (*(b + 1) == 'x')
        {
          stw(&d, y, 9);
        }
        else
        {
          *d.b_pos = '>';
          d.b_pos += 1;
        };

        b += 2;
      }
      else
      {
        char *n = b;

        while (n + 2 < a && *n >= ' ')
        {
          if (*n == ']' && *(n + 1) == '(')
          {
            // Start Link;
            char i[] = "<a href=\"", *h = "\">";
            stw(&d, i, sizeof(i) - 1);
            n += 2;

            // Write Link Source;
            char *r = n;

            while (n < a && *n != ')')
            {
              n += 1;
            };

            stw(&d, r, n - r);

            // Save Link Size And Write;
            su = n;
            stw(&d, h, 2);
          };

          n += 1;
        };
      };

      b += 1;
      break;
    }
    case ']':
    {
      // Link Content Ended;
      if (su != 0)
      {
        char *i = "</a>";
        b = su;
        su = 0;
        stw(&d, i, 4);
      }
      else
      {
        // Pass-Through;
        *d.b_pos = ']';
        b += 1;
        d.b_pos += 1;
      };

      b += 1;
      break;
    }
    case '`':
    {
      // Handle Memory;
      char *u = "</code>";

      // Handle Code Block;
      if (b + 3 < a && *(b + 1) == '`' && *(b + 2) == '`')
      {
        char *i = "<code language=\"";
        b += 3;
        stw(&d, i, 16);

        // Pass-Through Type;
        while (b < a && *b >= ' ')
        {
          if (pmo(&d, 12))
          {
            return 0;
          };

          *d.b_pos = *b;
          d.b_pos += 1;
          b += 1;
        };

        *d.b_pos = '"';
        *(d.b_pos + 1) = '>';
        d.b_pos += 2;

        // Pass-Through Code;
        while (b < a && *b <= ' ')
        {
          b += 1;
        };

        while (b < a && *b != '`')
        {
          if (pmo(&d, 12))
          {
            return 0;
          };

          *d.b_pos = *b;
          d.b_pos += 1;
          b += 1;
        };

        // Write End Tag;
        stw(&d, u, 7);
        b += 3;
      }
      else
      {
        // Single Line;
        if (sv == 0)
        {
          char *i = "<code>";
          stw(&d, i, 6);
          sv = 1;
        }
        else
        {
          stw(&d, u, 7);
          sv = 0;
        }

        b += 1;
      };

      break;
    }
    case '\\':
    {
      // Escape Characters;
      b += 1;
      if (b < a)
      {
        char i = *b, *r = b;

        while (b < a && *b == i)
        {
          b += 1;
        }

        stw(&d, r, b - r);
      }
      break;
    }
    default:
    {
      // Pass-through;
      *d.b_pos = *b;
      d.b_pos += 1;
      b += 1;
    }
    };
  };

  // Free Memory And Get Size;
  free(c);
  *u = (d.b_pos - d.b);

  // Null-Terminate;
  if (pmo(&d, 1))
  {
    return 0;
  }

  *d.b_pos = 0;
  d.b_pos += 1;

  return d.b;
}