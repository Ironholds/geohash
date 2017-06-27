## geohash
A package for encoding, decoding and neighbouring geohashes

__Author:__ Oliver Keyes<br/>
__License:__ [MIT](http://opensource.org/licenses/MIT)<br/>
__Status:__ Stable

[![Travis-CI Build Status](https://travis-ci.org/Ironholds/geohash.svg?branch=master)](https://travis-ci.org/Ironholds/geohash) [![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/geohash)](https://cran.r-project.org/package=geohash) ![downloads](http://cranlogs.r-pkg.org/badges/grand-total/geohash)

### Description

*geohashes* are a way of representing latitude/longitude pairs as individual,
short strings of numbers and letters. The <code>geohash</code> package provides
tools for:

* Encoding latitude/longitude pairs into geohashes;
* Decoding geohashes into latitude/longitude pairs;
* Identifying the neighbouring geohashes of a provided geohash (or vector of geohashes).

For more information, see the [geohash vignette](https://github.com/Ironholds/geohash/blob/master/vignettes/geohash.Rmd).

Please note that this project is released with a [Contributor Code of Conduct](https://github.com/Ironholds/geohash/blob/master/CONDUCT.md).
By participating in this project you agree to abide by its terms.

### Installation
    
To get the development version (currently recommended):

    devtools::install_github("ironholds/geohash")
    
The latest CRAN version can be obtained via:

    install.packages("geohash")
