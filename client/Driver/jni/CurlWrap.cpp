#include "CurlWrap.h"
#include "curl/curl.h"
#include "JniUtil.h"
CurlWrap::CurlWrap()
{

}

static size_t http_callback(char* ptr, size_t n, size_t m, void* cbk)
{
    CurlResponse* resp = (CurlResponse*)cbk;
    int count = n * m;
    memcpy(resp->data+resp->size, ptr, count);
    resp->size += count;
    return count;
}

bool CurlWrap::send(const char *url, const char *data, CurlResponse *resp)
{
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);

    CURLcode ret = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    resp->data[resp->size] = 0;

    if(ret != CURLE_OK)
    {
        MyErr("curl error, code is %d\n", (int)ret);
        return false;
    }

    return true;
}

CurlResponse::CurlResponse()
{
    data = new char[4096];
    size = 0;
}

CurlResponse::~CurlResponse()
{
    delete[] data;
}
