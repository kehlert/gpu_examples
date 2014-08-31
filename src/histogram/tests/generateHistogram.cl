__kernel void generateHistogram(uint lower,
                                uint upper,
                                __global int *data,
                                uint size,
                                __global uint *freq) {
    uint gid = get_global_id(0);
    if(gid == 0) {
        for(uint i = 0; i < size; ++i) {
            ++freq[data[i] - lower];
        }
    }
}
