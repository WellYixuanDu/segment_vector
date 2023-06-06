#include "segment_vector.h"

#include <vector>
#include <iostream>
inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}
#define CPU_FRE (2800000000)

class TestNode {
public:
  TestNode() = default;
  TestNode(int v, std::string str) : m_value(v), m_str(str) {}
  TestNode(const TestNode &other)
      : m_value(other.m_value), m_str(other.m_str) {}
  void print() {
    std::cout << "The m_value is " << m_value << " The m_str is " << m_str
              << std::endl;
  }
  ~TestNode() = default;

public:
  int m_value;
  std::string m_str;
};

void test_push_back()
{
    
    int count = 1000000;
    std::cout << "Test push_back " << count <<  " elems" << std::endl;
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
    std::cout << "vector push_back "<< count << " elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector push_back " << count <<" elems costs " << seg_vec_cost << std::endl;
}

void test_emplace_back()
{
    
    int count = 1000000;
    std::cout << "Test emplace_back " << count << " elems" << std::endl;
    std::vector<TestNode> tmp_vec;
    uint64_t vec_begin;
    uint64_t vec_end;
    vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        tmp_vec.emplace_back(TestNode(i, "hello"));
    }
    vec_end = rdtsc();

    segment_vector<TestNode> tmp_seg_vec;
    uint64_t seg_vec_begin;
    uint64_t seg_vec_end;
    seg_vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        tmp_seg_vec.emplace_back(TestNode(i, "hello"));
    }
    seg_vec_end = rdtsc();    
    double vec_cost;
    double seg_vec_cost;
    vec_cost = (vec_end - vec_begin)/double(CPU_FRE);
    seg_vec_cost = (seg_vec_end - seg_vec_begin)/double(CPU_FRE);
    std::cout << "vector emplace_back " << count << " elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector emplace_back " << count << " elems costs " << seg_vec_cost << std::endl;    
}


void test_resize()
{
    int count = 10000000;
    std::cout << "Test resize "<< count << " elems" << std::endl;
    std::vector<TestNode> tmp_vec;
    uint64_t vec_begin;
    uint64_t vec_end;
    vec_begin = rdtsc();
    tmp_vec.resize(count, TestNode(1, "hello"));
    vec_end = rdtsc();

    segment_vector<TestNode> tmp_seg_vec;
    uint64_t seg_vec_begin;
    uint64_t seg_vec_end;
    seg_vec_begin = rdtsc();
    tmp_seg_vec.resize(count, TestNode(1, "hello"));
    seg_vec_end = rdtsc();    
    double vec_cost;
    double seg_vec_cost;
    vec_cost = (vec_end - vec_begin)/double(CPU_FRE);
    seg_vec_cost = (seg_vec_end - seg_vec_begin)/double(CPU_FRE);
    std::cout << "vector resize "<< count << " elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector resize "<< count << " elems costs " << seg_vec_cost << std::endl;    
}


void test_traverse()
{
    int count = 1000000;
    std::cout << "Test traverse "<< count << " elems" << std::endl;
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


void test_pop_back()
{
    int count = 1000000;
    std::cout << "Test pop_back " << count << " elems" << std::endl;
    std::vector<TestNode> tmp_vec(count, TestNode(1, "hello"));
    uint64_t vec_begin;
    uint64_t vec_end;
    vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        tmp_vec.pop_back();
    }
    vec_end = rdtsc();

    segment_vector<TestNode> tmp_seg_vec(count, TestNode(1, "hello"));
    uint64_t seg_vec_begin;
    uint64_t seg_vec_end;
    seg_vec_begin = rdtsc();
    for (int i = 0; i < count; ++i)
    {
        tmp_seg_vec.pop_back();
    }
    seg_vec_end = rdtsc();    
    double vec_cost;
    double seg_vec_cost;
    vec_cost = (vec_end - vec_begin)/double(CPU_FRE);
    seg_vec_cost = (seg_vec_end - seg_vec_begin)/double(CPU_FRE);
    std::cout << "vector pop_back " << count << " elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector pop_back " << count << " elems costs " << seg_vec_cost << std::endl;      
}

void test_assign_val()
{
    int count = 10000000;
    int N = 10;
    std::cout << "Test assign " << count << " elems" << std::endl;
    std::vector<TestNode> tmp_vec;
    uint64_t vec_begin;
    uint64_t vec_end;
    vec_begin = rdtsc();
    for (int i = 0; i < N; ++i)
    {
        tmp_vec.assign(count, TestNode(i, "hello"));
    }
    vec_end = rdtsc();

    segment_vector<TestNode> tmp_seg_vec;
    uint64_t seg_vec_begin;
    uint64_t seg_vec_end;
    seg_vec_begin = rdtsc();
    for (int i = 0; i < N; ++i)
    {
        tmp_seg_vec.assign(count, TestNode(i, "hello"));
    }
    seg_vec_end = rdtsc();    
    double vec_cost;
    double seg_vec_cost;
    vec_cost = (vec_end - vec_begin)/double(CPU_FRE)/N;
    seg_vec_cost = (seg_vec_end - seg_vec_begin)/double(CPU_FRE)/N;
    std::cout << "vector assign " << count << " elems costs " << vec_cost << std::endl;
    std::cout << "segment_vector assign " << count << " elems costs " << seg_vec_cost << std::endl;  
}



int main()
{
    // test_push_back();
    // test_emplace_back();
    // test_resize();
    // test_traverse();
    // test_reserve();
    // test_pop_back();
    // test_assign_val();
    return 0;
}