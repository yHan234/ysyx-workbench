def generate_addi_test():
    binary_instructions = []

    for i in range(31):
        imm = f"{i + 1:012b}"
        rs1 = f"{i:05b}"
        funct3 = "000"
        rd = f"{i + 1:05b}"
        opcode = "0010011"
        instruction = imm + rs1 + funct3 + rd + opcode
        binary_instructions.append(int(instruction, 2))  # 将指令转成整数表示
        print(imm, rs1, funct3, rd, opcode)

    return binary_instructions


ebreak = int("00000000000100000000000001110011", base=2)


def write_binary_file(filename, binary_instructions):
    with open(filename, "wb") as f:
        for instruction in binary_instructions:
            f.write(
                instruction.to_bytes(4, byteorder="little")
            )  # 每条指令4个字节，小端序写入
        f.write(ebreak.to_bytes(4, byteorder="little"))


if __name__ == "__main__":
    write_binary_file("tests/addi.bin", generate_addi_test())
