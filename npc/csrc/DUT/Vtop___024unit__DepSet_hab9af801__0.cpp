// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "verilated.h"
#include "verilated_dpi.h"

#include "Vtop__Syms.h"
#include "Vtop___024unit.h"

extern "C" void get_inst(int* inst);

VL_INLINE_OPT void Vtop___024unit____Vdpiimwrap_get_inst_TOP____024unit(IData/*31:0*/ &inst) {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vtop___024unit____Vdpiimwrap_get_inst_TOP____024unit\n"); );
    // Body
    int inst__Vcvt;
    get_inst(&inst__Vcvt);
    inst = inst__Vcvt;
}

extern "C" void ebreak();

VL_INLINE_OPT void Vtop___024unit____Vdpiimwrap_ebreak_TOP____024unit() {
    VL_DEBUG_IF(VL_DBG_MSGF("+        Vtop___024unit____Vdpiimwrap_ebreak_TOP____024unit\n"); );
    // Body
    ebreak();
}
