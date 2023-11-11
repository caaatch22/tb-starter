# tb-starter
test and benchmark quick start!


## Build
conan >= 1.60.2  // which support gcc13

gcc 11 required.

```sh
mkdir -p build
cd build
conan install .. --build=missing

cmake .. [-DBUILD_TYPE=Release or Debug]
make -j
./bin/test # for example
```

