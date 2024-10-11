import csv


class Inst:
    def __init__(self):
        self.name = None
        self.op = None
        self.funct3 = None
        self.funct7 = None
        self.inst_type = None
        self.reg_wr_en = None
        self.branch = None
        self.mem2reg = None
        self.mem_rd_en = None
        self.mem_wr_en = None
        self.mem_op = None
        self.alu_src_a = None
        self.alu_src_b = None
        self.alu_ctr = None
        self.explain = None

    def res(self) -> str:
        return (
            self.inst_type
            + self.reg_wr_en
            + self.branch
            + self.mem2reg
            + self.mem_rd_en
            + self.mem_wr_en
            + self.mem_op
            + self.alu_src_a
            + self.alu_src_b
            + self.alu_ctr
        )


def load_insts_from_csv(path: str):
    insts = []
    with open(path) as f:
        reader = csv.reader(f)
        header = next(reader)
        assert header == [
            "name",
            "op[6:2]",
            "funct3",
            "funct7[5]",
            "inst_type",
            "reg_wr_en",
            "branch",
            "mem2reg",
            "mem_rd_en",
            "mem_wr_en",
            "mem_op",
            "alu_src_a",
            "alu_src_b",
            "alu_ctr",
            "explain",
        ]
        for row in reader:
            insts.append(Inst())
            insts[-1].name = row[0]
            insts[-1].op = None if row[1] == "" else row[1]
            insts[-1].funct3 = None if row[2] == "" else row[2]
            insts[-1].funct7 = None if row[3] == "" else row[3]
            insts[-1].inst_type = row[4].zfill(3)
            insts[-1].reg_wr_en = row[5].zfill(1)
            insts[-1].branch = row[6].zfill(3)
            insts[-1].mem2reg = row[7].zfill(1)
            insts[-1].mem_rd_en = row[8].zfill(1)
            insts[-1].mem_wr_en = row[9].zfill(1)
            insts[-1].mem_op = row[10].zfill(3)
            insts[-1].alu_src_a = row[11].zfill(1)
            insts[-1].alu_src_b = row[12].zfill(2)
            insts[-1].alu_ctr = row[13].zfill(4)
            insts[-1].explain = row[14]
    return insts
