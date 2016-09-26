//
// Created by xueguoliang on 16-9-24.
//

#ifndef DRIVER_USER_H
#define DRIVER_USER_H

#include "GeoHash.h"

extern char* sessionid;
bool Login(const char* username, const char* password);
bool SetStatus(int status);
int UpdateGeoInfo(double lng, double lat);

#endif //DRIVER_USER_H
