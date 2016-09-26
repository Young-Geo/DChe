//
// Created by xueguoliang on 16-9-24.
//

#include "User.h"
#include "JsonWrap.h"
#include "CurlWrap.h"
#include "JniUtil.h"
char* sessionid;
char* gorderid = NULL;

char* startlng = NULL;
char* startlat = NULL;
char* endlng = NULL;
char* endlat = NULL;

static const char* userUrl = "https://www.xueguoliang.cn:23450/user";


static int sendJsonToServer(JsonWrap& req, JsonWrap& respJson)
{
    CurlWrap curl;
    CurlResponse resp;
    bool ret = curl.send(userUrl, req.buf, &resp);
    if(ret)
    {
        respJson.parse(resp.data);
        return 0;
    }

    return -1;
}

bool Login(const char* username, const char* password)
{
    JsonWrap req;
    JsonWrap respJson;

    req.wrap2buf("username", username,
                 "password", password,
                 "type", "driver",
                 "cmd", "login", NULL);

    int ret = sendJsonToServer(req, respJson);
    if(ret != 0)
        return false;

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

#if 0
bool Login(const char* username, const char* password)
{
    JsonWrap req;
    req.wrap2buf("username", username,
                 "password", password,
                 "type", "driver",
                 "cmd", "login", NULL);


    // send json buffer to server
    CurlResponse resp;
    CurlWrap::send(userUrl, req.buf, &resp);

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
#endif

bool SetStatus(int status)
{
    /*
        {
            sessionid: id,
            status: status
        }
    */
    char statusBuf[10];
    JsonWrap json;
    json.wrap2buf("sessionid", sessionid,
                  "status", JniUtil::convert(status, statusBuf),
                  "cmd", "setstatus",
                  NULL);

    MyErr("int status=%d, status, char* statBuf=%s\n", status, statusBuf);
    JsonWrap jsonResp;


    int ret = sendJsonToServer(json, jsonResp);
    if(ret != 0)
        return false;

    if(strcmp(jsonResp.value("result"), "ok") == 0)
    {
        return true;
    }
    else
    {
        MyErr("set status error, reason is: %s", jsonResp.value("reason"));
    }

    return false;
}



/*
    < 0 error
    == 0 no order OK
    > 0 has order OK
*/
int UpdateGeoInfo(double lng, double lat)
{
    char lngBuf[32];
    char latBuf[32];
    char geohashBuf[32];

    MyErr("update geoinfo lng=%g, lat=%g", lng, lat);
    /*
    {
        cmd: orderget,
        sessionid: session,
        lng: xxxx.xxxx,
        lat: xxxx.xxxx
        geohash: xxxxxxxx
    }
    */

    JsonWrap json;
    json.wrap2buf("cmd", "orderget", "sessionid", sessionid,
                  "lng", JniUtil::convert(lng, lngBuf),
                  "lat", JniUtil::convert(lat, latBuf),
                  "geohash", GeoHash::geohash(lng, lat, geohashBuf), NULL);
     JsonWrap jsonResp;


    int ret = sendJsonToServer(json, jsonResp);
    if(ret != 0)
        return -1;

    /*
     * // has order
        {
            result: ok,
            orderid: xxxxx
            userid: xxxx
            startlng:xxx
            startlat:xxx
            endlng:xxx
            endlat:xxx
        }

        // no order
        {
            result: ok
        }

        // err
        {
            result: err,
            reason: reason
        }
    */

    if(strcmp(jsonResp.value("result"), "ok") == 0)
    {
        const char* orderid = jsonResp.value("orderid");
        if(orderid == NULL)
            return 0; // no order OK

        if(gorderid) free(gorderid);
        gorderid = strdup(orderid);

        if(startlat) free(startlat);
        if(startlng) free(startlng);
        if(endlat) free(endlat);
        if(endlng) free(endlng);

        startlat = strdup(jsonResp.value("startlat"));
        startlng = strdup(jsonResp.value("startlng"));
        endlat = strdup(jsonResp.value("endlat"));
        endlng = strdup(jsonResp.value("endlng"));

        return 1; // has order OK
    }
    else
    {
        MyErr("set status error, reason is: %s", jsonResp.value("reason"));
    }

    return -1;

}
