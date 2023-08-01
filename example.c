#include "foxes.h"
#include <stdio.h>

int main() {
    printf("%s\n", foxes_fox((foxes_options){}));
    printf("%s\n", foxes_fox((foxes_options){.width = 150}));
    printf("%s\n", foxes_fox((foxes_options){.width = 150, .height = 150}));
    printf("%s\n", foxes_fox((foxes_options){.width = 150, .height = 150, .aspectRatio = (char*) "1:2"}));
    return 0;
}
