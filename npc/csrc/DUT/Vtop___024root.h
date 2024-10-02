// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP___024ROOT_H_
#define VERILATED_VTOP___024ROOT_H_  // guard

#include "verilated.h"

class Vtop__Syms;
class Vtop___024unit;


class Vtop___024root final : public VerilatedModule {
  public:
    // CELLS
    Vtop___024unit* __PVT____024unit;

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    CData/*2:0*/ top__DOT__ExtOP;
    CData/*0:0*/ top__DOT__RegWr;
    CData/*0:0*/ top__DOT__ALUAsrc;
    CData/*1:0*/ top__DOT__ALUBsrc;
    CData/*3:0*/ top__DOT__ALUctr;
    CData/*2:0*/ top__DOT__Branch;
    CData/*0:0*/ top__DOT__MemtoReg;
    CData/*0:0*/ __Vtrigrprev__TOP__clk;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ top__DOT__NextPC;
    IData/*31:0*/ top__DOT__pc;
    IData/*31:0*/ top__DOT__reg_src1;
    IData/*31:0*/ top__DOT__reg_src2;
    IData/*31:0*/ top__DOT____Vcellinp__gpr__busW;
    IData/*31:0*/ top__DOT__inst;
    IData/*31:0*/ top__DOT__imm;
    IData/*31:0*/ top__DOT__ALUout;
    IData/*31:0*/ top__DOT____Vcellinp__alu__B;
    IData/*31:0*/ top__DOT____Vcellinp__alu__A;
    IData/*31:0*/ top__DOT__mux_next_pc__DOT__i0__DOT__lut_out;
    IData/*31:0*/ top__DOT__idu__DOT__immI;
    IData/*31:0*/ top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out;
    IData/*31:0*/ top__DOT__alu__DOT__adder;
    IData/*31:0*/ top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__lut_out;
    IData/*31:0*/ top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out;
    IData/*31:0*/ __Vtask_get_inst__0__inst;
    IData/*31:0*/ __VstlIterCount;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<CData/*2:0*/, 3> top__DOT__mux_next_pc__DOT__i0__DOT__key_list;
    VlUnpacked<IData/*31:0*/, 3> top__DOT__mux_next_pc__DOT__i0__DOT__data_list;
    VlUnpacked<IData/*31:0*/, 32> top__DOT__gpr__DOT__gpr;
    VlUnpacked<CData/*2:0*/, 5> top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list;
    VlUnpacked<IData/*31:0*/, 5> top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list;
    VlUnpacked<CData/*0:0*/, 2> top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__key_list;
    VlUnpacked<IData/*31:0*/, 2> top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__data_list;
    VlUnpacked<CData/*2:0*/, 8> top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list;
    VlUnpacked<IData/*31:0*/, 8> top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vtop__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtop___024root(Vtop__Syms* symsp, const char* v__name);
    ~Vtop___024root();
    VL_UNCOPYABLE(Vtop___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);


#endif  // guard
