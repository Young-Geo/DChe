#ifndef CURLWRAP_H
#define CURLWRAP_H

class CurlResponse
{
public:
    char* data;
    int size;

    CurlResponse();
    ~CurlResponse();

private:
    CurlResponse(const CurlResponse&);
    CurlResponse& operator=(const CurlResponse&);
};


class CurlWrap
{
public:
    CurlWrap();

    // POST METHOD
    static bool send(const char* url, const char* data, CurlResponse* resp);
};

#endif // CURLWRAP_H
