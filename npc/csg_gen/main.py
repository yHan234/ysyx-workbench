from inst_csv import *
from hdl import *


if __name__ == "__main__":
    icsv = InstCSV.load("csg_gen/inst.csv")

    csg = Module(
        name="CSG",
        i_pins={
            "op": Vector("op", (6, 0)),
            "funct3": Vector("funct3", (2, 0)),
            "funct7": Vector("funct7", (6, 0)),
        },
        o_pins={s.name: Vector(s.name, s.len) for s in icsv.signals},
    )

    csg_o_pins_join = None
    for s in icsv.signals:
        if csg_o_pins_join is None:
            csg_o_pins_join = csg.o_pins[s.name]
        else:
            csg_o_pins_join = csg_o_pins_join + csg.o_pins[s.name]

    mux_op = Mux(
        "mux_op",
        csg.i_pins["op"][6:2],
        csg_o_pins_join,
    )
    csg.add_submodule(mux_op)

    mux_funct3_dic = {}
    mux_funct7_dic = {}

    def get_mux_funct3(op):
        if op in mux_funct3_dic:
            mux_funct3 = mux_funct3_dic[op]
        else:
            mux_funct3 = Mux(
                name=f"mux_funct3_{op}",
                key=csg.i_pins["funct3"],
                out=Vector(f"mux_funct3_{op}_out", icsv.sig_tot_len),
            )
            csg.add_submodule(mux_funct3)
            mux_op.lut[op] = mux_funct3.out
            mux_funct3_dic[op] = mux_funct3
        return mux_funct3

    def get_mux_funct7(op, funct3):
        op_funct3 = op + funct3
        if op_funct3 in mux_funct7_dic:
            mux_funct7 = mux_funct7_dic[op_funct3]
        else:
            mux_funct7 = Mux(
                name=f"mux_funct7_{op}_{funct3}",
                key=csg.i_pins["funct7"][5],
                out=Vector(f"mux_funct7_{op}_{funct3}_out", icsv.sig_tot_len),
            )
            csg.add_submodule(mux_funct7)
            get_mux_funct3(op).lut[funct3] = mux_funct7.out
            mux_funct7_dic[op_funct3] = mux_funct7
        return mux_funct7

    for inst in icsv.insts:
        if inst.funct3 is None:
            mux_op.lut[inst.op] = inst.sig_str()
        elif inst.funct7 is None:
            mux_funct3 = get_mux_funct3(inst.op)
            mux_funct3.lut[inst.funct3] = inst.sig_str()
        else:
            mux_funct7 = get_mux_funct7(inst.op, inst.funct3)
            mux_funct7.lut[inst.funct7] = inst.sig_str()

    print(
        """// Control Signal Generator
// inst_type    选择立即数产生器的输出类型，具体见代码。
// reg_wr_en    控制是否对寄存器rd进行写回，为1时写回寄存器。
// branch       说明分支和跳转的种类，用于生成最终的分支控制信号，具体见代码。
// mem2reg      选择寄存器rd写回数据来源，为0时选择ALU输出，为1时选择数据存储器输出。
// mem_wr_en    控制是否对数据存储器进行写入，为1时写回存储器。
// mem_op       控制数据存储器读写格式，为010时为4字节读写，为001时为2字节读写带符号扩展，为000时为1字节读写带符号扩展，为101时为2字节读写无符号扩展，为100时为1字节读写无符号扩展。
"""
    )
    print(csg.dump_verilog())
