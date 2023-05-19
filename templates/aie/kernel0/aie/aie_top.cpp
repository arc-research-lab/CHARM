#include "aie_top_L{{layer}}.h"
using namespace adf;

{% for i in range(A) -%}
{% for j in range(C//A_BRO) -%}
{% for k in range(B) -%}
{% set row = k+j*B+i*(C//A_BRO)*B -%}
PLIO* LHS_in{{row}}_L{{layer}} = new PLIO("LHS_in{{row}}_L{{layer}}", adf::plio_{{port_width}}_bits, "data/input0.txt",{{freq}});
{% endfor -%}
{% endfor -%}
{% endfor %}

{% for j in range(C) -%}
{% for i in range(A//C_BRO) -%}
{% for k in range(B) -%}
{% set col = k+i*B+j*(A//C_BRO)*B -%}
PLIO* RHS_in{{col}}_L{{layer}} = new PLIO("RHS_in{{col}}_L{{layer}}", adf::plio_{{port_width}}_bits, "data/input1.txt",{{freq}});
{% endfor -%}
{% endfor -%}
{% endfor %}


{% for i in range(A) -%}
{% for j in range(C) -%}
PLIO* out{{j+i*C}}_L{{layer}} = new PLIO("out{{j+i*C}}_L{{layer}}", adf::plio_{{port_width}}_bits, "data/output{{j+i*C}}.txt",{{freq}});
{% endfor -%}
{% endfor %}

simulation::platform<{{A*(C//A_BRO)*B+C*(A//C_BRO)*B}}, {{A*C}}> platform(
                      {% for i in range(A*(C//A_BRO)*B) -%} LHS_in{{i}}_L{{layer}}, {% endfor %}
                      {% for i in range(C*(A//C_BRO)*B) -%} RHS_in{{i}}_L{{layer}}, {% endfor %}
                      {% for i in range(A*C-1) -%} out{{i}}_L{{layer}}, {% endfor %}out{{A*C-1}}_L{{layer}});

vit_x{{A}}_x{{B}}_x{{C}}_graph{{layer}}  vit_graph{{layer}} ;

{% for i in range(A) -%}
{% for j in range(C//A_BRO) -%}
{% for k in range(B) -%}
{% set row = k+j*B+i*(C//A_BRO)*B -%}
connect<> net_lhs_in{{row}}_L{{layer}} (platform.src[{{row}}], vit_graph{{layer}}.in_lhs[{{j+i*(C//A_BRO)}}][{{k}}]);
{% endfor -%}
{% endfor -%}
{% endfor %}

{% for j in range(C) -%}
{% for i in range(A//C_BRO) -%}
{% for k in range(B) -%}
{% set col = k+i*B+j*(A//C_BRO)*B -%}
connect<> net_rhs_in{{col}}_L{{layer}} (platform.src[{{col + A*(C//A_BRO)*B}}], vit_graph{{layer}}.in_rhs[{{i+j*(A//C_BRO)}}][{{k}}]);
{% endfor -%}
{% endfor -%}
{% endfor %}

{% for i in range(A) -%}
{% for j in range(C) -%}
connect<> net_out{{j+i*C}}_L{{layer}} (vit_graph{{layer}}.out[{{j+i*C}}], platform.sink[{{j+i*C}}]);
{% endfor -%}
{% endfor %}

#ifdef __AIESIM__
int main(int argc, char** argv) {
    vit_graph{{layer}}.init();
    vit_graph{{layer}}.run(4);
    vit_graph{{layer}}.end();
    return 0;
}
#endif