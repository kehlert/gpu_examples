__kernel void square(__global int *data) {
    uint gid = get_global_id(0);
    data[gid] = data[gid] * data[gid];
}
