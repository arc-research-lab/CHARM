#ifndef __GRAPH_L{{layer}}_H__
#define __GRAPH_L{{layer}}_H__

#include <adf.h>
#include "para_L{{layer}}.h"
#include <stdio.h>
using namespace adf;


template <int COL_OFFSET, int ROW_OFFSET>
class simpleGraph_k1_B{{B}}_L{{layer}}: public graph {
  private:
    kernel mm[L{{layer}}_NUM_KERNEL];

  public:
    port<input> in0[L{{layer}}_NUM_KERNEL],in1[L{{layer}}_NUM_KERNEL];
    port<output> out;
    
    simpleGraph_k1_B{{B}}_L{{layer}}() {
      {% if B ==1 %}
      mm[0] = kernel::create(mm0_kernel3_L{{layer}});
      source(mm[0]) = "mm0_kernel3_L{{layer}}.cc";
      runtime<ratio>(mm[0]) = 1;
      connect<window<L{{layer}}_h1*L{{layer}}_w1*4>>(in0[0], mm[0].in[0]);
      connect<window<L{{layer}}_w1*L{{layer}}_w2*4>>(in1[0], mm[0].in[1]);
      connect<window<L{{layer}}_h1*L{{layer}}_w2*4>>(mm[0].out[0], out);
      adf::location<kernel>(mm[0]) = adf::tile(COL_OFFSET,ROW_OFFSET);
      {% elif B > 1 %}
      for (int i=0; i<L{{layer}}_NUM_KERNEL;i++){
        if(i==0){
          mm[i] = kernel::create(mm0_kernel0_L{{layer}});
          source(mm[i]) = "mm0_kernel0_L{{layer}}.cc";
        }
        else if(i==L{{layer}}_NUM_KERNEL-1){
          mm[i] = kernel::create(mm0_kernel2_L{{layer}});
          source(mm[i]) = "mm0_kernel2_L{{layer}}.cc";
        }
        {% if B > 2 %}
        else{
          mm[i] = kernel::create(mm0_kernel1_L{{layer}});
          source(mm[i]) = "mm0_kernel1_L{{layer}}.cc";
        }
        {% endif %}
        runtime<ratio>(mm[i]) = 1;
        if ((ROW_OFFSET%2)==0){
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET+i,ROW_OFFSET);
        }
        else{
          adf::location<kernel>(mm[i]) = adf::tile(COL_OFFSET-i,ROW_OFFSET);
        }
      }

      for (int i=0; i<L{{layer}}_NUM_KERNEL;i++){
        if ((ROW_OFFSET%2)==0){
          location<buffer>(mm[i].in[0]) =
          { address(COL_OFFSET+i, ROW_OFFSET+1, 0x0000), 
            address(COL_OFFSET+i, ROW_OFFSET+1, 0x2000)};
          location<buffer>(mm[i].in[1]) =
          { address(COL_OFFSET+i, ROW_OFFSET, 0x4000), 
            address(COL_OFFSET+i, ROW_OFFSET, 0x6000)};
          if(i==L{{layer}}_NUM_KERNEL-1){
            location<buffer>(mm[i].out[0]) =
            { address(COL_OFFSET+i-1, ROW_OFFSET, 0x5000), 
              address(COL_OFFSET+i-1, ROW_OFFSET, 0x7000)};
          }  
          location<stack>(mm[i]) = address(COL_OFFSET+i, ROW_OFFSET, 0x3000);
        }
        else{
          location<buffer>(mm[i].in[0]) =
          { address(COL_OFFSET-i, ROW_OFFSET-1, 0x0000), 
            address(COL_OFFSET-i, ROW_OFFSET-1, 0x2000)};
          location<buffer>(mm[i].in[1]) =
          { address(COL_OFFSET-i, ROW_OFFSET, 0x4000), 
            address(COL_OFFSET-i, ROW_OFFSET, 0x6000)};
          if(i==L{{layer}}_NUM_KERNEL-1){
            location<buffer>(mm[i].out[0]) =
            { address(COL_OFFSET-i+1, ROW_OFFSET, 0x5000), 
              address(COL_OFFSET-i+1, ROW_OFFSET, 0x7000)};
          }
          location<stack>(mm[i]) = address(COL_OFFSET-i, ROW_OFFSET, 0x3000);
        }
      }


      for (int i=0; i<L{{layer}}_NUM_KERNEL;i++){
        connect<window<L{{layer}}_h1*L{{layer}}_w1*4>>(in0[i], mm[i].in[0]);
        connect<window<L{{layer}}_w1*L{{layer}}_w2*4>>(in1[i], mm[i].in[1]);
      }

      for (int i=0; i<L{{layer}}_NUM_KERNEL;i++){
        if(i==L{{layer}}_NUM_KERNEL-1){
          connect<window<L{{layer}}_h1*L{{layer}}_w2*4>>(mm[i].out[0], out);
        }
        else{
          connect<cascade>(mm[i].out[0], mm[i+1].in[2]);
        }
      }
      {% endif %}

    };
    
};

#endif /**********__GRAPH_H__**********/