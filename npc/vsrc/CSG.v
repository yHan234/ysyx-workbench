module CSG(
	input [6:0] op,
	input [2:0] funct3,
	input [6:0] funct7,
	output [2:0] inst_type,
	output [0:0] reg_wr_en,
	output [2:0] branch,
	output [0:0] mem2reg,
	output [0:0] mem_rd_en,
	output [0:0] mem_wr_en,
	output [2:0] mem_op,
	output [0:0] alu_src_a,
	output [1:0] alu_src_b,
	output [3:0] alu_ctr
);
MuxKey #(9, 5, 20) mux_op (
	.key(op[6:2]),
	.out({inst_type, reg_wr_en, branch, mem2reg, mem_rd_en, mem_wr_en, mem_op, alu_src_a, alu_src_b, alu_ctr}),
	.lut({
		5'b01101, 20'b00110000000000010011,
		5'b00101, 20'b00110000000001010000,
		5'b00100, mux_funct3_00100_out,
		5'b01100, mux_funct3_01100_out,
		5'b11011, 20'b10010010000001100000,
		5'b11001, mux_funct3_11001_out,
		5'b11000, mux_funct3_11000_out,
		5'b00000, mux_funct3_00000_out,
		5'b01000, mux_funct3_01000_out
	})
);
wire [19:0] mux_funct3_00100_out;
MuxKey #(8, 3, 20) mux_funct3_00100 (
	.key(funct3),
	.out(mux_funct3_00100_out),
	.lut({
		3'b000, 20'b00010000000000010000,
		3'b010, 20'b00010000000000010010,
		3'b011, 20'b00010000000000011010,
		3'b100, 20'b00010000000000010100,
		3'b110, 20'b00010000000000010110,
		3'b111, 20'b00010000000000010111,
		3'b001, mux_funct7_00100_001_out,
		3'b101, mux_funct7_00100_101_out
	})
);
wire [19:0] mux_funct7_00100_001_out;
MuxKey #(1, 1, 20) mux_funct7_00100_001 (
	.key(funct7[5:5]),
	.out(mux_funct7_00100_001_out),
	.lut({
		1'b0, 20'b00010000000000010001
	})
);
wire [19:0] mux_funct7_00100_101_out;
MuxKey #(2, 1, 20) mux_funct7_00100_101 (
	.key(funct7[5:5]),
	.out(mux_funct7_00100_101_out),
	.lut({
		1'b0, 20'b00010000000000010101,
		1'b1, 20'b00010000000000011101
	})
);
wire [19:0] mux_funct7_01100_000_out;
MuxKey #(2, 1, 20) mux_funct7_01100_000 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_000_out),
	.lut({
		1'b0, 20'b00010000000000000000,
		1'b1, 20'b00010000000000001000
	})
);
wire [19:0] mux_funct3_01100_out;
MuxKey #(8, 3, 20) mux_funct3_01100 (
	.key(funct3),
	.out(mux_funct3_01100_out),
	.lut({
		3'b000, mux_funct7_01100_000_out,
		3'b001, mux_funct7_01100_001_out,
		3'b010, mux_funct7_01100_010_out,
		3'b011, mux_funct7_01100_011_out,
		3'b100, mux_funct7_01100_100_out,
		3'b101, mux_funct7_01100_101_out,
		3'b110, mux_funct7_01100_110_out,
		3'b111, mux_funct7_01100_111_out
	})
);
wire [19:0] mux_funct7_01100_001_out;
MuxKey #(1, 1, 20) mux_funct7_01100_001 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_001_out),
	.lut({
		1'b0, 20'b00010000000000000001
	})
);
wire [19:0] mux_funct7_01100_010_out;
MuxKey #(1, 1, 20) mux_funct7_01100_010 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_010_out),
	.lut({
		1'b0, 20'b00010000000000000010
	})
);
wire [19:0] mux_funct7_01100_011_out;
MuxKey #(1, 1, 20) mux_funct7_01100_011 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_011_out),
	.lut({
		1'b0, 20'b00010000000000001010
	})
);
wire [19:0] mux_funct7_01100_100_out;
MuxKey #(1, 1, 20) mux_funct7_01100_100 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_100_out),
	.lut({
		1'b0, 20'b00010000000000000100
	})
);
wire [19:0] mux_funct7_01100_101_out;
MuxKey #(2, 1, 20) mux_funct7_01100_101 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_101_out),
	.lut({
		1'b0, 20'b00010000000000000101,
		1'b1, 20'b00010000000000001101
	})
);
wire [19:0] mux_funct7_01100_110_out;
MuxKey #(1, 1, 20) mux_funct7_01100_110 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_110_out),
	.lut({
		1'b0, 20'b00010000000000000110
	})
);
wire [19:0] mux_funct7_01100_111_out;
MuxKey #(1, 1, 20) mux_funct7_01100_111 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_111_out),
	.lut({
		1'b0, 20'b00010000000000000111
	})
);
wire [19:0] mux_funct3_11001_out;
MuxKey #(1, 3, 20) mux_funct3_11001 (
	.key(funct3),
	.out(mux_funct3_11001_out),
	.lut({
		3'b000, 20'b00010100000001100000
	})
);
wire [19:0] mux_funct3_11000_out;
MuxKey #(6, 3, 20) mux_funct3_11000 (
	.key(funct3),
	.out(mux_funct3_11000_out),
	.lut({
		3'b000, 20'b01101000000000000010,
		3'b001, 20'b01101010000000000010,
		3'b100, 20'b01101100000000000010,
		3'b101, 20'b01101110000000000010,
		3'b110, 20'b01101100000000001010,
		3'b111, 20'b01101110000000001010
	})
);
wire [19:0] mux_funct3_00000_out;
MuxKey #(5, 3, 20) mux_funct3_00000 (
	.key(funct3),
	.out(mux_funct3_00000_out),
	.lut({
		3'b000, 20'b00010001100000010000,
		3'b001, 20'b00010001100010010000,
		3'b010, 20'b00010001100100010000,
		3'b100, 20'b00010001101000010000,
		3'b101, 20'b00010001101010010000
	})
);
wire [19:0] mux_funct3_01000_out;
MuxKey #(3, 3, 20) mux_funct3_01000 (
	.key(funct3),
	.out(mux_funct3_01000_out),
	.lut({
		3'b000, 20'b01000000010000010000,
		3'b001, 20'b01000000010010010000,
		3'b010, 20'b01000000010100010000
	})
);
endmodule
