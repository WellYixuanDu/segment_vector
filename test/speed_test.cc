#include "segment_vector.h"

#include <vector>
#include <iostream>
inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}
#define CPU_FRE (2800000000)

void test_push_back()
{
    std::cout << "Test push_back 10000 elems" << std::endl;
    int count = 10000;
    std::vector<int> tmp_vec;
    uint64_t vec_begin;
    uint64_t vec_end;
    vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        tmp_vec.push_back(i);
    }
    vec_end = rdtsc();

    segment_vector<int> tmp_seg_vec;
    uint64_t seg_vec_begin;
    uint64_t seg_vec_end;
    seg_vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        tmp_seg_vec.push_back(i);
    }
    seg_vec_end = rdtsc();    
    double vec_cost;
    double seg_vec_cost;
    vec_cost = (vec_end - vec_begin)/double(CPU_FRE);
    seg_vec_cost = (seg_vec_end - seg_vec_begin)/double(CPU_FRE);
    std::cout << "vector push_back 10000 elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector push_back 10000 elems costs " << seg_vec_cost << std::endl;
}

void test_emplace_back()
{
    std::cout << "Test emplace_back 10000000 elems" << std::endl;
    int count = 1000;
    std::vector<int> tmp_vec;
    uint64_t vec_begin;
    uint64_t vec_end;
    vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        tmp_vec.emplace_back(i);
    }
    vec_end = rdtsc();

    segment_vector<int> tmp_seg_vec;
    uint64_t seg_vec_begin;
    uint64_t seg_vec_end;
    seg_vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        tmp_seg_vec.emplace_back(i);
    }
    seg_vec_end = rdtsc();    
    double vec_cost;
    double seg_vec_cost;
    vec_cost = (vec_end - vec_begin)/double(CPU_FRE);
    seg_vec_cost = (seg_vec_end - seg_vec_begin)/double(CPU_FRE);
    std::cout << "vector emplace_back 10000 elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector emplace_back 10000 elems costs " << seg_vec_cost << std::endl;    
}


void test_resize()
{
    std::cout << "Test resize 100000 elems" << std::endl;
    int count = 10000000;
    std::vector<int> tmp_vec;
    uint64_t vec_begin;
    uint64_t vec_end;
    vec_begin = rdtsc();
    tmp_vec.resize(10000, 10);
    vec_end = rdtsc();

    segment_vector<int> tmp_seg_vec;
    uint64_t seg_vec_begin;
    uint64_t seg_vec_end;
    seg_vec_begin = rdtsc();
    tmp_seg_vec.resize(10000, 10);
    seg_vec_end = rdtsc();    
    double vec_cost;
    double seg_vec_cost;
    vec_cost = (vec_end - vec_begin)/double(CPU_FRE);
    seg_vec_cost = (seg_vec_end - seg_vec_begin)/double(CPU_FRE);
    std::cout << "vector resize 10000 elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector resize 10000 elems costs " << seg_vec_cost << std::endl;    
}


void test_traverse()
{
    std::cout << "Test push_back 10000 elems" << std::endl;
    int count = 10000;
    std::vector<int> tmp_vec(count, 10);
    uint64_t vec_begin;
    uint64_t vec_end;
    long long int vec_sum;
    vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        vec_sum += tmp_vec[i];
    }
    vec_end = rdtsc();

    segment_vector<int> tmp_seg_vec(count, 10);
    uint64_t seg_vec_begin;
    uint64_t seg_vec_end;
    long long int seg_vec_sum;
    seg_vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        seg_vec_sum += tmp_seg_vec[i];
    }
    seg_vec_end = rdtsc();    
    double vec_cost;
    double seg_vec_cost;
    vec_cost = (vec_end - vec_begin)/double(CPU_FRE);
    seg_vec_cost = (seg_vec_end - seg_vec_begin)/double(CPU_FRE);
    std::cout << "vector traverse 10000 elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector traverse 10000 elems costs " << seg_vec_cost << std::endl;    
}


int main()
{
    test_push_back();
    test_emplace_back();
    test_resize();
    test_traverse();
    return 0;
}