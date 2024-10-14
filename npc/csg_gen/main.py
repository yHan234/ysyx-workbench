from inst_csv import *
from hdl import *


if __name__ == "__main__":
    icsv = InstCSV.load("csg_gen/inst.csv")

    csg = Module(
        name="CSG",
        i_pins={
            "inst": Vector("inst", 32),
            "op": Vector("op", 7),
            "funct3": Vector("funct3", 3),
            "funct7": Vector("funct7", 7),
        },
        o_pins={s.name: Vector(s.name, s.len) for s in icsv.signals},
    )

    csg_o_pins_join = None
    for s in icsv.signals:
        if csg_o_pins_join is None:
            csg_o_pins_join = csg.o_pins[s.name]
        else:
            csg_o_pins_join = csg_o_pins_join + csg.o_pins[s.name]

    mux_special = MuxKeyWithDefault("mux_special", csg.i_pins["inst"], csg_o_pins_join)
    csg.add_submodule(mux_special)

    mux_op = MuxKey(
        "mux_op",
        csg.i_pins["op"][6:2],
        Vector("mux_op_out", icsv.sig_tot_len),
    )
    csg.add_submodule(mux_op)
    mux_special.default = mux_op.out

    mux_funct3_dic = {}
    mux_funct7_dic = {}

    def get_mux_funct3(op):
        if op in mux_funct3_dic:
            mux_funct3 = mux_funct3_dic[op]
        else:
            mux_funct3 = MuxKey(
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
            mux_funct7 = MuxKey(
                name=f"mux_funct7_{op}_{funct3}",
                key=csg.i_pins["funct7"][5],
                out=Vector(f"mux_funct7_{op}_{funct3}_out", icsv.sig_tot_len),
            )
            csg.add_submodule(mux_funct7)
            get_mux_funct3(op).lut[funct3] = mux_funct7.out
            mux_funct7_dic[op_funct3] = mux_funct7
        return mux_funct7

    for inst in icsv.insts:
        if inst.inst is not None:  # 全 32 位固定的指令
            if len(inst.inst) != 32:
                print(inst.name)
            mux_special.lut[inst.inst] = inst.sig_str()
        elif inst.funct3 is None:  # 只有 op 的指令
            mux_op.lut[inst.op] = inst.sig_str()
        elif inst.funct7 is None:  # 有 op + funct3 的指令
            mux_funct3 = get_mux_funct3(inst.op)
            mux_funct3.lut[inst.funct3] = inst.sig_str()
        else:  # 有 op + funct3 + funct7 的指令
            mux_funct7 = get_mux_funct7(inst.op, inst.funct3)
            mux_funct7.lut[inst.funct7] = inst.sig_str()

    print(csg.dump_verilog())
