// pc_src_a控制PC加法器输入A的信号，为0时选择常数4，为1时选择imm
// pc_src_b控制PC加法器输入B的信号，为0时选择本周期PC，为1时选择寄存器rs1

module BranchCond (
    input [2:0] branch,
    input less,
    input zero,
    output pc_src_a,
    output pc_src_b
);
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
endmodule
