import csv


class InstCSV:
    """
    CSV 格式：

    row1: name, inst, op[6:2], funct3, funct7[5], {要生成的信号...}
        inst 为全 32 位固定的指令特判

    row2: ,,,,,{要生成的信号的长度...}

    rows...
    """

    class Signal:
        def __init__(self, name, len):
            self.name = name
            self.len = len

    class Inst:
        def __init__(self, name, inst, op, funct3, funct7, signals):
            self.name = name
            self.inst = inst
            self.op = op
            self.funct3 = funct3
            self.funct7 = funct7
            self.signals = signals

        def sig_str(self):
            return "".join(self.signals)

    def __init__(self):
        self.signals = None
        self.insts = None
        self.sig_tot_len = None

    def load(path: str):
        icsv = InstCSV
        with open(path) as f:
            reader = csv.reader(f)

            header = next(reader)
            assert header[0:5] == [
                "name",
                "inst",
                "op[6:2]",
                "funct3",
                "funct7[5]",
            ]
            lengths = [None if s == "" else int(s) for s in next(reader)]

            icsv.signals = [
                InstCSV.Signal(header[i], lengths[i]) for i in range(5, len(header))
            ]
            icsv.sig_tot_len = sum([s.len for s in icsv.signals])

            icsv.insts = [
                InstCSV.Inst(
                    inst[0],  # name
                    None if inst[1] == "" else inst[1],  # inst
                    None if inst[2] == "" else inst[2],  # op
                    None if inst[3] == "" else inst[3],  # funct3
                    None if inst[4] == "" else inst[4],  # funct7
                    [
                        inst[i].zfill(lengths[i])  # 用 0 填充各个信号
                        for i in range(5, len(inst))
                    ],
                )
                for inst in reader
            ]

        return icsv
