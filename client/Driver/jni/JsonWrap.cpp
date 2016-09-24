#include "JsonWrap.h"

JsonWrap::JsonWrap()
{
    json = NULL;
    buf = NULL;
}

JsonWrap::~JsonWrap()
{
    if(json) cJSON_Delete(json);
    if(buf) free(buf);
}

void JsonWrap::wrap(const char *key, const char *value, ...)
{
    va_list ap;
    va_start(ap, value);
    wrap(key, value, ap);
    va_end(ap);
}

void JsonWrap::wrap(const char *key, const char *value, va_list ap)
{
    if(json) cJSON_Delete(json);

    json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, key, value);

    while(1)
    {
        const char* k = va_arg(ap, const char*);
        if(k==NULL)
            break;
        const char* v = va_arg(ap, const char*);
        cJSON_AddStringToObject(json, k, v);
    }
}

void JsonWrap::wrap2buf(const char *key, const char *value, ...)
{
    va_list ap;
    va_start(ap, value);
    wrap(key, value, ap);
    va_end(ap);

    if(buf) free(buf);
    buf = cJSON_Print(json);
}

void JsonWrap::parse(const char *data)
{
    if(json) cJSON_Delete(json);
    json = cJSON_Parse(data);
}

const char *JsonWrap::value(const char *key)
{
    if(json == NULL) return NULL;

    cJSON* obj = cJSON_GetObjectItem(json, key);

    return obj->valuestring; // result: ok
}


#if 0


#endif
