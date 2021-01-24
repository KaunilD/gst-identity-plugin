#include <CL/cl2.hpp>

#include <sstream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <cassert>


class OCL{
public:
    OCL(){};
    
    void check_platforms();
};