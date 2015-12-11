# README #

This is a simple console app for indexing a collection of tagged music
files (currently only supports MP3 ID3V2 tags).  Once your collection is
indexed, you can then run SQL queries using any tool that works with
sqlite3 databases.  This is handy for running scripts to manage your
collection, generate playlists, etc.

### What is this repository for? ###

* Index your music collection with a simple command-line tool!
* Version 0.01 (alpha)

### How do I get set up? ###

To build this application, you will need the following:

* c++11 compiler
* cmake 3.2
* boost 1.55
* taglib 1.10
* vsqlitepp 0.3.13

Here is an example build on a unix system (starting in the top-level directory of the project):

```
$ mkdir build

$ cd build/

$ : /usr/local/bin/cmake .. -G "Unix Makefiles"
-- The C compiler identification is GNU 4.9.2
-- The CXX compiler identification is GNU 4.9.2
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Boost version: 1.55.0
-- Found the following Boost libraries:
--   system
--   filesystem
--   program_options
-- VSQlite++ found.
--   Include path: /usr/include
--   Library path: /usr/lib/x86_64-linux-gnu/libvsqlitepp.so
-- Found PkgConfig: /usr/bin/pkg-config (found version "0.28") 
-- checking for module 'taglib'
--   found taglib, version 1.10
-- Configuring done
-- Generating done
-- Build files have been written to: /xxx/xxx/xxx/build

$ make
Scanning dependencies of target tagdb
[ 50%] Building CXX object CMakeFiles/tagdb.dir/src/main.cpp.o
[100%] Linking CXX executable tagdb
[100%] Built target tagdb

$ sql sqlite3:///$PWD/tag.db < tagdb.sql 

$ ./tagdb --music /xxx/xxx/xxx/Music --database tag.db 2>/dev/null
files: 4221
artists: 449
albums: 444
titles: 2879
genres: 121

$ sql sqlite3:///$PWD/tag.db <<< "select * from artist;" | tail -n10
440|Gil Gutierrez
441|Bad Hair Day
442|Menorah
443|JOHN GRISHAM
444|DJ Patrick Bateman
445|Smashing Pumpkins
446|Estrella Morente
447|El Potito con Estrella Morente
448|Pearl Jam
449|Red Hot Chili Peppers
```

### TIP ###
Building the database is I/O intensive.   Using a RAM disk for this
step, like *tmpfs* ( https://en.wikipedia.org/wiki/Tmpfs ), will make
a huge difference.
