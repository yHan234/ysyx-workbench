// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "verilated.h"
#include "verilated_dpi.h"

#include "Vtop__Syms.h"
#include "Vtop___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf);
#endif  // VL_DEBUG

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers__act\n"); );
    // Body
    vlSelf->__VactTriggered.at(0U) = ((IData)(vlSelf->clk) 
                                      & (~ (IData)(vlSelf->__Vtrigrprev__TOP__clk)));
    vlSelf->__Vtrigrprev__TOP__clk = vlSelf->clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__act(vlSelf);
    }
#endif
}

void Vtop___024unit____Vdpiimwrap_get_inst_TOP____024unit(IData/*31:0*/ &inst);
void Vtop___024unit____Vdpiimwrap_ebreak_TOP____024unit();

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*0:0*/ __Vdlyvset__top__DOT__gpr__DOT__gpr__v0;
    __Vdlyvset__top__DOT__gpr__DOT__gpr__v0 = 0;
    CData/*4:0*/ __Vdlyvdim0__top__DOT__gpr__DOT__gpr__v32;
    __Vdlyvdim0__top__DOT__gpr__DOT__gpr__v32 = 0;
    IData/*31:0*/ __Vdlyvval__top__DOT__gpr__DOT__gpr__v32;
    __Vdlyvval__top__DOT__gpr__DOT__gpr__v32 = 0;
    CData/*0:0*/ __Vdlyvset__top__DOT__gpr__DOT__gpr__v32;
    __Vdlyvset__top__DOT__gpr__DOT__gpr__v32 = 0;
    // Body
    __Vdlyvset__top__DOT__gpr__DOT__gpr__v0 = 0U;
    __Vdlyvset__top__DOT__gpr__DOT__gpr__v32 = 0U;
    VL_WRITEF("regs:\n          0 = %10# |          1 = %10# |          2 = %10# |          3 = %10#\n          4 = %10# |          5 = %10# |          6 = %10# |          7 = %10#\n          8 = %10# |          9 = %10# |         10 = %10# |         11 = %10#\n         12 = %10# |         13 = %10# |         14 = %10# |         15 = %10#\n         16 = %10# |         17 = %10# |         18 = %10# |         19 = %10#\n         20 = %10# |         21 = %10# |         22 = %10# |         23 = %10#\n",
              32,vlSelf->top__DOT__gpr__DOT__gpr[0U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[1U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[2U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[3U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[4U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[5U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[6U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[7U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[8U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[9U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0xaU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0xbU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0xcU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0xdU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0xeU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0xfU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x10U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x11U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x12U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x13U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x14U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x15U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x16U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x17U]);
    VL_WRITEF("         24 = %10# |         25 = %10# |         26 = %10# |         27 = %10#\n         28 = %10# |         29 = %10# |         30 = %10# |         31 = %10#\n",
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x18U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x19U],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x1aU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x1bU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x1cU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x1dU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x1eU],
              32,vlSelf->top__DOT__gpr__DOT__gpr[0x1fU]);
    if (vlSelf->rst) {
        __Vdlyvset__top__DOT__gpr__DOT__gpr__v0 = 1U;
    } else if (VL_UNLIKELY(vlSelf->top__DOT__RegWr)) {
        VL_WRITEF("store %10# to register %2# which contains %10#\n",
                  32,vlSelf->top__DOT____Vcellinp__gpr__busW,
                  5,(0x1fU & (vlSelf->top__DOT__inst 
                              >> 7U)),32,vlSelf->top__DOT__gpr__DOT__gpr
                  [(0x1fU & (vlSelf->top__DOT__inst 
                             >> 7U))]);
        __Vdlyvval__top__DOT__gpr__DOT__gpr__v32 = vlSelf->top__DOT____Vcellinp__gpr__busW;
        __Vdlyvset__top__DOT__gpr__DOT__gpr__v32 = 1U;
        __Vdlyvdim0__top__DOT__gpr__DOT__gpr__v32 = 
            (0x1fU & (vlSelf->top__DOT__inst >> 7U));
    }
    vlSelf->top__DOT__pc = ((IData)(vlSelf->rst) ? 0x80000000U
                             : vlSelf->top__DOT__NextPC);
    if (__Vdlyvset__top__DOT__gpr__DOT__gpr__v0) {
        vlSelf->top__DOT__gpr__DOT__gpr[0U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[1U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[2U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[3U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[4U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[5U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[6U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[7U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[8U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[9U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0xaU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0xbU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0xcU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0xdU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0xeU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0xfU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x10U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x11U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x12U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x13U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x14U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x15U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x16U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x17U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x18U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x19U] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x1aU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x1bU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x1cU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x1dU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x1eU] = 0U;
        vlSelf->top__DOT__gpr__DOT__gpr[0x1fU] = 0U;
    }
    if (__Vdlyvset__top__DOT__gpr__DOT__gpr__v32) {
        vlSelf->top__DOT__gpr__DOT__gpr[__Vdlyvdim0__top__DOT__gpr__DOT__gpr__v32] 
            = __Vdlyvval__top__DOT__gpr__DOT__gpr__v32;
    }
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__data_list[2U] 
        = ((IData)(4U) + vlSelf->top__DOT__pc);
    Vtop___024unit____Vdpiimwrap_get_inst_TOP____024unit(vlSelf->__Vtask_get_inst__0__inst);
    vlSelf->top__DOT__inst = vlSelf->__Vtask_get_inst__0__inst;
    VL_WRITEF("PC: %10# inst: %b\n",32,vlSelf->top__DOT__pc,
              32,vlSelf->top__DOT__inst);
    if ((0x100073U == vlSelf->top__DOT__inst)) {
        Vtop___024unit____Vdpiimwrap_ebreak_TOP____024unit();
    }
    vlSelf->top__DOT__RegWr = 0U;
    vlSelf->top__DOT__idu__DOT__immI = (((- (IData)(
                                                    (vlSelf->top__DOT__inst 
                                                     >> 0x1fU))) 
                                         << 0xcU) | 
                                        (vlSelf->top__DOT__inst 
                                         >> 0x14U));
    vlSelf->top__DOT__MemtoReg = 0U;
    vlSelf->top__DOT__Branch = 0U;
    vlSelf->top__DOT__ALUctr = 0U;
    vlSelf->top__DOT__ALUAsrc = 0U;
    vlSelf->top__DOT__reg_src1 = ((0U == (0x1fU & (vlSelf->top__DOT__inst 
                                                   >> 0xfU)))
                                   ? 0U : vlSelf->top__DOT__gpr__DOT__gpr
                                  [(0x1fU & (vlSelf->top__DOT__inst 
                                             >> 0xfU))]);
    vlSelf->top__DOT__ALUBsrc = 0U;
    vlSelf->top__DOT__reg_src2 = ((0U == (0x1fU & (vlSelf->top__DOT__inst 
                                                   >> 0x14U)))
                                   ? 0U : vlSelf->top__DOT__gpr__DOT__gpr
                                  [(0x1fU & (vlSelf->top__DOT__inst 
                                             >> 0x14U))]);
    vlSelf->top__DOT__ExtOP = 0U;
    if ((0x40U & vlSelf->top__DOT__inst)) {
        if ((0x20U & vlSelf->top__DOT__inst)) {
            if ((1U & (~ (vlSelf->top__DOT__inst >> 4U)))) {
                if ((8U & vlSelf->top__DOT__inst)) {
                    if ((4U & vlSelf->top__DOT__inst)) {
                        if ((2U & vlSelf->top__DOT__inst)) {
                            if ((1U & vlSelf->top__DOT__inst)) {
                                vlSelf->top__DOT__RegWr = 1U;
                                vlSelf->top__DOT__MemtoReg = 0U;
                                vlSelf->top__DOT__Branch = 1U;
                                vlSelf->top__DOT__ALUctr = 0U;
                                vlSelf->top__DOT__ALUAsrc = 1U;
                                vlSelf->top__DOT__ALUBsrc = 2U;
                                vlSelf->top__DOT__ExtOP = 4U;
                            }
                        }
                    }
                } else if ((4U & vlSelf->top__DOT__inst)) {
                    if ((2U & vlSelf->top__DOT__inst)) {
                        if ((1U & vlSelf->top__DOT__inst)) {
                            if ((0U == (7U & (vlSelf->top__DOT__inst 
                                              >> 0xcU)))) {
                                vlSelf->top__DOT__RegWr = 1U;
                                vlSelf->top__DOT__MemtoReg = 0U;
                                vlSelf->top__DOT__Branch = 2U;
                                vlSelf->top__DOT__ALUctr = 0U;
                                vlSelf->top__DOT__ALUAsrc = 1U;
                                vlSelf->top__DOT__ALUBsrc = 2U;
                                vlSelf->top__DOT__ExtOP = 0U;
                            }
                        }
                    }
                }
            }
        }
    } else if ((0x20U & vlSelf->top__DOT__inst)) {
        if ((0x10U & vlSelf->top__DOT__inst)) {
            if ((1U & (~ (vlSelf->top__DOT__inst >> 3U)))) {
                if ((4U & vlSelf->top__DOT__inst)) {
                    if ((2U & vlSelf->top__DOT__inst)) {
                        if ((1U & vlSelf->top__DOT__inst)) {
                            vlSelf->top__DOT__RegWr = 1U;
                            vlSelf->top__DOT__MemtoReg = 0U;
                            vlSelf->top__DOT__Branch = 0U;
                            vlSelf->top__DOT__ALUctr = 3U;
                            vlSelf->top__DOT__ALUAsrc = 1U;
                            vlSelf->top__DOT__ALUBsrc = 1U;
                            vlSelf->top__DOT__ExtOP = 1U;
                        }
                    }
                }
            }
        }
    } else if ((0x10U & vlSelf->top__DOT__inst)) {
        if ((1U & (~ (vlSelf->top__DOT__inst >> 3U)))) {
            if ((4U & vlSelf->top__DOT__inst)) {
                if ((2U & vlSelf->top__DOT__inst)) {
                    if ((1U & vlSelf->top__DOT__inst)) {
                        vlSelf->top__DOT__RegWr = 1U;
                        vlSelf->top__DOT__MemtoReg = 0U;
                        vlSelf->top__DOT__Branch = 0U;
                        vlSelf->top__DOT__ALUctr = 0U;
                        vlSelf->top__DOT__ALUAsrc = 1U;
                        vlSelf->top__DOT__ALUBsrc = 1U;
                        vlSelf->top__DOT__ExtOP = 1U;
                    }
                }
            } else if ((2U & vlSelf->top__DOT__inst)) {
                if ((1U & vlSelf->top__DOT__inst)) {
                    if ((0U == (7U & (vlSelf->top__DOT__inst 
                                      >> 0xcU)))) {
                        vlSelf->top__DOT__RegWr = 1U;
                        vlSelf->top__DOT__MemtoReg = 0U;
                        vlSelf->top__DOT__Branch = 0U;
                        vlSelf->top__DOT__ALUctr = 0U;
                        vlSelf->top__DOT__ALUAsrc = 0U;
                        vlSelf->top__DOT__ALUBsrc = 1U;
                        vlSelf->top__DOT__ExtOP = 0U;
                    }
                }
            }
        }
    }
    VL_WRITEF("op: %3#\nExtOP: %1#\n",7,(0x7fU & vlSelf->top__DOT__inst),
              3,(IData)(vlSelf->top__DOT__ExtOP));
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list[0U] 
        = (((- (IData)((vlSelf->top__DOT__inst >> 0x1fU))) 
            << 0x14U) | ((0xff000U & vlSelf->top__DOT__inst) 
                         | ((0x800U & (vlSelf->top__DOT__inst 
                                       >> 9U)) | (0x7feU 
                                                  & (vlSelf->top__DOT__inst 
                                                     >> 0x14U)))));
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list[1U] 
        = (((- (IData)((vlSelf->top__DOT__inst >> 0x1fU))) 
            << 0xcU) | ((0x800U & (vlSelf->top__DOT__inst 
                                   << 4U)) | ((0x7e0U 
                                               & (vlSelf->top__DOT__inst 
                                                  >> 0x14U)) 
                                              | (0x1eU 
                                                 & (vlSelf->top__DOT__inst 
                                                    >> 7U)))));
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list[2U] 
        = (((- (IData)((vlSelf->top__DOT__inst >> 0x1fU))) 
            << 0xcU) | ((0xfe0U & (vlSelf->top__DOT__inst 
                                   >> 0x14U)) | (0x1fU 
                                                 & (vlSelf->top__DOT__inst 
                                                    >> 7U))));
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list[3U] 
        = (0xfffff000U & vlSelf->top__DOT__inst);
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list[4U] 
        = (((- (IData)((vlSelf->top__DOT__inst >> 0x1fU))) 
            << 0xcU) | (vlSelf->top__DOT__inst >> 0x14U));
    vlSelf->top__DOT____Vcellinp__alu__A = ((IData)(vlSelf->top__DOT__ALUAsrc)
                                             ? vlSelf->top__DOT__pc
                                             : vlSelf->top__DOT__reg_src1);
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
        = ((- (IData)(((IData)(vlSelf->top__DOT__ExtOP) 
                       == vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list
                       [0U]))) & vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list
           [0U]);
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
           | ((- (IData)(((IData)(vlSelf->top__DOT__ExtOP) 
                          == vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list
                          [1U]))) & vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list
              [1U]));
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
           | ((- (IData)(((IData)(vlSelf->top__DOT__ExtOP) 
                          == vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list
                          [2U]))) & vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list
              [2U]));
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
           | ((- (IData)(((IData)(vlSelf->top__DOT__ExtOP) 
                          == vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list
                          [3U]))) & vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list
              [3U]));
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out 
           | ((- (IData)(((IData)(vlSelf->top__DOT__ExtOP) 
                          == vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list
                          [4U]))) & vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list
              [4U]));
    vlSelf->top__DOT__imm = vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out;
    VL_WRITEF("immI: %10#\nimm: %10#\n",32,vlSelf->top__DOT__idu__DOT__immI,
              32,vlSelf->top__DOT__imm);
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__data_list[0U] 
        = (vlSelf->top__DOT__imm + vlSelf->top__DOT__reg_src1);
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__data_list[1U] 
        = (vlSelf->top__DOT__imm + vlSelf->top__DOT__pc);
    if ((0U == (IData)(vlSelf->top__DOT__ALUBsrc))) {
        vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[4U] 
            = vlSelf->top__DOT__reg_src2;
        vlSelf->top__DOT____Vcellinp__alu__B = vlSelf->top__DOT__reg_src2;
    } else if ((1U == (IData)(vlSelf->top__DOT__ALUBsrc))) {
        vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[4U] 
            = vlSelf->top__DOT__imm;
        vlSelf->top__DOT____Vcellinp__alu__B = vlSelf->top__DOT__imm;
    } else if ((2U == (IData)(vlSelf->top__DOT__ALUBsrc))) {
        vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[4U] = 4U;
        vlSelf->top__DOT____Vcellinp__alu__B = 4U;
    } else {
        vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[4U] = 0U;
        vlSelf->top__DOT____Vcellinp__alu__B = 0U;
    }
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__lut_out 
        = ((- (IData)(((IData)(vlSelf->top__DOT__Branch) 
                       == vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__key_list
                       [0U]))) & vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__data_list
           [0U]);
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__lut_out 
           | ((- (IData)(((IData)(vlSelf->top__DOT__Branch) 
                          == vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__key_list
                          [1U]))) & vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__data_list
              [1U]));
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__lut_out 
           | ((- (IData)(((IData)(vlSelf->top__DOT__Branch) 
                          == vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__key_list
                          [2U]))) & vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__data_list
              [2U]));
    vlSelf->top__DOT__NextPC = vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__lut_out;
    vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__data_list[0U] 
        = (vlSelf->top__DOT____Vcellinp__alu__A - vlSelf->top__DOT____Vcellinp__alu__B);
    vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__data_list[1U] 
        = (vlSelf->top__DOT____Vcellinp__alu__A + vlSelf->top__DOT____Vcellinp__alu__B);
    vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__lut_out 
        = ((- (IData)(((1U & ((IData)(vlSelf->top__DOT__ALUctr) 
                              >> 3U)) == vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__key_list
                       [0U]))) & vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__data_list
           [0U]);
    vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__lut_out 
           | ((- (IData)(((1U & ((IData)(vlSelf->top__DOT__ALUctr) 
                                 >> 3U)) == vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__key_list
                          [1U]))) & vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__data_list
              [1U]));
    vlSelf->top__DOT__alu__DOT__adder = vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__lut_out;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[7U] 
        = vlSelf->top__DOT__alu__DOT__adder;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
        = ((- (IData)(((7U & (IData)(vlSelf->top__DOT__ALUctr)) 
                       == vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list
                       [0U]))) & vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list
           [0U]);
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
           | ((- (IData)(((7U & (IData)(vlSelf->top__DOT__ALUctr)) 
                          == vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list
                          [1U]))) & vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list
              [1U]));
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
           | ((- (IData)(((7U & (IData)(vlSelf->top__DOT__ALUctr)) 
                          == vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list
                          [2U]))) & vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list
              [2U]));
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
           | ((- (IData)(((7U & (IData)(vlSelf->top__DOT__ALUctr)) 
                          == vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list
                          [3U]))) & vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list
              [3U]));
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
           | ((- (IData)(((7U & (IData)(vlSelf->top__DOT__ALUctr)) 
                          == vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list
                          [4U]))) & vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list
              [4U]));
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
           | ((- (IData)(((7U & (IData)(vlSelf->top__DOT__ALUctr)) 
                          == vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list
                          [5U]))) & vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list
              [5U]));
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
           | ((- (IData)(((7U & (IData)(vlSelf->top__DOT__ALUctr)) 
                          == vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list
                          [6U]))) & vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list
              [6U]));
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
        = (vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out 
           | ((- (IData)(((7U & (IData)(vlSelf->top__DOT__ALUctr)) 
                          == vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list
                          [7U]))) & vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list
              [7U]));
    vlSelf->top__DOT__ALUout = vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out;
    vlSelf->top__DOT____Vcellinp__gpr__busW = ((IData)(vlSelf->top__DOT__MemtoReg)
                                                ? 0U
                                                : vlSelf->top__DOT__ALUout);
}
