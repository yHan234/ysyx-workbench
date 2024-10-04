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
    )

    mux_op = Mux(
        "mux_op",
        csg.i_pins["op"][6:2],
        Vector("mux_op_out", (19, 0)),
    )
    csg.add_submodule(mux_op)

    csg.o_pins = {
        "ExtOP": mux_op.out[19:17],
        "RegWr": mux_op.out[16],
        "Branch": mux_op.out[15:13],
        "MemToReg": mux_op.out[12],
        "MemRd": mux_op.out[11],
        "MemWr": mux_op.out[10],
        "MemOp": mux_op.out[9:7],
        "ALUAsrc": mux_op.out[6],
        "ALUBsrc": mux_op.out[5:4],
        "ALUctr": mux_op.out[3:0],
    }

    mux_func3_dic = {}
    mux_func7_dic = {}

    for inst in insts:
        if inst.func3 is None:
            mux_op.lut[inst.op] = inst.res()
        elif inst.func7 is None:
            if inst.op in mux_func3_dic:
                mux_func3 = mux_func3_dic[inst.op]
            else:
                mux_func3 = Mux(
                    name=f"mux_func3_{inst.op}",
                    key=csg.i_pins["func3"],
                    out=Vector(f"mux_func3_{inst.op}_out", (19, 0)),
                )
                csg.add_submodule(mux_func3)
                mux_op.lut[inst.op] = mux_func3.out
                mux_func3_dic[inst.op] = mux_func3

            mux_func3.lut[inst.func3] = inst.res()
        else:
            # copy 的上个 if 分支
            # TODO: 精简代码
            if inst.op in mux_func3_dic:
                mux_func3 = mux_func3_dic[inst.op]
            else:
                mux_func3 = Mux(
                    name=f"mux_func3_{inst.op}",
                    key=csg.i_pins["func3"],
                    out=Vector(f"mux_func3_{inst.op}_out", (19, 0)),
                )
                csg.add_submodule(mux_func3)
                mux_op.lut[inst.op] = mux_func3.out
                mux_func3_dic[inst.op] = mux_func3

            op_func3 = inst.op + inst.func3
            if op_func3 in mux_func7_dic:
                mux_func7 = mux_func7_dic[op_func3]
            else:
                mux_func7 = Mux(
                    name=f"mux_func7_{inst.op}_{inst.func3}",
                    key=csg.i_pins["func7"][5],
                    out=Vector(f"mux_func7_{inst.op}_{inst.func3}_out", (19, 0)),
                )
                csg.add_submodule(mux_func7)
                mux_func3.lut[inst.func3] = mux_func7.out
                mux_func7_dic[op_func3] = mux_func7

            mux_func7.lut[inst.func7] = inst.res()

    print(
        """// Control Signal Generator
// output [2:0] ExtOP,     // 选择立即数产生器的输出类型，具体见代码。
// output RegWr,           // 控制是否对寄存器rd进行写回，为1时写回寄存器。
// output [2:0] Branch,    // 说明分支和跳转的种类，用于生成最终的分支控制信号，具体见代码。
// output MemtoReg,        // 选择寄存器rd写回数据来源，为0时选择ALU输出，为1时选择数据存储器输出。
// output MemWr,           // 控制是否对数据存储器进行写入，为1时写回存储器。
// output [2:0] MemOp,     // 控制数据存储器读写格式，为010时为4字节读写，为001时为2字节读写带符号扩展，为000时为1字节读写带符号扩展，为101时为2字节读写无符号扩展，为100时为1字节读写无符号扩展。
"""
    )
    print(csg.dump_verilog())
