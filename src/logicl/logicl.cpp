#include <logicl.h>

using std::unique_ptr;
using std::shared_ptr;

LogiCL::LogiCL(std::string path):kernel_path(path){
    
}

int
LogiCL::read_cl_file(std::string path, std::string& dst_string){
    using std::string;
    using std::fstream;

    char* file_str;
    fstream f(path.c_str(), (fstream::in | fstream::binary));
    if (f.is_open()) {
        size_t len;
        f.seekg(0, fstream::end);
        
        len = (size_t)f.tellg();
        f.seekg(0, fstream::beg);
        
        file_str = (char *) malloc(len + 1);
        f.read(file_str, len);
        file_str[len] = '\0';
        
        dst_string = std::string(file_str);

        free(file_str);
        f.close();

        return 0;
    }

    std::cout << LOG_OCL << "open file failed!\n";
    return -1;
}

int
LogiCL::build_program(){
    using namespace std;

    // get platform
    vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
        
    if (all_platforms.size() == 0){
        cout << LOG_OCL << "No platforms found. \n";
        exit(1);
    }

    auto platform = all_platforms.front();
    cout << LOG_OCL << "Using platform: "<< platform.getInfo<CL_PLATFORM_NAME>() <<"\n";
    
    // get device
    std::vector<cl::Device> all_devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
    
    if(all_devices.size() == 0){
        cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    
    auto device = all_devices.front();
    std::cout<< "Using device: "<< device.getInfo<CL_DEVICE_NAME>() <<"\n";
    
    // get context
    auto context = cl::Context(device);
    
    std::string kernel_code;

    int status = read_cl_file(kernel_path, kernel_code);
    if( status < 0){
        std::cout << LOG_OCL << "Error reading source file!\n";
    }

    // get program
    auto program = get_program(kernel_code, context, device);
    this->program = shared_ptr<cl::Program>(new cl::Program(std::move(program)));
    
    return this->program->getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
}


cl::Program
LogiCL::get_program(std::string& source, cl::Context context, cl::Device device){
        
        cl::Program::Sources sources;
        sources.push_back({source.c_str(), source.length()});
        
        cl::Program program(context, sources);

        auto status = program.build({device});

        if (status != CL_SUCCESS) {
                std::cout << "Error Code:\t" << status << std::endl;
                std::cout << "Error building:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
                exit(1);
        }
        
        return std::move(program);
};
