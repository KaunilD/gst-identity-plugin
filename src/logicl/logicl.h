#ifndef __OPENCL_WRAPPER_H__
#define __OPENCL_WRAPPER_H__

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

#define LOG_OCL "LogiCL: "

class LogiCL{
    
    public:

        LogiCL(){};
        virtual ~LogiCL(){};
        LogiCL(std::string);

        std::string                     kernel_path;
        std::shared_ptr<cl::Program>    program;
        
        // Build program from new kernel file or
        // hooked kernel file
        int         build_program();
        int         build_program(std::string);
        int         read_cl_file(std::string, std::string&);

        // get program from the device and ctx
        cl::Program get_program(
            std::string& , 
            cl::Context , 
            cl::Device );
        
        // getters for obtaininig values from program.
        cl::Platform    get_platforms();
        cl::Device      get_device();
        cl::Context     get_context();
        
};

#endif