import csv


class Inst:
    def __init__(self):
        self.name = None
        self.op = None
        self.func3 = None
        self.func7 = None
        self.ExtOP = None
        self.RegWr = None
        self.Branch = None
        self.MemToReg = None
        self.MemWr = None
        self.MemOp = None
        self.ALUAsrc = None
        self.ALUBsrc = None
        self.ALUctr = None
        self.ALUAction = None

    def res(self) -> str:
        return (
            self.ExtOP
            + self.RegWr
            + self.Branch
            + self.MemToReg
            + self.MemWr
            + self.MemOp
            + self.ALUAsrc
            + self.ALUBsrc
            + self.ALUctr
        )


def load_insts_from_csv(path: str):
    insts = []
    with open(path) as f:
        reader = csv.reader(f)
        header = next(reader)
        assert header == [
            "name",
            "op[6:2]",
            "func3",
            "func7[5]",
            "ExtOP",
            "RegWr",
            "Branch",
            "MemToReg",
            "MemWr",
            "MemOp",
            "ALUAsrc",
            "ALUBsrc",
            "ALUctr",
            "ALUAction",
        ]
        for row in reader:
            insts.append(Inst())
            insts[-1].name = row[0]
            insts[-1].op = None if row[1] == "" else row[1]
            insts[-1].func3 = None if row[2] == "" else row[2]
            insts[-1].func7 = None if row[3] == "" else row[3]
            insts[-1].ExtOP = row[4].zfill(3)
            insts[-1].RegWr = row[5].zfill(1)
            insts[-1].Branch = row[6].zfill(3)
            insts[-1].MemToReg = row[7].zfill(1)
            insts[-1].MemWr = row[8].zfill(1)
            insts[-1].MemOp = row[9].zfill(3)
            insts[-1].ALUAsrc = row[10].zfill(1)
            insts[-1].ALUBsrc = row[11].zfill(2)
            insts[-1].ALUctr = row[12].zfill(4)
            insts[-1].ALUAction = row[13]
    return insts
