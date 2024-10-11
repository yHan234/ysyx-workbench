module CSG(
	input [6:0] op,
	input [2:0] funct3,
	input [6:0] funct7,
	output [2:0] inst_type,
	output [0:0] gpr_wr_en,
	output [1:0] gpr_wr_src,
	output [2:0] branch,
	output [0:0] mem_rd_en,
	output [0:0] mem_wr_en,
	output [2:0] mem_op,
	output [0:0] alu_src_a,
	output [1:0] alu_src_b,
	output [3:0] alu_ctr,
	output [0:0] csr_wr_en,
	output [0:0] csr_wr_set
);
MuxKey #(10, 5, 23) mux_op (
	.key(op[6:2]),
	.out({inst_type, gpr_wr_en, gpr_wr_src, branch, mem_rd_en, mem_wr_en, mem_op, alu_src_a, alu_src_b, alu_ctr, csr_wr_en, csr_wr_set}),
	.lut({
		5'b01101, 23'b00110000000000001001100,
		5'b00101, 23'b00110000000000101000000,
		5'b00100, mux_funct3_00100_out,
		5'b01100, mux_funct3_01100_out,
		5'b11011, 23'b10010000100000110000000,
		5'b11001, mux_funct3_11001_out,
		5'b11000, mux_funct3_11000_out,
		5'b00000, mux_funct3_00000_out,
		5'b01000, mux_funct3_01000_out,
		5'b11100, mux_funct3_11100_out
	})
);
wire [22:0] mux_funct3_00100_out;
MuxKey #(8, 3, 23) mux_funct3_00100 (
	.key(funct3),
	.out(mux_funct3_00100_out),
	.lut({
		3'b000, 23'b00010000000000001000000,
		3'b010, 23'b00010000000000001001000,
		3'b011, 23'b00010000000000001101000,
		3'b100, 23'b00010000000000001010000,
		3'b110, 23'b00010000000000001011000,
		3'b111, 23'b00010000000000001011100,
		3'b001, mux_funct7_00100_001_out,
		3'b101, mux_funct7_00100_101_out
	})
);
wire [22:0] mux_funct7_00100_001_out;
MuxKey #(1, 1, 23) mux_funct7_00100_001 (
	.key(funct7[5:5]),
	.out(mux_funct7_00100_001_out),
	.lut({
		1'b0, 23'b00010000000000001000100
	})
);
wire [22:0] mux_funct7_00100_101_out;
MuxKey #(2, 1, 23) mux_funct7_00100_101 (
	.key(funct7[5:5]),
	.out(mux_funct7_00100_101_out),
	.lut({
		1'b0, 23'b00010000000000001010100,
		1'b1, 23'b00010000000000001110100
	})
);
wire [22:0] mux_funct7_01100_000_out;
MuxKey #(2, 1, 23) mux_funct7_01100_000 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_000_out),
	.lut({
		1'b0, 23'b00010000000000000000000,
		1'b1, 23'b00010000000000000100000
	})
);
wire [22:0] mux_funct3_01100_out;
MuxKey #(8, 3, 23) mux_funct3_01100 (
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
wire [22:0] mux_funct7_01100_001_out;
MuxKey #(1, 1, 23) mux_funct7_01100_001 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_001_out),
	.lut({
		1'b0, 23'b00010000000000000000100
	})
);
wire [22:0] mux_funct7_01100_010_out;
MuxKey #(1, 1, 23) mux_funct7_01100_010 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_010_out),
	.lut({
		1'b0, 23'b00010000000000000001000
	})
);
wire [22:0] mux_funct7_01100_011_out;
MuxKey #(1, 1, 23) mux_funct7_01100_011 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_011_out),
	.lut({
		1'b0, 23'b00010000000000000101000
	})
);
wire [22:0] mux_funct7_01100_100_out;
MuxKey #(1, 1, 23) mux_funct7_01100_100 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_100_out),
	.lut({
		1'b0, 23'b00010000000000000010000
	})
);
wire [22:0] mux_funct7_01100_101_out;
MuxKey #(2, 1, 23) mux_funct7_01100_101 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_101_out),
	.lut({
		1'b0, 23'b00010000000000000010100,
		1'b1, 23'b00010000000000000110100
	})
);
wire [22:0] mux_funct7_01100_110_out;
MuxKey #(1, 1, 23) mux_funct7_01100_110 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_110_out),
	.lut({
		1'b0, 23'b00010000000000000011000
	})
);
wire [22:0] mux_funct7_01100_111_out;
MuxKey #(1, 1, 23) mux_funct7_01100_111 (
	.key(funct7[5:5]),
	.out(mux_funct7_01100_111_out),
	.lut({
		1'b0, 23'b00010000000000000011100
	})
);
wire [22:0] mux_funct3_11001_out;
MuxKey #(1, 3, 23) mux_funct3_11001 (
	.key(funct3),
	.out(mux_funct3_11001_out),
	.lut({
		3'b000, 23'b00010001000000110000000
	})
);
wire [22:0] mux_funct3_11000_out;
MuxKey #(6, 3, 23) mux_funct3_11000 (
	.key(funct3),
	.out(mux_funct3_11000_out),
	.lut({
		3'b000, 23'b01100010000000000001000,
		3'b001, 23'b01100010100000000001000,
		3'b100, 23'b01100011000000000001000,
		3'b101, 23'b01100011100000000001000,
		3'b110, 23'b01100011000000000101000,
		3'b111, 23'b01100011100000000101000
	})
);
wire [22:0] mux_funct3_00000_out;
MuxKey #(5, 3, 23) mux_funct3_00000 (
	.key(funct3),
	.out(mux_funct3_00000_out),
	.lut({
		3'b000, 23'b00010100010000001000000,
		3'b001, 23'b00010100010001001000000,
		3'b010, 23'b00010100010010001000000,
		3'b100, 23'b00010100010100001000000,
		3'b101, 23'b00010100010101001000000
	})
);
wire [22:0] mux_funct3_01000_out;
MuxKey #(3, 3, 23) mux_funct3_01000 (
	.key(funct3),
	.out(mux_funct3_01000_out),
	.lut({
		3'b000, 23'b01000000001000001000000,
		3'b001, 23'b01000000001001001000000,
		3'b010, 23'b01000000001010001000000
	})
);
wire [22:0] mux_funct3_11100_out;
MuxKey #(2, 3, 23) mux_funct3_11100 (
	.key(funct3),
	.out(mux_funct3_11100_out),
	.lut({
		3'b001, 23'b00011000000000000000010,
		3'b010, 23'b00011000000000000000011
	})
);
endmodule
