__kernel void d_rgb2gray(
    const int W,
    const int H,
    const int C,
    __global const char* src,
    __global char* dst)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    dst[j*W + i] = max(0.2989*src[j*W + i + 2] + 0.5870*src[j*W + i + 1] + 0.1140*src[j*W + i], 1.0f);
    
}