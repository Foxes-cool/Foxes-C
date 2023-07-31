#ifdef __cplusplus
extern "C" {
#endif

#ifndef FOXES_H_
#define FOXES_H_

typedef struct foxes_options{
    int width;
    int height;
    char* aspectRatio;
} foxes_options;

char* foxes_fox(foxes_options options);
char* foxes_curious(foxes_options options);
char* foxes_happy(foxes_options options);
char* foxes_scary(foxes_options options);
char* foxes_sleeping(foxes_options options);

#endif

#ifdef __cplusplus
}
#endif

