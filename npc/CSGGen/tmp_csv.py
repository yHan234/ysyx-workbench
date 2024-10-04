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
        self.MemOP = None
        self.ALUAsrc = None
        self.ALUBsrc = None
        self.ALUctr = None
        self.ALUAction = None

    def res(self) -> str:
        return (
            self.ALUctr
            + self.ALUBsrc
            + self.ALUAsrc
            + self.MemOP
            + self.MemWr
            + self.MemToReg
            + self.Branch
            + self.RegWr
            + self.ExtOP
        )


def load_insts_from_csv(path: str, encoding: str):
    insts = []
    with open(path, encoding=encoding) as f:
        with open("newinst.csv", "w", encoding="utf-8") as fw:
            reader = csv.reader(f)
            writer = csv.writer(fw)
            header = next(reader)
            writer.writerow(header)
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
                "MemOP",
                "ALUAsrc",
                "ALUBsrc",
                "ALUctr",
                "ALUAction",
            ]
            for row in reader:
                row[0] = row[0]
                row[1] = None if row[1] == "" else row[1].zfill(5)
                row[2] = None if row[2] == "" else row[2].zfill(3)
                row[3] = None if row[3] == "" else row[3]
                row[4] = None if row[4] == "" else row[4].zfill(3)
                row[5] = None if row[5] == "" else row[5].zfill(1)
                row[6] = None if row[6] == "" else row[6].zfill(3)
                row[7] = None if row[7] == "" else row[7].zfill(1)
                row[8] = None if row[8] == "" else row[8].zfill(1)
                row[9] = None if row[9] == "" else row[9].zfill(3)
                row[10] = None if row[10] == "" else row[10].zfill(1)
                row[11] = None if row[11] == "" else row[11].zfill(2)
                row[12] = None if row[12] == "" else row[12].zfill(4)
                row[13] = None if row[13] == "" else row[13]
                writer.writerow(row)
    return insts

load_insts_from_csv("CSGGen/inst.csv", "utf-8-sig")