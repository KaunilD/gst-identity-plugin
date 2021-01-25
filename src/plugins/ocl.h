#include <CL/cl2.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <cassert>


namespace OCL{
    cl::Platform get_platforms();
    cl::Device get_device(cl::Platform&);
    cl::Context get_context(cl::Device&);
    cl::Program get_program(std::string&, cl::Context&, cl::Device&);
};