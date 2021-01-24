__constant float4 gray_coeffs = {0.29f, 0.58f, 0.11f, 0.0f};

__kernel void function(__global uchar* data){
    data[get_global_id(0)]*=2;
    
}