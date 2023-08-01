#include "foxes.h"

#include <time.h>
#include <stdio.h>
#include <curl/curl.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum Tags {
    Fox,
    Curious,
    Happy,
    Scary,
    Sleeping,
} Tags;

typedef struct Counts {
    long count;
    long days;
} Counts;

Counts totals[(Tags)Sleeping+1];

struct CurlString {
  char *memory;
  size_t size;
};

char* concat(int count, ...) {
    long length = 1;
    va_list args;

    va_start(args, count);
    for (int i = 0; i < count; i++) {
        length+=strlen(va_arg(args, char*));
    }
    va_end(args);

    char* final = (char*) malloc(length);

    va_start(args, count);
    strcpy(final, va_arg(args, char*));
    for (int i = 1; i < count; i++) {
        strcat(final, va_arg(args, char*));
    }
    va_end(args);

    return final;
}

char* iota(long value) {
    char buffer[ 64 ] = { 0 };
    snprintf(buffer, sizeof(buffer), "%li", value);
    return strdup(buffer);
}

size_t writefunc(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct CurlString *mem = (struct CurlString *)userp;

    char *ptr = (char*) realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) {
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char* internal(char* name, Tags tag, foxes_options options) {
    long days = time(NULL)/86400;
    if (totals[tag].days != days) {
        CURL *curl = curl_easy_init();

        if(curl) {
            struct CurlString body;

            body.memory = (char*) malloc(1);
            body.size = 0;

            char* url = concat(2, "https://foxes.cool/counts/", name);

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
            curl_easy_perform(curl);

            free(url);
            totals[tag] = (Counts){
                .count = atol(body.memory),
                .days = days,
            };
            free(body.memory);
        }
    }
    srand(time(NULL));
    long id = rand()%totals[tag].count;
    char* url;

    char* params[3] = {(char*) "", (char*) "", (char*) ""};

    int index = 0;
    if (options.width) params[index++] = concat(2, "width=", iota(options.width));
    if (options.height) params[index++] = concat(2, "height=", iota(options.height));
    if (options.aspectRatio) params[index++] = concat(2, "aspect_ratio=", options.aspectRatio);

    if (index > 0) {
        url = concat(10, "https://img.foxes.cool/", name, "/", iota(id), ".jpg?", params[0], "&", params[1], "&", params[2]);
        char *end = url + strlen(url) - 1;
        while ((char)*end == '&') end--;
        end[1] = '\0';
    }
    else {
        url = concat(5, "https://img.foxes.cool/", name, "/", iota(id), ".jpg");
    }

    return url;
}

char* foxes_fox(foxes_options options) {
    return internal((char*) "fox", (Tags)Fox, options);
}

char* foxes_curious(foxes_options options) {
    return internal((char*) "curious", (Tags)Curious, options);
}

char* foxes_happy(foxes_options options) {
    return internal((char*) "happy", (Tags)Happy, options);
}
char* foxes_scary(foxes_options options) {
    return internal((char*) "scary", (Tags)Scary, options);
}
char* foxes_sleeping(foxes_options options) {
    return internal((char*) "sleeping", (Tags)Sleeping, options);
}
