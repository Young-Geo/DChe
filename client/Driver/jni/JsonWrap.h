#ifndef JSONWRAP_H
#define JSONWRAP_H



#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"



class JsonWrap
{
public:
    JsonWrap();
    ~JsonWrap();

    cJSON* json;
    char* buf;

    void wrap(const char* key, const char* value, ...);
    void wrap(const char* key, const char* value, va_list ap);
    void wrap2buf(const char* key, const char* value, ...);

    void parse(const char* data);
    const char* value(const char* key);


#if 0
    cJSON* wrap(const char* key, const char* value, ...);
    cJSON* wrap(const char* key, const char* value, va_list ap);

    char* wrap2buf(const char* key, const char* value, ...);
    const char* value(cJSON* json, const char* key);
    cJSON* toJson(char* buf);
#endif

private:
    JsonWrap(const JsonWrap&);
    JsonWrap& operator=(const JsonWrap&);

};

#endif // JSONWRAP_H
