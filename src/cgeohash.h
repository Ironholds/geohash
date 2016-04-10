#ifndef __CGEOHASH_HPP__
#define __CGEOHASH_HPP__

#include <Rcpp.h>
using namespace Rcpp;
namespace cgeohash {

struct DecodedBBox {
    double minlat, minlon, maxlat, maxlon;
};

struct DecodedHash {
    double latitude;
    double longitude;

    double latitude_err;
    double longitude_err;
};

// Encode a pair of latitude and longitude into geohash
std::string encode(double latitude, double longitude, int precision);

// Decode a hash string into pair of latitude and longitude
DecodedHash decode(std::string hash_string);

// Decode hashstring into a bound box matches it
DecodedBBox decode_bbox(std::string hash_string);

// Find neighbor of a geohash string in certain direction.
// Direction is a two-element array:
// Ex: [ 1, 0] == north
// Ex: [-1,-1] == southwest
std::string neighbor(std::string hash_string, const int direction []);

std::vector < std::string > all_neighbours(std::string hash);
}

#endif
