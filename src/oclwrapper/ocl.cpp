#include <ocl.h>

using namespace std;

void
OCL::check_platforms(){
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        auto platform = platforms.front();
        
};