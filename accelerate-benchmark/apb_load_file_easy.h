/*
 Easy audio file to float loading!
 
 This works with practically any kind of source audio, automatically.
 
 You pass in the filename and you get two arrays of floats with the
 audio data as floats, and the size of those arrays, as the result.
 
 Example:
 
 filename[1024] = "hello.mp3";
 float * left;
 float * right;
 uint32_t num_frames = apb_load_file_easyStereo(filename, &left, &right);
 
 // Now, left[0] to left[num_frames - 1]
 // contains the left channel for the audio stored in "hello.mp3";
 
 */

#ifndef apb_load_file_easy_h
#define apb_load_file_easy_h

#include "string.h"
#include "stdio.h"

#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

uint32_t apb_load_file_easyStereo(char * filename, float ** leftChannel, float ** rightChannel);
uint32_t apb_load_file_easyStereoCFURL(CFURLRef  filename, float ** leftChannel, float ** rightChannel);

#endif