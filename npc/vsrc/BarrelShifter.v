module BarrelShifter (
    input         la,     // 0: logical, 1: arithmetical
    input         lr,     // 0: left, 1: right
    input  [ 4:0] shamt,  // shift amount
    input  [31:0] in,     // 32-bit input
    output [31:0] out     // 32-bit output
);

  wire [31:0] stage1;
  wire [31:0] stage2;
  wire [31:0] stage3;
  wire [31:0] stage4;

  // First stage (shift by 1 bit)
  assign stage1 = (shamt[0]) ? (lr == 0 ? {in[30:0], 1'b0} : {la ? in[31] : 1'b0, in[31:1]}) : in;

  // Second stage (shift by 2 bits)
  assign stage2 = (shamt[1]) ?
                    (lr == 0 ? {stage1[29:0], 2'b0} : {la ? {2{stage1[31]}} : 2'b0, stage1[31:2]}) :
                    stage1;

  // Third stage (shift by 4 bits)
  assign stage3 = (shamt[2]) ?
                    (lr == 0 ? {stage2[27:0], 4'b0} : {la ? {4{stage2[31]}} : 4'b0, stage2[31:4]}) :
                    stage2;

  // Fourth stage (shift by 8 bits)
  assign stage4 = (shamt[3]) ?
                    (lr == 0 ? {stage3[23:0], 8'b0} : {la ? {8{stage3[31]}} : 8'b0, stage3[31:8]}) :
                    stage3;

  // Final stage (shift by 16 bits)
  assign out = (shamt[4]) ?
                 (lr == 0 ? {stage4[15:0], 16'b0} : {la ? {16{stage4[31]}} : 16'b0, stage4[31:16]}) :
                 stage4;

endmodule
