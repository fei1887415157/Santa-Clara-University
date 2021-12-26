

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "list.h"


int main(int argc, char *argv[])
{
    LIST *lp;

    lp = createList();
    int i;
    int *p;
    for(i = 0; i < 100; i ++){
        addFirst(lp, p);
        printf("count : %d, item = %d\n", numItems(lp), p);
    }
}
