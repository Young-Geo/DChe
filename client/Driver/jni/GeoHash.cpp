#include "GeoHash.h"
#include <inttypes.h>
GeoHash::GeoHash()
{

}

char *GeoHash::geohash(double lng, double lat, char *buf, int bits)
{
    uint64_t lngLatBits = 0;

    double lngMin = -180;
    double lngMax = 180;
    double latMin = -90;
    double latMax = 90;


    for(int i=0; i<bits; ++i)
    {
        lngLatBits<<=1;

        double lngMid = (lngMax + lngMin)/2;
        if(lng > lngMid)
        {
            lngLatBits += 1;
            lngMin = lngMid;
        }
        else
        {
            lngMax = lngMid;
        }

        lngLatBits <<=1;

        double latMid = (latMax + latMin)/2;
        if(lat > latMid)
        {
            lngLatBits += 1;
            latMin = latMid;
        }
        else
        {
            latMax = latMid;
        }
    }

    static char base32encode[] = "0123456789bcdefghjkmnpqrstuvwxyz";

    // 11010 10010 10010 10010 10101 11011 00100 11000
    // a     b     c     1     2     x     2     3    \0
    int i;
    for(i=0;i <8; ++i)
    {
        uint32_t index = lngLatBits >> (35-i*5);
        index &= 31;
        buf[i] = base32encode[index];
    }

    buf[i] = 0;
    return buf;
}
