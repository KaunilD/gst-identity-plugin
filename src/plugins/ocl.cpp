#include <ocl.h>

using namespace std;

cl::Platform
OCL::get_platforms(){
        std::vector<cl::Platform> all_platforms;
        cl::Platform::get(&all_platforms);
        
        if (all_platforms.size() == 0){
                cout << "No platforms found. \n";
                exit(1);
        }


        cl::Platform default_platform = all_platforms.front();
        cout << "Using platform: "<< default_platform.getInfo<CL_PLATFORM_NAME>() <<"\n";

        return default_platform;
};

cl::Device
OCL::get_device(cl::Platform& platform){
        std::vector<cl::Device> all_devices;
        platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
        
        if(all_devices.size() == 0){
                cout<<" No devices found. Check OpenCL installation!\n";
                exit(1);
        }
        
        cl::Device default_device = all_devices.front();
        std::cout<< "Using device: "<< default_device.getInfo<CL_DEVICE_NAME>() <<"\n";
        
}

cl::Context
OCL::get_context(cl::Device& device){
        cl::Context context(device);
        return context;
}

cl::Program
OCL::get_program(std::string& source, cl::Context& context, cl::Device& device){

        cl::Program::Sources sources;
        sources.push_back({
                source.c_str(), 
                source.length()+1
        });

        cl::Program program(context, sources);

        cl_int status = program.build({device});
        if (status != CL_SUCCESS) {
                std::cout << "Error Code:\t" << status << std::endl;
                std::cout << "Error building:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
                exit(1);
        }
        
        return program;

};