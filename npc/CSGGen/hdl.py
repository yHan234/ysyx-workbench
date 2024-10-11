class Vector:
    """
    表示一个一维向量。

    name:   向量的名字，None 为匿名。

    val:    当 val 为 tuple(l, r) 时，这是一个原始向量，l 和 r 表示这个向量表示的比特范围。
            当 val 为 Vector 时，表示这个向量是另一个向量的切片，由 slice 表示切片范围。
            当 val 为 list[Vector] 时，表示这是一些向量的拼接。

    slice:  tuple(l, r) 表示这个向量切片 val 的范围，None 表示全部。

    切片 vec[l:r]
    拼接 vec1 + vec2
    长度 len(vec)
    """

    def __init__(self, name, val=None):
        self.name = name
        self.val = val if val is not None else (0, 0)
        self.slice = None

    def __getitem__(self, key):
        res = Vector(self.name, self.val)
        if isinstance(key, slice):
            res.slice = (key.start, key.stop)
        elif isinstance(key, int):
            res.slice = (key, key)
        else:
            raise RuntimeError("Invalid key")
        return res

    def __len__(self):
        if self.slice is not None:
            return self.slice[0] - self.slice[1] + 1
        elif isinstance(self.val, tuple):
            return self.val[0] - self.val[1] + 1
        elif isinstance(self.val, Vector):
            return len(self.val)
        elif isinstance(self.val, list):
            return sum(len(vec) for vec in self.val)
        else:
            raise RuntimeError(f'Vector "{self.name}" 内部结构错误')

    def __add__(self, other):
        def to_list(vec):
            return vec.val if isinstance(vec.val, list) else [vec]

        return Vector(None, to_list(self) + to_list(other))

    def __str__(self):
        if self.name is not None:
            s = self.name
        elif isinstance(self.val, list):
            s = "{" + ", ".join(str(vec) for vec in self.val) + "}"
        else:
            raise RuntimeError(f'Vector "{self.name}" 内部结构错误')

        if self.slice is not None:
            s += f"[{self.slice[0]}:{self.slice[1]}]"
        return s


class Module:
    def __init__(self, name, i_pins, o_pins={}):
        self.name = name
        self.i_pins = i_pins
        self.o_pins = o_pins

        self.submodules = []

    def add_submodule(self, mod):
        self.submodules.append(mod)

    def dump_verilog(self):
        code = f"module {self.name}(\n"
        for name, vec in self.i_pins.items():
            assert name == vec.name
            assert isinstance(vec, Vector)
            assert isinstance(vec.val, tuple)
            assert vec.slice is None
            code += f"\tinput [{vec.val[0]}:{vec.val[1]}] {name},\n"
        for i, (name, vec) in enumerate(self.o_pins.items()):
            code += f"\toutput [{vec.val[0]}:{vec.val[1]}] {name}"
            code += "\n" if i == len(self.o_pins) - 1 else ",\n"
        code += ");\n"

        for submodule in self.submodules:
            code += submodule.dump_verilog()

        code += "endmodule"
        return code


class Mux:
    def __init__(self, name, key=None, out=None):
        self.name = name
        self.key = key
        self.out = out
        self.lut = {}

    def dump_verilog(self):
        code = (
            f"wire [{self.out.val[0]}:{self.out.val[1]}] {self.out.name};\n"
            if self.out.name is not None
            else ""
        )
        code += f"MuxKey #({len(self.lut)}, {len(self.key)}, {len(self.out)}) {self.name} (\n"
        code += f"\t.key({self.key}),\n"
        code += f"\t.out({self.out}),\n"
        code += "\t.lut({\n"
        for i, (k, v) in enumerate(self.lut.items()):
            v_str = v if isinstance(v, Vector) else f"{len(v)}'b{v}"
            code += f"\t\t{len(self.key)}'b{k}, {v_str}"
            code += "\n" if i == len(self.lut) - 1 else ",\n"  # 最后一个不加逗号
        code += "\t})\n"
        code += ");\n"
        return code
