//
// Created by xueguoliang on 16-9-24.
//

#include "User.h"
#include "JsonWrap.h"
#include "CurlWrap.h"
#include "JniUtil.h"
char* sessionid;

bool Login(const char* username, const char* password)
{
    JsonWrap req;
    req.wrap2buf("username", username,
                 "password", password,
                 "type", "driver",
                 "cmd", "login", NULL);

    // send json buffer to server
    CurlResponse resp;
    CurlWrap::send("https://www.xueguoliang.cn:23450/user", req.buf, &resp);

    MyWarning("response buffer is: %s\n", resp.data);

    JsonWrap respJson;
    MyWarning("1\n");
    respJson.parse(resp.data);
    MyWarning("2\n");

    const char* result = respJson.value("result");
    if(result == NULL)
    {
        MyWarning("result is null\n");
    }
    else
    {
        MyWarning("result is %s\n", result);
    }
    MyWarning("3\n");
    if(0==strcmp(result, "ok"))
    {
        MyWarning("4\n");
        // save the session id
        const char* s = respJson.value("sessionid");
        MyWarning("5\n");
        sessionid = strdup(s);
        MyWarning("6\n");
        return true;
    }

    MyWarning("7\n");
    const char* reason = respJson.value("reason");
    MyWarning("login error %s\n", reason);

    return false;
}
