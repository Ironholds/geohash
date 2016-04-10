#include "cgeohash.h"

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

// Build a map of characters -> index position from the above array
const std::map<char, int> build_base32_indexes();
const std::map<char, int> base32_indexes = build_base32_indexes();

// Reverse map of characters --> index position
const std::map<char, int> build_base32_indexes(){
  std::map<char, int> output;

  for(int i = 0, max = 36; i < max; i++) {
      output.insert( std::pair<char, int>(base32_codes[i], i) );
  }

  return output;
}

// Convert the index position to the character in the array
char base32_codes_value_of(int index)
{
  return base32_codes[index];
}

// Convert the character to the index position in the array
int base32_codes_index_of(char c)
{
  return base32_indexes.find(c)->second;
}

std::string encode(double latitude, double longitude, int precision){
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
};

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
};

std::string neighbor(std::string hash_string, const int direction [])
{
    // Adjust the DecodedHash for the direction of the neighbors
    DecodedHash lonlat = decode(hash_string);
    lonlat.latitude   += direction[0] * lonlat.latitude_err * 2;
    lonlat.longitude  += direction[1] * lonlat.longitude_err * 2;

    return encode(lonlat.latitude, lonlat.longitude, hash_string.length());
}

std::vector < std::string > all_neighbours(std::string hash){

  std::vector < std::string > output(8);
  DecodedHash lonlat = decode(hash);
  unsigned int input_size = hash.size();

  output[0] = encode((lonlat.latitude + (1 * lonlat.latitude_err * 2)), // North
                     (lonlat.longitude + (0 * lonlat.longitude_err * 2)),
                     input_size);
  output[1] = encode((lonlat.latitude + (1 * lonlat.latitude_err * 2)), // Northeast
                     (lonlat.longitude + (1 * lonlat.longitude_err * 2)),
                     input_size);
  output[2] = encode((lonlat.latitude + (0 * lonlat.latitude_err * 2)), // East
                     (lonlat.longitude + (1 * lonlat.longitude_err * 2)),
                     input_size);
  output[3] = encode((lonlat.latitude + (-1 * lonlat.latitude_err * 2)), // Southeast
                     (lonlat.longitude + (1 * lonlat.longitude_err * 2)),
                     input_size);
  output[4] = encode((lonlat.latitude + (-1 * lonlat.latitude_err * 2)), // South
                     (lonlat.longitude + (0 * lonlat.longitude_err * 2)),
                     input_size);
  output[5] = encode((lonlat.latitude + (-1 * lonlat.latitude_err * 2)), // Southwest
                     (lonlat.longitude + (-1 * lonlat.longitude_err * 2)),
                     input_size);
  output[6] = encode((lonlat.latitude + (0 * lonlat.latitude_err * 2)), // West
                     (lonlat.longitude + (-1 * lonlat.longitude_err * 2)),
                     input_size);
  output[7] = encode((lonlat.latitude + (1 * lonlat.latitude_err * 2)), // Northwest
                     (lonlat.longitude + (-1 * lonlat.longitude_err * 2)),
                     input_size);
  return output;
}
} // end namespace cgeohash

