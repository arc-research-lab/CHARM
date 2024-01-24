#include "aie_graph_L{{layer}}.h"

using namespace adf;

const int L{{layer}}_A={{A}};
const int L{{layer}}_B={{B}};
const int L{{layer}}_C={{C}};
const int L{{layer}}_A_BRO={{A_BRO}};
const int L{{layer}}_C_BRO={{C_BRO}};

class mm_x{{A}}_x{{B}}_x{{C}}_graph{{layer}} : public adf::graph {
public:
    input_port in_lhs[(L{{layer}}_A*L{{layer}}_C/L{{layer}}_A_BRO)][L{{layer}}_B];
    input_port in_rhs[(L{{layer}}_A*L{{layer}}_C/L{{layer}}_C_BRO)][L{{layer}}_B];
	output_port out[L{{layer}}_A*L{{layer}}_C];

    {# Two types of Placement: when c_a_bro is even, C dim will be interleved for cascade #}
    {% set c_a_bro = C//A_BRO -%}
    {% set c_div = C//2 -%}
    {% set c_left = C%2 -%}

    {% for i in range(A) -%}
    {% for j in range(C) -%}
    {% set col = pos_col + ((j+i*C)//height)*B + (((j+i*C)%height)%2)*(B-1) -%}
    {% set row = pos_row + (j+i*C)%height -%}
    {% if c_a_bro%2==0 and height%2==0 -%}
    {% set j_div = j//2 -%}
    {% set j_left = j%2 -%}
    {% if j_left==0 -%}
    {% set j_pos = j_div -%}
    {% else -%}
    {% set j_pos = j_div + c_div + c_left -%}
    {% endif -%}
    mm_k6_B{{B}}_L{{layer}} <{{col}}, {{row}}>  mm_{{i}}_{{j_pos}};
    {% else -%}
    mm_k6_B{{B}}_L{{layer}} <{{col}}, {{row}}>  mm_{{i}}_{{j}};
    {% endif -%}
    {% endfor -%}
    {% endfor %}



    mm_x{{A}}_x{{B}}_x{{C}}_graph{{layer}} () {

        {% for i in range(A) -%}
        {% for j in range(C) -%}
        {% for k in range(B//PACK_IN) -%}
        {% set row = (C//A_BRO)*i+ (j//A_BRO)-%}
        connect<stream,window<L{{layer}}_h1*L{{layer}}_w1*L{{layer}}_Byte>>(in_lhs[{{row}}][{{k}}],mm_{{i}}_{{j}}.in0[{{k}}]);
        {% endfor -%}
        {% endfor -%}
        {% endfor %}

        {% for j in range(C) -%}
        {% for i in range(A) -%}
        {% for k in range(B//PACK_IN) -%}
        {% set col = (A//C_BRO)*j+(i//C_BRO) -%}
        connect<stream,window<L{{layer}}_w1*L{{layer}}_w2*L{{layer}}_Byte>>(in_rhs[{{col}}][{{k}}],mm_{{i}}_{{j}}.in1[{{k}}]);
        {% endfor -%}
        {% endfor -%}
        {% endfor %}

        {% for i in range(A) -%}
        {% for j in range(C) -%}
        connect<window<L{{layer}}_h1*L{{layer}}_w2*L{{layer}}_Byte>,stream>(mm_{{i}}_{{j}}.out,out[{{j+i*C}}]);
        {% endfor -%}
        {% endfor %}
    }
};