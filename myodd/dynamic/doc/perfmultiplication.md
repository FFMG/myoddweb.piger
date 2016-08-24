## Introduction

Those are the loops we used to time how much slower myodd::dynamic::Any() is compared to 'normal' multiplication.

### Standard loop

    #include <iostream>
    #include <time.h>
    
    int main() {
      clock_t t = clock();
      long long int i = 0;
      long long int c = 0;
      for (i = 0; i<1000000000; i++)
      {
      	c = c*1;
      }
      t = clock() - t;
      printf("It took me %d clicks (%f seconds)", t, ((float)t)/CLOCKS_PER_SEC );
    
      return 0;
    }

### myodd::dynamic::Any loop

    #include <iostream>
    #include <time.h>
    #include "dynamic/any.h"
    
    int main() {
      clock_t t = clock();
      long long int i = 0;
      myodd::dynamic::Any c = 0;
      for (i = 0; i<1000000000; i++)
      {
      	c = c*1;
      }
      t = clock() - t;
      printf("It took me %d clicks (%f seconds)", t, ((float)t)/CLOCKS_PER_SEC );
    
      return 0;
    }
