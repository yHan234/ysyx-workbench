// Control Signal Generator
// output [2:0] ExtOP,     // 选择立即数产生器的输出类型，具体见代码。
// output RegWr,           // 控制是否对寄存器rd进行写回，为1时写回寄存器。
// output [2:0] Branch,    // 说明分支和跳转的种类，用于生成最终的分支控制信号，具体见代码。
// output MemtoReg,        // 选择寄存器rd写回数据来源，为0时选择ALU输出，为1时选择数据存储器输出。
// output MemWr,           // 控制是否对数据存储器进行写入，为1时写回存储器。
// output [2:0] MemOp,     // 控制数据存储器读写格式，为010时为4字节读写，为001时为2字节读写带符号扩展，为000时为1字节读写带符号扩展，为101时为2字节读写无符号扩展，为100时为1字节读写无符号扩展。

module CSG(
	input [6:0] op,
	input [2:0] func3,
	input [6:0] func7,
	output [2:0] ExtOP,
	output [0:0] RegWr,
	output [2:0] Branch,
	output [0:0] MemToReg,
	output [0:0] MemRd,
	output [0:0] MemWr,
	output [2:0] MemOp,
	output [0:0] ALUAsrc,
	output [1:0] ALUBsrc,
	output [3:0] ALUctr
);
MuxKey #(9, 5, 20) mux_op (
	.key(op[6:2]),
	.out({ExtOP,RegWr,Branch,MemToReg,MemRd,MemWr,MemOp,ALUAsrc,ALUBsrc,ALUctr}),
	.lut({
		5'b01101, 20'b00110000000000010011,
		5'b00101, 20'b00110000000001010000,
		5'b00100, mux_func3_00100_out,
		5'b01100, mux_func3_01100_out,
		5'b11011, 20'b10010010000001100000,
		5'b11001, mux_func3_11001_out,
		5'b11000, mux_func3_11000_out,
		5'b00000, mux_func3_00000_out,
		5'b01000, mux_func3_01000_out
	})
);
wire [19:0] mux_func3_00100_out;
MuxKey #(8, 3, 20) mux_func3_00100 (
	.key(func3),
	.out(mux_func3_00100_out),
	.lut({
		3'b000, 20'b00010000000000010000,
		3'b010, 20'b00010000000000010010,
		3'b011, 20'b00010000000000011010,
		3'b100, 20'b00010000000000010100,
		3'b110, 20'b00010000000000010110,
		3'b111, 20'b00010000000000010111,
		3'b001, mux_func7_00100_001_out,
		3'b101, mux_func7_00100_101_out
	})
);
wire [19:0] mux_func7_00100_001_out;
MuxKey #(1, 1, 20) mux_func7_00100_001 (
	.key(func7[5:5]),
	.out(mux_func7_00100_001_out),
	.lut({
		1'b0, 20'b00010000000000010001
	})
);
wire [19:0] mux_func7_00100_101_out;
MuxKey #(2, 1, 20) mux_func7_00100_101 (
	.key(func7[5:5]),
	.out(mux_func7_00100_101_out),
	.lut({
		1'b0, 20'b00010000000000010101,
		1'b1, 20'b00010000000000011101
	})
);
wire [19:0] mux_func3_01100_out;
MuxKey #(8, 3, 20) mux_func3_01100 (
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
wire [19:0] mux_func7_01100_000_out;
MuxKey #(2, 1, 20) mux_func7_01100_000 (
	.key(func7[5:5]),
	.out(mux_func7_01100_000_out),
	.lut({
		1'b0, 20'b00010000000000000000,
		1'b1, 20'b00010000000000001000
	})
);
wire [19:0] mux_func7_01100_001_out;
MuxKey #(1, 1, 20) mux_func7_01100_001 (
	.key(func7[5:5]),
	.out(mux_func7_01100_001_out),
	.lut({
		1'b0, 20'b00010000000000000001
	})
);
wire [19:0] mux_func7_01100_010_out;
MuxKey #(1, 1, 20) mux_func7_01100_010 (
	.key(func7[5:5]),
	.out(mux_func7_01100_010_out),
	.lut({
		1'b0, 20'b00010000000000000010
	})
);
wire [19:0] mux_func7_01100_011_out;
MuxKey #(1, 1, 20) mux_func7_01100_011 (
	.key(func7[5:5]),
	.out(mux_func7_01100_011_out),
	.lut({
		1'b0, 20'b00010000000000001010
	})
);
wire [19:0] mux_func7_01100_100_out;
MuxKey #(1, 1, 20) mux_func7_01100_100 (
	.key(func7[5:5]),
	.out(mux_func7_01100_100_out),
	.lut({
		1'b0, 20'b00010000000000000100
	})
);
wire [19:0] mux_func7_01100_101_out;
MuxKey #(2, 1, 20) mux_func7_01100_101 (
	.key(func7[5:5]),
	.out(mux_func7_01100_101_out),
	.lut({
		1'b0, 20'b00010000000000000101,
		1'b1, 20'b00010000000000001101
	})
);
wire [19:0] mux_func7_01100_110_out;
MuxKey #(1, 1, 20) mux_func7_01100_110 (
	.key(func7[5:5]),
	.out(mux_func7_01100_110_out),
	.lut({
		1'b0, 20'b00010000000000000110
	})
);
wire [19:0] mux_func7_01100_111_out;
MuxKey #(1, 1, 20) mux_func7_01100_111 (
	.key(func7[5:5]),
	.out(mux_func7_01100_111_out),
	.lut({
		1'b0, 20'b00010000000000000111
	})
);
wire [19:0] mux_func3_11001_out;
MuxKey #(1, 3, 20) mux_func3_11001 (
	.key(func3),
	.out(mux_func3_11001_out),
	.lut({
		3'b000, 20'b00010100000001100000
	})
);
wire [19:0] mux_func3_11000_out;
MuxKey #(6, 3, 20) mux_func3_11000 (
	.key(func3),
	.out(mux_func3_11000_out),
	.lut({
		3'b000, 20'b01101000000000000010,
		3'b001, 20'b01101010000000000010,
		3'b100, 20'b01101100000000000010,
		3'b101, 20'b01101110000000000010,
		3'b110, 20'b01101100000000001010,
		3'b111, 20'b01101110000000001010
	})
);
wire [19:0] mux_func3_00000_out;
MuxKey #(5, 3, 20) mux_func3_00000 (
	.key(func3),
	.out(mux_func3_00000_out),
	.lut({
		3'b000, 20'b00010001100000010000,
		3'b001, 20'b00010001100010010000,
		3'b010, 20'b00010001100100010000,
		3'b100, 20'b00010001101000010000,
		3'b101, 20'b00010001101010010000
	})
);
wire [19:0] mux_func3_01000_out;
MuxKey #(3, 3, 20) mux_func3_01000 (
	.key(func3),
	.out(mux_func3_01000_out),
	.lut({
		3'b000, 20'b01000000010000010000,
		3'b001, 20'b01000000010010010000,
		3'b010, 20'b01000000010100010000
	})
);
endmodule
