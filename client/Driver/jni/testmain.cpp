
#include "GeoHash.h"
#include <stdio.h>

int main()
{
    char buf[32];
    GeoHash::geohash(116.3906, 39.92324, buf, 20);
    printf("%s\n", buf);
    GeoHash::geohash(116.390, 39.9232, buf, 20);

    printf("%s\n", buf);
    return 0;
}
