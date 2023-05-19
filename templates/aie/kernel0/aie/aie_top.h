#include "aie_graph_L{{layer}}.h"

using namespace adf;

const int L{{layer}}_A={{A}};
const int L{{layer}}_B={{B}};
const int L{{layer}}_C={{C}};
const int L{{layer}}_A_BRO={{A_BRO}};
const int L{{layer}}_C_BRO={{C_BRO}};

class vit_x{{A}}_x{{B}}_x{{C}}_graph{{layer}} : public adf::graph {
public:
    input_port in_lhs[(L{{layer}}_A*L{{layer}}_C/L{{layer}}_A_BRO)][L{{layer}}_B];
    input_port in_rhs[(L{{layer}}_A*L{{layer}}_C/L{{layer}}_C_BRO)][L{{layer}}_B];
	output_port out[L{{layer}}_A*L{{layer}}_C];

    {% for i in range(A) -%}
    {% for j in range(C) -%}
    {% set col = pos_col + ((j+i*C)//height)*B + ((j+i*C)%2)*(B-1) + (A*B*C//height) -%}
    {% set row = pos_row + (j+i*C)%height -%}
    simpleGraph_k1_B{{B}}_L{{layer}} <{{col}}, {{row}}>  vit_{{i}}_{{j}};
    {% endfor -%}
    {% endfor %}

    vit_x{{A}}_x{{B}}_x{{C}}_graph{{layer}} () {

        {% for i in range(A) -%}
        {% for j in range(C) -%}
        {% for k in range(B) -%}
        {% set row = (C//A_BRO)*i+(j//A_BRO) -%}
        connect<stream,window<L{{layer}}_h1*L{{layer}}_w1>>(in_lhs[{{row}}][{{k}}],vit_{{i}}_{{j}}.in0[{{k}}]);
        {% endfor -%}
        {% endfor -%}
        {% endfor %}

        {% for j in range(C) -%}
        {% for i in range(A) -%}
        {% for k in range(B) -%}
        {% set col = (A//C_BRO)*j+(i//C_BRO) -%}
        connect<stream,window<L{{layer}}_w1*L{{layer}}_w2>>(in_rhs[{{col}}][{{k}}],vit_{{i}}_{{j}}.in1[{{k}}]);
        {% endfor -%}
        {% endfor -%}
        {% endfor %}


        {% for i in range(A) -%}
        {% for j in range(C) -%}
        connect<stream,stream>(vit_{{i}}_{{j}}.out,out[{{j+i*C}}]);
        {% endfor -%}
        {% endfor %}
    }
};