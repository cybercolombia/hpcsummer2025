g++ -O3 -Wall -shared -std=c++11 -fopenmp -fPIC $(python3 -m pybind11 --includes) -I . npmatmul.cpp -o npmatmul$(python3-config --extension-suffix)
