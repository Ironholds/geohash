#include "cgeohash.h"
#include <math.h>

namespace cgeohash {

// Static array of 0-9, a-z
const char base32_codes[] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'j',
    'k',
    'm',
    'n',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z'
};

// direction constants
const int dir_east[] = {0,1};
const int dir_southeast[] = {-1,1};
const int dir_south[] = {-1,0};
const int dir_southwest[] = {-1,-1};
const int dir_west[] = {0,-1};
const int dir_northwest[] = {1,-1};
const int dir_north[] = {1,0};
const int dir_northeast[] = {1,1};

// Reverse map of characters --> index position
const std::map<char, int> build_base32_indexes(){
  std::map<char, int> output;

  for(unsigned int i = 0; i < strlen(base32_codes); i++) {
      output.insert( std::pair<char, int>(base32_codes[i], i) );
  }

  return output;
}

// Build a map of characters -> index position from the above array
const std::map<char, int> base32_indexes = build_base32_indexes();

// Convert the index position to the character in the array
char base32_codes_value_of(int index)
{
  return base32_codes[index];
}

// Convert the character to the index position in the array int
int base32_codes_index_of(char c) {
  std::map<char,int>::const_iterator it = base32_indexes.find(c);
  if(it==base32_indexes.end()){
    return -1;
  }
  return it->second; }

std::string encode(double latitude, double longitude, unsigned int precision){
  // DecodedBBox for the lat/lon + errors
  std::string output;
  DecodedBBox bbox;
  bbox.maxlat = 90;
  bbox.maxlon = 180;
  bbox.minlat = -90;
  bbox.minlon = -180;
  double mid        = 0;
  bool   islon      = true;
  int    num_bits   = 0;
  int    hash_index = 0;

  // Pre-Allocate the hash string
  output = std::string(precision, ' ');
  unsigned int output_length = 0;

  while(output_length < precision) {
    if (islon) {
      mid = (bbox.maxlon + bbox.minlon) / 2;
      if(longitude > mid) {
        hash_index = (hash_index << 1) + 1;
        bbox.minlon=mid;
      } else {
        hash_index = (hash_index << 1) + 0;
        bbox.maxlon=mid;
      }
    } else {
      mid = (bbox.maxlat + bbox.minlat) / 2;
      if(latitude > mid ) {
        hash_index = (hash_index << 1) + 1;
        bbox.minlat = mid;
      } else {
        hash_index = (hash_index << 1) + 0;
        bbox.maxlat = mid;
      }
    }
    islon = !islon;

    ++num_bits;
    if (5 == num_bits) {
      // Append the character to the pre-allocated string
      // This gives us roughly a 2x speed boost
      output[output_length] = base32_codes[hash_index];

      output_length++;
      num_bits   = 0;
      hash_index = 0;
    }
  }
  return output;
}

DecodedBBox decode_bbox(std::string _hash_string){
  // Copy of the string down-cased
  // Wish this was ruby, then it would be simple: _hash_string.downcase();
  std::string hash_string(_hash_string);
  std::transform(
    _hash_string.begin(),
    _hash_string.end(),
    hash_string.begin(),
    ::tolower);

  DecodedBBox output;
  output.maxlat = 90;
  output.maxlon = 180;
  output.minlat = -90;
  output.minlon = -180;

  bool islon = true;

  for(int i = 0, max = hash_string.length(); i < max; i++) {
    int char_index = base32_codes_index_of(hash_string[i]);
    if(char_index ==-1){
      Rcpp::stop("Invalid character "+std::string(1, hash_string[i])+ " for hash "+hash_string);
      output.maxlat = 90;
      output.maxlon = 180;
      output.minlat = -90;
      output.minlon = -180;
      break;
    }
    for (int bits = 4; bits >= 0; --bits) {
      int bit = (char_index >> bits) & 1;
      if (islon) {
        double mid = (output.maxlon + output.minlon) / 2;
        if(bit == 1) {
          output.minlon = mid;
        } else {
          output.maxlon = mid;
        }
      } else {
        double mid = (output.maxlat + output.minlat) / 2;
        if(bit == 1) {
          output.minlat = mid;
        } else {
          output.maxlat = mid;
        }
      }
      islon = !islon;
    }
  }
  return output;
}

DecodedHash decode(std::string hash_string){
  DecodedBBox bbox = decode_bbox(hash_string);
  DecodedHash output;
  output.latitude      = (bbox.minlat + bbox.maxlat) / 2;
  output.longitude     = (bbox.minlon + bbox.maxlon) / 2;
  output.latitude_err  = bbox.maxlat - output.latitude;
  output.longitude_err = bbox.maxlon - output.longitude;
  return output;
}

std::string neighbor(std::string hash_string, const int direction [])
{
  DecodedHash lonlat = decode(hash_string);

  return neighbor(lonlat, direction, hash_string.length());
}

std::string neighbor(DecodedHash lonlat, const int direction [], const int length)
{
  // Adjust the DecodedHash for the direction of the neighbors
  lonlat.latitude   += direction[0] * lonlat.latitude_err * 2;
  lonlat.longitude  += direction[1] * lonlat.longitude_err * 2;

  //pole check
  if(fabs(lonlat.latitude)>90.){
    lonlat.latitude   -= direction[0] * lonlat.latitude_err * 2;
    lonlat.longitude  += 180.0;
  }

  return encode(lonlat.latitude, fix_longitude(lonlat.longitude), length);
}

std::vector < std::string > all_neighbours(std::string hash){

  std::vector < std::string > output(8);
  DecodedHash lonlat = decode(hash);
  unsigned int input_size = hash.size();

  output[0] = neighbor(lonlat, dir_north, input_size); // North
  output[1] = neighbor(lonlat, dir_northeast, input_size); // Northeast
  output[2] = neighbor(lonlat, dir_east, input_size); // East
  output[3] = neighbor(lonlat, dir_southeast, input_size); // Southeast
  output[4] = neighbor(lonlat, dir_south, input_size); // South
  output[5] = neighbor(lonlat, dir_southwest, input_size); // Southwest
  output[6] = neighbor(lonlat, dir_west, input_size); // West
  output[7] = neighbor(lonlat, dir_northwest, input_size); // Northwest
  return output;
}


double fix_longitude(double longitude){
  if(longitude < -180.){
    longitude += 360.;
    return longitude;
  }
  if(longitude > 180.){
    longitude -= 360.;
    return longitude;
  }
  return(longitude);
}
} // end namespace cgeohash
