from inst_csv import *
from hdl import *


if __name__ == "__main__":
    insts = load_insts_from_csv("CSGGen/inst.csv")

    csg = Module(
        name="CSG",
        i_pins={
            "op": Vector("op", (6, 0)),
            "func3": Vector("func3", (2, 0)),
            "func7": Vector("func7", (6, 0)),
        },
        o_pins={
            "inst_type": Vector("inst_type", (2, 0)),
            "reg_wr_en": Vector("reg_wr_en"),
            "branch": Vector("branch", (2, 0)),
            "mem2reg": Vector("mem2reg"),
            "mem_rd_en": Vector("mem_rd_en"),
            "mem_wr_en": Vector("mem_wr_en"),
            "mem_op": Vector("mem_op", (2, 0)),
            "alu_src_a": Vector("alu_src_a"),
            "alu_src_b": Vector("alu_src_b", (1, 0)),
            "alu_ctr": Vector("alu_ctr", (3, 0)),
        },
    )

    mux_op = Mux(
        "mux_op",
        csg.i_pins["op"][6:2],
        csg.o_pins["inst_type"]
        + csg.o_pins["reg_wr_en"]
        + csg.o_pins["branch"]
        + csg.o_pins["mem2reg"]
        + csg.o_pins["mem_rd_en"]
        + csg.o_pins["mem_wr_en"]
        + csg.o_pins["mem_op"]
        + csg.o_pins["alu_src_a"]
        + csg.o_pins["alu_src_b"]
        + csg.o_pins["alu_ctr"],
    )
    csg.add_submodule(mux_op)

    mux_func3_dic = {}
    mux_func7_dic = {}

    def get_mux_func3(op):
        if op in mux_func3_dic:
            mux_func3 = mux_func3_dic[op]
        else:
            mux_func3 = Mux(
                name=f"mux_func3_{op}",
                key=csg.i_pins["func3"],
                out=Vector(f"mux_func3_{op}_out", (19, 0)),
            )
            csg.add_submodule(mux_func3)
            mux_op.lut[op] = mux_func3.out
            mux_func3_dic[op] = mux_func3
        return mux_func3

    def get_mux_func7(op, func3):
        op_func3 = op + func3
        if op_func3 in mux_func7_dic:
            mux_func7 = mux_func7_dic[op_func3]
        else:
            mux_func7 = Mux(
                name=f"mux_func7_{op}_{func3}",
                key=csg.i_pins["func7"][5],
                out=Vector(f"mux_func7_{op}_{func3}_out", (19, 0)),
            )
            csg.add_submodule(mux_func7)
            get_mux_func3(op).lut[func3] = mux_func7.out
            mux_func7_dic[op_func3] = mux_func7
        return mux_func7

    for inst in insts:
        if inst.func3 is None:
            mux_op.lut[inst.op] = inst.res()
        elif inst.func7 is None:
            mux_func3 = get_mux_func3(inst.op)
            mux_func3.lut[inst.func3] = inst.res()
        else:
            mux_func7 = get_mux_func7(inst.op, inst.func3)
            mux_func7.lut[inst.func7] = inst.res()

    print(
        """// Control Signal Generator
// output [2:0] inst_type,     // 选择立即数产生器的输出类型，具体见代码。
// output reg_wr_en,           // 控制是否对寄存器rd进行写回，为1时写回寄存器。
// output [2:0] branch,    // 说明分支和跳转的种类，用于生成最终的分支控制信号，具体见代码。
// output mem2reg,        // 选择寄存器rd写回数据来源，为0时选择ALU输出，为1时选择数据存储器输出。
// output mem_wr_en,           // 控制是否对数据存储器进行写入，为1时写回存储器。
// output [2:0] mem_op,     // 控制数据存储器读写格式，为010时为4字节读写，为001时为2字节读写带符号扩展，为000时为1字节读写带符号扩展，为101时为2字节读写无符号扩展，为100时为1字节读写无符号扩展。
"""
    )
    print(csg.dump_verilog())
