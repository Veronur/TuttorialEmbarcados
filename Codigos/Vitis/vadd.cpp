/**********
Copyright (c) 2020, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

/*******************************************************************************
Description:
    HLS pragmas can be used to optimize the design : improve throughput, reduce
latency and
    device resource utilization of the resulting RTL code
    This is vector addition example to demonstrate how HLS optimizations are
used in kernel.
*******************************************************************************/

#include <cmath>

#define BUFFER_SIZE 256
#define DATA_SIZE 1024

// TRIPCOUNT identifier
const unsigned int c_len = DATA_SIZE / BUFFER_SIZE;
const unsigned int c_size = BUFFER_SIZE;

#define M_PI 3.14159265358979323846
#define SIG 1.0
#define MU 0.0

/*
    Vector Addition Kernel Implementation
    Arguments:
        in1   (input)     --> Input Vector1
        in2   (input)     --> Input Vector2
        out_r   (output)    --> Output Vector
        size  (input)     --> Size of Vector in Integer
*/

double q(double x) { //Gaussian
  return (double) ((1.0/(sqrt(2.0*M_PI*pow(SIG,2.0)))*(exp(-(pow(x-MU,2.0)))/(2.0*pow(SIG,2.0)))));
}

extern "C" {
void vadd(const double *random1, // Read-Only Vector 1
          const double *random2, // Read-Only Vector 2
          double *out_r,     // Output Result
          int n_elements                 // Size in integer
          ) {

  double p, r;

  for (int i = 0 ; i < n_elements ; i += 1) {
  #pragma HLS LOOP_TRIPCOUNT min = c_len max = c_len
        
    double rn = r + random1[i]; //random value from -1 to 1
    double pn = q(rn);
    if (pn >= p) {
            p = pn;
            r = rn;
    } else {
            double u = random2[i]; //random value from 0 to 1
            if(u < (double) (pn/p)) {
                    p = pn;
                    r = rn;
            }
    }
    out_r[i] = r;
  }
}
}
