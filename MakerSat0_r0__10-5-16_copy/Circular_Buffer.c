/*
 * Circular_Buffer.c
 *
 *  Created on: Aug 10, 2016
 *      Author: Dr. Griffin
 */
#include <msp430fr6989.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Circular_Buffer.h"

#define MAXBUFFERSIZE 500

uint16_t g_buffer[MAXBUFFERSIZE];

int16_t g_begin = 0;
int16_t g_end = MAXBUFFERSIZE-1;
int16_t g_top;
int16_t g_bottom;
bool g_bufferEmpty;
bool g_bufferFull;

void init_Buffer(void){
    g_top = 0;
    g_bottom = 0;
    g_bufferFull = false;
    g_bufferEmpty = false;
}

void write_Buffer(uint8_t data) {
    g_buffer[g_bottom] = data;
    g_bottom++;
    if(g_bottom > g_end) {
        g_bottom = g_begin;
    }
    if(g_top == g_bottom){
        g_bufferFull = true;
    } else {
        g_bufferFull = false;
    }
}

void write_Buffer_Element(uint8_t data, uint8_t ele_Num) {
    g_buffer[ele_Num] = data;
}

uint8_t read_Buffer(void) {
    uint8_t readVal = g_buffer[g_top];
    g_top++;
    if(g_top > g_end) {
        g_top = g_begin;
    }
    if(g_top == g_bottom){
        g_bufferEmpty = true;
    } else {
        g_bufferEmpty = false;
    }
    return readVal;
}

uint8_t read_Buffer_Element(uint8_t ele_Num) {
    uint8_t readVal = g_buffer[ele_Num];
    return readVal;
}

bool is_Buffer_Full(void) {
    return g_bufferFull;
}

bool is_Buffer_Empty(void) {
    return g_bufferEmpty;
}
