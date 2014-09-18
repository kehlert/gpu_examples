__kernel void generateSubHists(const uint lower,
                                const uint upper,
                                __global int *data,
                                uint dataSize,
                                uint histSize,
                                __local uint *subHist,
                                __global uint* subHists) {
    uint gid = get_global_id(0);
    uint lid = get_local_id(0);

    if(lid == 0) {
        for(uint i = 0 ; i < histSize; ++i) {
            subHist[i] = 0;
        }
    }

    barrier(CLK_LOCAL_MEM_FENCE);
   
    for (uint i = gid; i < dataSize; i += get_global_size(0)) {
        atomic_inc(&subHist[data[i] - lower]);
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    for (uint i = lid; i < histSize; i += get_local_size(0)) {
        subHists[get_group_id(0) * histSize + i] = subHist[i];
    }
}
