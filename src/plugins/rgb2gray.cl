__kernel void d_rgb2gray(
    const int W,
    const int H,
    const int C,
    __global const char* src,
    __global char* dst)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    dst[j*W + i] = 0.5*src[j*W + i] + 0.2*src[j*W + i + 1] + 0.8*src[j*W + i+ 2]; 
}