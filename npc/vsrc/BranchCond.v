// branch
// 000 不跳转
// 001 无条件跳转 PC 目标
// 010 无条件跳转 rs1 目标

// 100  zero 跳转
// 101 !zero 跳转
// 110  less 跳转
// 111 !less 跳转

module BranchCond (
    input [2:0] branch,
    input less,
    input zero,
    input [31:0] pc,
    input [31:0] imm,
    input [31:0] rbus1,
    input [1:0] csr_branch,
    input [31:0] mtvec,
    input [31:0] mepc,
    output [31:0] next_pc
);
  wire pc_src_a, pc_src_b;
  wire [1:0] zero_ctr = branch[0] ^ zero ? 2'b10 : 2'b00;
  wire [1:0] less_ctr = branch[0] ^ less ? 2'b10 : 2'b00;
  MuxKey #(7, 3, 2) mux_br (
      .key(branch),
      .out({pc_src_a, pc_src_b}),
      .lut({
        3'b000, 2'b00,
        3'b001, 2'b10,
        3'b010, 2'b11,
        3'b100, zero_ctr,
        3'b101, zero_ctr,
        3'b110, less_ctr,
        3'b111, less_ctr
      })
  );

  MuxKey #(3, 2, 32) mux_next_pc (
      .key(csr_branch),
      .out(next_pc),
      .lut({
        2'b00, (pc_src_a == 0 ? 4 : imm) + (pc_src_b == 0 ? pc : rbus1),
        2'b01, mtvec,
        2'b10, mepc
      })
  );
endmodule
