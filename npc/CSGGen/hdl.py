class Vector:
    def __init__(self, name, val):
        self.name = name
        self.val = val
        self.slice = None

    def __getitem__(self, key):
        res = Vector(self.name, self.val)
        if isinstance(key, slice):
            res.slice = (key.start, key.stop)
        else:
            res.slice = (key, key)
        return res

    def __len__(self):
        if self.slice is not None:
            return self.slice[0] - self.slice[1] + 1
        elif isinstance(self.val, tuple):
            return self.val[0] - self.val[1] + 1
        elif isinstance(self.val, list):
            l = 0
            for vec in list:
                l += len(vec)
            return l
        else:
            raise RuntimeError(f'Vector "{self.name}" 内部结构错误')

    def __str__(self):
        # TODO: 向量拼接
        s = self.name
        if self.slice is not None:
            s += f"[{self.slice[0]}:{self.slice[1]}]"
        return s


class Module:
    def __init__(self, name, i_pins, o_pins={}):
        self.name = name
        self.i_pins = i_pins
        self.o_pins = o_pins

        self.vectors = []
        self.submodules = []

    def add_submodule(self, mod):
        self.submodules.append(mod)

    def new_vector(self, *args):
        self.vectors.append(Vector(*args))

    def dump_verilog(self):
        code = f"module {self.name}(\n"
        for name, vec in self.i_pins.items():
            assert name == vec.name
            assert isinstance(vec, Vector)
            assert isinstance(vec.val, tuple)
            assert vec.slice is None
            code += f"\tinput [{vec.val[0]}:{vec.val[1]}] {name},\n"
        for i, (name, vec) in enumerate(self.o_pins.items()):
            # 特殊处理
            code += f"\toutput [{vec.slice[0] - vec.slice[1]}:{0}] {name}"
            code += "\n" if i == len(self.o_pins) - 1 else ",\n"
        code += ");\n"

        for name, vec in self.o_pins.items():
            code += f"assign {name} = {vec.name}[{vec.slice[0]}:{vec.slice[1]}];\n"
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
        code = f"wire [{self.out.val[0]}:{self.out.val[1]}] {self.out.name};\n"
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
