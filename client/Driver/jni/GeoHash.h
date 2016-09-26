#ifndef GEOHASH_H
#define GEOHASH_H

#include <inttypes.h>

class GeoHash
{
public:
    GeoHash();

    static char* geohash(double lng, double lat, char* buf, int bits = 20);
};

#endif // GEOHASH_H
