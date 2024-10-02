// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "verilated.h"
#include "verilated_dpi.h"

#include "Vtop___024root.h"

VL_ATTR_COLD void Vtop___024root___eval_static(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    // Body
    Vtop___024root___eval_initial__TOP(vlSelf);
    vlSelf->__Vtrigrprev__TOP__clk = vlSelf->clk;
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial__TOP\n"); );
    // Body
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__key_list[0U] = 2U;
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__key_list[1U] = 1U;
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__key_list[2U] = 0U;
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list[0U] = 4U;
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list[1U] = 3U;
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list[2U] = 2U;
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list[3U] = 1U;
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list[4U] = 0U;
    vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__key_list[0U] = 1U;
    vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__key_list[1U] = 0U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[0U] = 7U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[1U] = 6U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[2U] = 5U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[3U] = 4U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[4U] = 3U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[5U] = 2U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[6U] = 1U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[7U] = 0U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[0U] = 0U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[1U] = 0U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[2U] = 0U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[3U] = 0U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[5U] = 0U;
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[6U] = 0U;
}

VL_ATTR_COLD void Vtop___024root___eval_final(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_final\n"); );
}

VL_ATTR_COLD void Vtop___024root___eval_triggers__stl(Vtop___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    // Init
    CData/*0:0*/ __VstlContinue;
    // Body
    vlSelf->__VstlIterCount = 0U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        __VstlContinue = 0U;
        Vtop___024root___eval_triggers__stl(vlSelf);
        if (vlSelf->__VstlTriggered.any()) {
            __VstlContinue = 1U;
            if (VL_UNLIKELY((0x64U < vlSelf->__VstlIterCount))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__stl(vlSelf);
#endif
                VL_FATAL_MT("vsrc/top.v", 1, "", "Settle region did not converge.");
            }
            vlSelf->__VstlIterCount = ((IData)(1U) 
                                       + vlSelf->__VstlIterCount);
            Vtop___024root___eval_stl(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if (vlSelf->__VstlTriggered.at(0U)) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    // Body
    if (vlSelf->__VstlTriggered.at(0U)) {
        Vtop___024root___stl_sequent__TOP__0(vlSelf);
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if (vlSelf->__VactTriggered.at(0U)) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__nba(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if (vlSelf->__VnbaTriggered.at(0U)) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__NextPC = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__pc = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__reg_src1 = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__reg_src2 = VL_RAND_RESET_I(32);
    vlSelf->top__DOT____Vcellinp__gpr__busW = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__inst = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__ExtOP = VL_RAND_RESET_I(3);
    vlSelf->top__DOT__imm = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__RegWr = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__ALUAsrc = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__ALUBsrc = VL_RAND_RESET_I(2);
    vlSelf->top__DOT__ALUctr = VL_RAND_RESET_I(4);
    vlSelf->top__DOT__Branch = VL_RAND_RESET_I(3);
    vlSelf->top__DOT__MemtoReg = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__ALUout = VL_RAND_RESET_I(32);
    vlSelf->top__DOT____Vcellinp__alu__B = VL_RAND_RESET_I(32);
    vlSelf->top__DOT____Vcellinp__alu__A = VL_RAND_RESET_I(32);
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__key_list[__Vi0] = VL_RAND_RESET_I(3);
    }
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__data_list[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->top__DOT__mux_next_pc__DOT__i0__DOT__lut_out = VL_RAND_RESET_I(32);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->top__DOT__gpr__DOT__gpr[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->top__DOT__idu__DOT__immI = VL_RAND_RESET_I(32);
    for (int __Vi0 = 0; __Vi0 < 5; ++__Vi0) {
        vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__key_list[__Vi0] = VL_RAND_RESET_I(3);
    }
    for (int __Vi0 = 0; __Vi0 < 5; ++__Vi0) {
        vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__data_list[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->top__DOT__idu__DOT__mux_imm__DOT__i0__DOT__lut_out = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__alu__DOT__adder = VL_RAND_RESET_I(32);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__key_list[__Vi0] = VL_RAND_RESET_I(1);
    }
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__data_list[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->top__DOT__alu__DOT__mux_adder__DOT__i0__DOT__lut_out = VL_RAND_RESET_I(32);
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__key_list[__Vi0] = VL_RAND_RESET_I(3);
    }
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__data_list[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->top__DOT__alu__DOT__mux_top__DOT__i0__DOT__lut_out = VL_RAND_RESET_I(32);
    vlSelf->__Vtask_get_inst__0__inst = 0;
    vlSelf->__Vtrigrprev__TOP__clk = VL_RAND_RESET_I(1);
}
