//
//  apb_load_file_easy.c
//  Stochastik
//
//  Created by Aaron Blohowiak on 12/14/11.
//  Copyright (c) 2011 none. All rights reserved.
//

#include "apb_load_file_easy.h"


uint32_t apb_load_file_easyStereo(char * filename, float ** leftChannel, float ** rightChannel){
    //turn a char* into a CFURLRef
    CFURLRef sndFile = CFURLCreateFromFileSystemRepresentation(NULL, (UInt8 *)filename, strlen(filename), false);
//    printf("loading file %s\n", filename);
    uint32_t num_frames = apb_load_file_easyStereoCFURL( sndFile, leftChannel, rightChannel);
    CFRelease(sndFile);
    return num_frames;
}


uint32_t apb_load_file_easyStereoCFURL(CFURLRef  sndFile, float ** leftChannel, float ** rightChannel){
    
    
    //Create an audio file proxy object
    ExtAudioFileRef eaf;
    
    OSStatus err = ExtAudioFileOpenURL((CFURLRef)sndFile, &eaf);
    
    //not very robust error handling ^.^
    if(err != 0){
        
        printf("could not open file. errno: %ld", err);
        return(-1);
    }
    
	const int four_bytes_per_float = 4;
	const int eight_bits_per_byte = 8;
	AudioStreamBasicDescription streamFormat;
	streamFormat.mSampleRate = 44100;
	streamFormat.mFormatID = kAudioFormatLinearPCM;
	streamFormat.mFormatFlags =
    kAudioFormatFlagsNativeFloatPacked | kAudioFormatFlagIsNonInterleaved;
	streamFormat.mBytesPerPacket = four_bytes_per_float;
	streamFormat.mFramesPerPacket = 1;	
	streamFormat.mBytesPerFrame = four_bytes_per_float;		
	streamFormat.mChannelsPerFrame = 2;	
	streamFormat.mBitsPerChannel = four_bytes_per_float * eight_bits_per_byte;
	
    err = ExtAudioFileSetProperty(eaf, kExtAudioFileProperty_ClientDataFormat, sizeof(streamFormat), &streamFormat);
    
    if(err != 0){
        printf("could not set file property. errno: %ld", err);
        return(-1);
    }
    
    UInt32 property_size = sizeof(UInt64);
    
    UInt64 frame_count;
    
    err = ExtAudioFileGetProperty (
                                   eaf,
                                   kExtAudioFileProperty_FileLengthFrames,
                                   &property_size,
                                   &frame_count
                                   );
    
    if(err != 0){
        printf("could not get file length in frames. errno: %ld", err);
        return(-1);
    }
    
    UInt32 num_frames = (uint32_t) frame_count;
    
    AudioBufferList * abl = malloc(sizeof(AudioBufferList) + 2 * sizeof(sizeof abl->mBuffers[0]));
    abl -> mNumberBuffers = 2;
    //left channel
    abl->mBuffers[0].mNumberChannels = 1;
    abl->mBuffers[0].mDataByteSize = num_frames * 4;
    abl->mBuffers[0].mData = malloc(num_frames * 4);
    //right channel
    abl->mBuffers[1].mNumberChannels = 1;
    abl->mBuffers[1].mDataByteSize = num_frames * 4;
    abl->mBuffers[1].mData = malloc(num_frames * 4);
    
    err = ExtAudioFileRead (eaf, &num_frames, abl);
    
    if(err != 0){
        printf("could not read file. errno: %ld", err);
        return(-1);
    }

    *leftChannel = (float *) abl->mBuffers[0].mData;
    *rightChannel = (float *) abl->mBuffers[1].mData;
    
    //clean up!
    ExtAudioFileDispose(eaf);
    free(abl);

    return num_frames;
}
