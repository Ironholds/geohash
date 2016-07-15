#include "cgeohash.h"

//'@title Encode Latitude and Longitude Values
//'@description \code{\link{gh_encode}} generates geohashes of a
//'given precision from vectors of latitude and longitude values.
//'
//'@param lats a numeric vector of latitude values.
//'
//'@param lngs a numeric vector of longitude values. Must be the same length
//'as \code{lat}.
//'
//'@param precision an integer representing the precision the hashes should have.
//'This should be between 1 and 10; if the precision requested is greater than 10, it will
//'use 10 - if less than 1, it will error.
//'
//'@return a character vector of hashes, the same length as \code{lat} and \code{lng},
//'with \code{NA} values where one of the equivalent lat/lng pair was NA.
//'
//'@examples
//'# A simple example:
//'gh_encode(lat = 42.60498046875, lng = -5.60302734375, precision = 5)
//'#[1] "ezs42"
//'
//'@seealso \code{\link{gh_decode}}, for taking geohashes and turning them back into coordinates,
//'and \code{\link{gh_neighbours}} for retrieving the neighbouring hashes to a particular hash.
//'
//'@export
//[[Rcpp::export]]
CharacterVector gh_encode(NumericVector lats, NumericVector lngs,
                          IntegerVector precision = 6){

  // Initial checks
  if(precision.size() < 1 || IntegerVector::is_na(precision[0])){
    Rcpp::stop("A value for precision must be provided, and cannot be NA");
  }
  if(precision[0] < 1){
    Rcpp::stop("Precision must be greater than 1");
  }

  signed int& precision_ref = precision[0];
  if(precision_ref > 10){
    Rcpp::warning("Precision is greater than 10, the maximum. 10 used instead.");
    precision_ref = 10;
  }

  const unsigned int input_size = lats.size();
  if(input_size != lngs.size()){
    Rcpp::stop("The lats and lngs vectors must be the same size");
  }

  // Set up output and run
  CharacterVector output(input_size);

  for(unsigned int i = 0; i < input_size; i++){

    //Check to see if the user has asked us to stop doing the thing.
    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }

    // Check to see if one of the inputs is NA
    if(NumericVector::is_na(lats[i]) || NumericVector::is_na(lngs[i])){
      output[i] = NA_STRING;
    } else {
      // Do the thing.
      output[i] = cgeohash::encode(lats[i], lngs[i], precision_ref);
    }
  }

  return output;
}

//'@title Decode Geohashes
//'@description \code{gh_decode} takes geohashes and turns them back into
//'latitude/longitude pairs, with an associated margin of error for each value.
//'
//'@param hashes a character vector of geohashes.
//'
//'@return a data.frame of four columns; "lat", "lng", "lat_error" and "lng_error"
//'
//'@seealso \code{\link{gh_encode}} for generating geohashes, and
//'\code{\link{gh_neighbours}} for identifying the neighbouring hash boxes
//'to a geohash.
//'
//'@examples
//'# A simple example:
//'gh_encode(lat = 42.60498046875, lng = -5.60302734375, precision = 5)
//'#[1] "ezs42"
//'
//'gh_decode("ezs42")
//'# lat      lng      lat_error  lng_error
//'# 42.60498 -5.603027 0.02197266 0.02197266
//'@export
//[[Rcpp::export]]
DataFrame gh_decode(CharacterVector hashes){

  unsigned int input_size = hashes.size();
  NumericVector lats(input_size);
  NumericVector lngs(input_size);
  NumericVector lat_error(input_size);
  NumericVector lng_error(input_size);
  for(unsigned int i = 0; i < input_size; i++){

    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }

    if(CharacterVector::is_na(hashes[i])){
      lats[i] = NA_REAL;
      lngs[i] = NA_REAL;
      lat_error[i] = NA_REAL;
      lng_error[i] = NA_REAL;
    } else {
      cgeohash::DecodedHash holding = cgeohash::decode(Rcpp::as<std::string>(hashes[i]));

      lats[i] = holding.latitude;
      lngs[i] = holding.longitude;
      lat_error[i] = holding.latitude_err;
      lng_error[i] = holding.longitude_err;
    }

  }

  return DataFrame::create(_["lat"] = lats,
                           _["lng"] = lngs,
                           _["lat_error"] = lat_error,
                           _["lng_error"] = lng_error);
}

//'@rdname neighbours
//'@export
// [[Rcpp::export]]
DataFrame gh_neighbours(CharacterVector hashes){

  unsigned int input_size = hashes.size();
  CharacterVector north(input_size);
  CharacterVector ne(input_size);
  CharacterVector east(input_size);
  CharacterVector se(input_size);
  CharacterVector south(input_size);
  CharacterVector sw(input_size);
  CharacterVector west(input_size);
  CharacterVector nw(input_size);
  std::vector < std::string > holding(8);

  for(unsigned int i = 0; i < input_size; i++){

    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }

    if(CharacterVector::is_na(hashes[i])){
      north[i] = NA_REAL;
      ne[i] = NA_REAL;
      east[i] = NA_REAL;
      se[i] = NA_REAL;
      south[i] = NA_REAL;
      sw[i] = NA_REAL;
      west[i] = NA_REAL;
      nw[i] = NA_REAL;
    } else {

      holding = cgeohash::all_neighbours(Rcpp::as<std::string>(hashes[i]));

      north[i] = holding[0];
      ne[i] =  holding[1];
      east[i] =  holding[2];
      se[i] =  holding[3];
      south[i] =  holding[4];
      sw[i] =  holding[5];
      west[i] =  holding[6];
      nw[i] =  holding[7];
    }

  }

  return DataFrame::create(_["north"] = north,
                           _["northeast"] = ne,
                           _["east"] = east,
                           _["southeast"] = se,
                           _["south"] = south,
                           _["southwest"] = sw,
                           _["west"] = west,
                           _["northwest"] = nw,
                           _["stringsAsFactors"] = false);

}

// [[Rcpp::export]]
CharacterVector gh_neighbour(CharacterVector hashes, IntegerVector direction){

  unsigned int input_size = hashes.size();
  int directions [2] = {direction[0], direction[1]};
  CharacterVector output(input_size);
  for(unsigned int i = 0; i < input_size; i++){

    if((i % 10000) == 0){
      Rcpp::checkUserInterrupt();
    }

    if(CharacterVector::is_na(hashes[i])){
      output[i] = NA_STRING;
    } else {
      output[i] = cgeohash::neighbor(Rcpp::as<std::string>(hashes[i]), directions);
    }
  }

  return output;
}
