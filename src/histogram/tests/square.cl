__kernel void square(__global int *data, unsigned int size) {
    uint gid = get_global_id(0);
    if(gid < size) {
        data[gid] = data[gid] * data[gid];
    }
}
