__kernel void reduceSubHists(__global uint *hist,
                             uint histSize,
                             __global uint* subHists,
                             uint nSubHists) {
    uint gid = get_global_id(0);

    for (uint i = gid; i < histSize; i+= get_global_size(0)) {
        uint bin = 0;
        for(uint j = 0; j < nSubHists; ++j) {
            bin += subHists[j * histSize + i];
        }
        hist[i] = bin;
    }
}
