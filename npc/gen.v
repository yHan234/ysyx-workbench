// Control Signal Generator
// output [2:0] ExtOP,     // 选择立即数产生器的输出类型，具体见代码。
// output RegWr,           // 控制是否对寄存器rd进行写回，为1时写回寄存器。
// output [2:0] Branch,    // 说明分支和跳转的种类，用于生成最终的分支控制信号，具体见代码。
// output MemtoReg,        // 选择寄存器rd写回数据来源，为0时选择ALU输出，为1时选择数据存储器输出。
// output MemWr,           // 控制是否对数据存储器进行写入，为1时写回存储器。
// output [2:0] MemOp,     // 控制数据存储器读写格式，为010时为4字节读写，为001时为2字节读写带符号扩展，为000时为1字节读写带符号扩展，为101时为2字节读写无符号扩展，为100时为1字节读写无符号扩展。
// output ALUAsrc,         // 选择ALU输入端A的来源。为0时选择rs1，为1时选择PC。
// output [1:0] ALUBsrc,   // 选择ALU输入端B的来源。为00时选择rs2，为01时选择imm(当是立即数移位指令时，只有低5位有效)，为10时选择常数4（用于跳转时计算返回地址PC+4）。
// output [3:0] ALUctr     // 选择ALU执行的操作，具体见ALU代码。

module CSG(
	input [6:0] op,
	input [2:0] func3,
	input [6:0] func7,
	output [2:0] ExtOP,
	output [0:0] RegWr,
	output [2:0] Branch,
	output [0:0] MemToReg,
	output [0:0] MemWr,
	output [2:0] MemOp,
	output [0:0] ALUAsrc,
	output [1:0] ALUBsrc,
	output [3:0] ALUctr
);
assign ExtOP = mux_op_out[18:16];
assign RegWr = mux_op_out[15:15];
assign Branch = mux_op_out[14:12];
assign MemToReg = mux_op_out[11:11];
assign MemWr = mux_op_out[10:10];
assign MemOp = mux_op_out[9:7];
assign ALUAsrc = mux_op_out[6:6];
assign ALUBsrc = mux_op_out[5:4];
assign ALUctr = mux_op_out[3:0];
wire [18:0] mux_op_out;
MuxKey #(9, 5, 19) mux_op (
	.key(op[6:2]),
	.out(mux_op_out),
	.lut({
		5'b01101, 19'b0011000000000010011,
		5'b00101, 19'b0011000000001010000,
		5'b00100, mux_func3_00100_out,
		5'b01100, mux_func3_01100_out,
		5'b11011, 19'b1001001000001100000,
		5'b11001, mux_func3_11001_out,
		5'b11000, mux_func3_11000_out,
		5'b00000, mux_func3_00000_out,
		5'b01000, mux_func3_01000_out
	})
);
wire [18:0] mux_func3_00100_out;
MuxKey #(8, 3, 19) mux_func3_00100 (
	.key(func3),
	.out(mux_func3_00100_out),
	.lut({
		3'b000, 19'b0001000000000010000,
		3'b010, 19'b0001000000000010010,
		3'b011, 19'b0001000000000011010,
		3'b100, 19'b0001000000000010100,
		3'b110, 19'b0001000000000010110,
		3'b111, 19'b0001000000000010111,
		3'b001, mux_func7_00100_001_out,
		3'b101, mux_func7_00100_101_out
	})
);
wire [18:0] mux_func7_00100_001_out;
MuxKey #(1, 1, 19) mux_func7_00100_001 (
	.key(func7[5:5]),
	.out(mux_func7_00100_001_out),
	.lut({
		1'b0, 19'b0001000000000010001
	})
);
wire [18:0] mux_func7_00100_101_out;
MuxKey #(2, 1, 19) mux_func7_00100_101 (
	.key(func7[5:5]),
	.out(mux_func7_00100_101_out),
	.lut({
		1'b0, 19'b0001000000000010101,
		1'b1, 19'b0001000000000011101
	})
);
wire [18:0] mux_func3_01100_out;
MuxKey #(8, 3, 19) mux_func3_01100 (
	.key(func3),
	.out(mux_func3_01100_out),
	.lut({
		3'b000, mux_func7_01100_000_out,
		3'b001, mux_func7_01100_001_out,
		3'b010, mux_func7_01100_010_out,
		3'b011, mux_func7_01100_011_out,
		3'b100, mux_func7_01100_100_out,
		3'b101, mux_func7_01100_101_out,
		3'b110, mux_func7_01100_110_out,
		3'b111, mux_func7_01100_111_out
	})
);
wire [18:0] mux_func7_01100_000_out;
MuxKey #(2, 1, 19) mux_func7_01100_000 (
	.key(func7[5:5]),
	.out(mux_func7_01100_000_out),
	.lut({
		1'b0, 19'b0001000000000000000,
		1'b1, 19'b0001000000000001000
	})
);
wire [18:0] mux_func7_01100_001_out;
MuxKey #(1, 1, 19) mux_func7_01100_001 (
	.key(func7[5:5]),
	.out(mux_func7_01100_001_out),
	.lut({
		1'b0, 19'b0001000000000000001
	})
);
wire [18:0] mux_func7_01100_010_out;
MuxKey #(1, 1, 19) mux_func7_01100_010 (
	.key(func7[5:5]),
	.out(mux_func7_01100_010_out),
	.lut({
		1'b0, 19'b0001000000000000010
	})
);
wire [18:0] mux_func7_01100_011_out;
MuxKey #(1, 1, 19) mux_func7_01100_011 (
	.key(func7[5:5]),
	.out(mux_func7_01100_011_out),
	.lut({
		1'b0, 19'b0001000000000001010
	})
);
wire [18:0] mux_func7_01100_100_out;
MuxKey #(1, 1, 19) mux_func7_01100_100 (
	.key(func7[5:5]),
	.out(mux_func7_01100_100_out),
	.lut({
		1'b0, 19'b0001000000000000100
	})
);
wire [18:0] mux_func7_01100_101_out;
MuxKey #(2, 1, 19) mux_func7_01100_101 (
	.key(func7[5:5]),
	.out(mux_func7_01100_101_out),
	.lut({
		1'b0, 19'b0001000000000000101,
		1'b1, 19'b0001000000000001101
	})
);
wire [18:0] mux_func7_01100_110_out;
MuxKey #(1, 1, 19) mux_func7_01100_110 (
	.key(func7[5:5]),
	.out(mux_func7_01100_110_out),
	.lut({
		1'b0, 19'b0001000000000000110
	})
);
wire [18:0] mux_func7_01100_111_out;
MuxKey #(1, 1, 19) mux_func7_01100_111 (
	.key(func7[5:5]),
	.out(mux_func7_01100_111_out),
	.lut({
		1'b0, 19'b0001000000000000111
	})
);
wire [18:0] mux_func3_11001_out;
MuxKey #(1, 3, 19) mux_func3_11001 (
	.key(func3),
	.out(mux_func3_11001_out),
	.lut({
		3'b000, 19'b0001010000001100000
	})
);
wire [18:0] mux_func3_11000_out;
MuxKey #(6, 3, 19) mux_func3_11000 (
	.key(func3),
	.out(mux_func3_11000_out),
	.lut({
		3'b000, 19'b0110100000000000010,
		3'b001, 19'b0110101000000000010,
		3'b100, 19'b0110110000000000010,
		3'b101, 19'b0110111000000000010,
		3'b110, 19'b0110110000000001010,
		3'b111, 19'b0110111000000001010
	})
);
wire [18:0] mux_func3_00000_out;
MuxKey #(5, 3, 19) mux_func3_00000 (
	.key(func3),
	.out(mux_func3_00000_out),
	.lut({
		3'b000, 19'b0001000100000010000,
		3'b001, 19'b0001000100010010000,
		3'b010, 19'b0001000100100010000,
		3'b100, 19'b0001000101000010000,
		3'b101, 19'b0001000101010010000
	})
);
wire [18:0] mux_func3_01000_out;
MuxKey #(3, 3, 19) mux_func3_01000 (
	.key(func3),
	.out(mux_func3_01000_out),
	.lut({
		3'b000, 19'b0100000010000010000,
		3'b001, 19'b0100000010010010000,
		3'b010, 19'b0100000010100010000
	})
);
endmodule
