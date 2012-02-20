#include <Accelerate/Accelerate.h>
#include <mach/mach_time.h>

#include <stdint.h>
#include "apb_load_file_easy.h"


void perform_work_accelerated(uint32_t len_a, float * left_a, float * right_a, 
							 uint32_t len_b, float * left_b, float * right_b,
							 uint32_t len_dest, float * left_dest, float * right_dest);


void perform_work_naive(uint32_t len_a, float * left_a, float * right_a, 
							 uint32_t len_b, float * left_b, float * right_b,
							 uint32_t len_dest, float * left_dest, float * right_dest);


void adjust_gain_naive(float amount, uint32_t len, float * source, float * dest);
void adjust_gain_accelerated(float amount, uint32_t len, float * source, float * dest);


void add_channels_naive(uint32_t len, float * source_a, float * source_b, float * dest);
void add_channels_accelerated(uint32_t len, float * source_a, float * source_b, float * dest);

void clip_accelerated(float * src, float * dst, float lower, float upper, uint32_t length);
void clip_naive(float * src, float * dst, float lower, float upper, uint32_t length);

long double benchmark(uint64_t * results_naive, uint64_t * results_accelerated, size_t runs, char * track_a, char * track_b);

long double write_benchmark_to_file(char * filename, char * track_a, char * track_b, size_t runs);