#include "XitiveBenchmark.h"

void perform_work_accelerated(uint32_t len_a, float * left_a, float * right_a, 
							 uint32_t len_b, float * left_b, float * right_b,
							 uint32_t len_dest, float * left_dest, float * right_dest){
	
	uint32_t min_len = len_a < len_b ? len_a : len_b;
	adjust_gain_accelerated(0.5, len_a, left_a, left_dest);
	adjust_gain_accelerated(0.5, len_b, left_b, left_dest);
	add_channels_accelerated(min_len, left_a, left_b, left_dest);
	clip_accelerated(left_dest, left_dest, -1, 1, len_dest);
}


void perform_work_naive(uint32_t len_a, float * left_a, float * right_a, 
							 uint32_t len_b, float * left_b, float * right_b,
							 uint32_t len_dest, float * left_dest, float * right_dest){

	uint32_t min_len = len_a < len_b ? len_a : len_b;
	adjust_gain_naive(0.5, len_a, left_a, left_dest);
	adjust_gain_naive(0.5, len_b, left_b, left_dest);
	add_channels_naive(min_len, left_a, left_b, left_dest);
	clip_naive(left_dest, left_dest, -1, 1, len_dest);
}


void clip_naive(float * src, float * dst, float lower, float upper, uint32_t length){

	for (uint32_t i = 0; i < length; ++i)
	{
		if(src[i] < lower){
			dst[i] = lower;
			continue;
		}

		if(src[i] > upper){
			dst[i] = upper;
			continue;
		}

		dst[i] = src[i];
	}
}

void clip_accelerated(float * src, float * dst, float lower, float upper, uint32_t length){
	vDSP_vclip(src, 1, &lower, &upper, dst, 1, length);
}


void add_channels_naive(uint32_t len, float * source_a, float * source_b, float * dest){
	for (uint32_t i = 0; i < len; ++i)
	{
		dest[i] = source_a[i] + source_b[i];
	}
}

void add_channels_accelerated(uint32_t len, float * source_a, float * source_b, float * dest){
	vDSP_vadd(source_a, 1, source_b, 1, dest, 1, len);
}


void adjust_gain_naive(float amount, uint32_t len, float * source, float * dest){
	for (uint32_t i = 0; i < len; ++i)
	{
		dest[i] = source[i] * amount;
	}
}

void adjust_gain_accelerated(float amount, uint32_t len, float * source, float * dest){
	vDSP_vsmul ( source, 1, &amount, dest, 1, len );
}


long double benchmark(uint64_t * results_naive, uint64_t * results_accelerated, size_t runs, char * track_a, char * track_b){
	uint64_t end_time, start_time, accelerate_time, naive_time;

	float * left_a, *left_b;
	float * right_a, *right_b;
	uint32_t len_a, len_b;

	len_a = apb_load_file_easyStereo(track_a, &left_a, &right_a);
	len_b = apb_load_file_easyStereo(track_b, &left_b, &right_b);


	uint32_t len_dest = len_a < len_b ? len_b : len_a;

	float * left_dest = calloc(len_dest, sizeof(float));
	float * right_dest = calloc(len_dest, sizeof(float));

	long double result = 0;

	for (int i = 0; i < runs; ++i)
	{
		start_time = mach_absolute_time();
		perform_work_accelerated(len_a, left_a, right_a, len_b, left_b, right_b, len_dest, left_dest, right_dest);
		end_time = mach_absolute_time();

		accelerate_time = end_time - start_time;
		results_accelerated[i] = accelerate_time;

		start_time = mach_absolute_time();
		perform_work_naive(len_a, left_a, right_a, len_b, left_b, right_b, len_dest, left_dest, right_dest);
		end_time = mach_absolute_time();

		naive_time = end_time - start_time;
		results_naive[i] = naive_time;

		result =  result + ((long double)naive_time/(long double)accelerate_time);
	}

	result = result / runs;
	return result;
}

long double write_benchmark_to_file(char * filename, char * track_a, char * track_b, size_t runs){
	
    uint64_t * results_naive = calloc(runs, sizeof(uint64_t));
	uint64_t * results_accelerated = calloc(runs, sizeof(uint64_t));

	long double result = benchmark(results_naive, results_accelerated, runs, track_a, track_b);

	FILE * file = fopen(filename, "w");
	for (int i = 0; i < runs; ++i)
	{
		fprintf(file, "%llu, %llu\n", results_naive[i], results_accelerated[i]);
	}
	fclose(file);

	return result;
}