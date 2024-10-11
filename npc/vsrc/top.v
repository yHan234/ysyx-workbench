module top (
    input clk,
    input rst
);

  // PC

  wire [31:0] pc_next, pc  /* verilator public */;
  wire pc_src_a, pc_src_b;
  BranchCond bc (
      .branch  (branch),
      .less    (less),
      .zero    (zero),
      .pc_src_a(pc_src_a),
      .pc_src_b(pc_src_b)
  );
  Reg #(32, 32'h80000000) pc_r (
      .clk (clk),
      .rst (rst),
      .din (pc_next),
      .dout(pc),
      .wen (1'b1)
  );
  assign pc_next = (pc_src_a == 0 ? 4 : imm) + (pc_src_b == 0 ? pc : rbus1);

  // GPR

  wire [31:0] rbus1, rbus2;

  GPR gpr (
      .rst     (rst),
      .wr_clk  (clk),
      .wr_en   (reg_wr_en),
      .wr_reg  (rd),
      .wr_bus  (mem2reg ? mem_out : alu_out),
      .rd_reg_a(rs1),
      .rd_bus_a(rbus1),
      .rd_reg_b(rs2),
      .rd_bus_b(rbus2)
  );

  // Instruction Memory

  wire [31:0] inst  /* verilator public */;

  InstMem inst_mem (
      .rst (rst),
      .pc  (pc),
      .inst(inst)
  );

  // Instruction Decode

  wire [ 2:0] inst_type;
  wire [ 6:0] op;
  wire [ 2:0] func3;
  wire [ 6:0] func7;
  wire [ 4:0] rd;
  wire [ 4:0] rs1;
  wire [ 4:0] rs2;
  wire [31:0] imm;

  IDU idu (
      .inst     (inst),
      .inst_type(inst_type),
      .op       (op),
      .func3    (func3),
      .func7    (func7),
      .rd       (rd),
      .rs1      (rs1),
      .rs2      (rs2),
      .imm      (imm)
  );


  // Control Signal Generate

  wire       reg_wr_en;
  wire       alu_src_a;
  wire [1:0] alu_src_b;
  wire [3:0] alu_ctr;
  wire [2:0] branch;
  wire       mem2reg;
  wire       mem_rd_en;
  wire       mem_wr_en;
  wire [2:0] mem_op;

  CSG csg (
      .op       (op),
      .func3    (func3),
      .func7    (func7),
      .inst_type(inst_type),
      .reg_wr_en(reg_wr_en),
      .alu_src_a(alu_src_a),
      .alu_src_b(alu_src_b),
      .alu_ctr  (alu_ctr),
      .branch   (branch),
      .mem2reg  (mem2reg),
      .mem_rd_en(mem_rd_en),
      .mem_wr_en(mem_wr_en),
      .mem_op   (mem_op)
  );


  // ALU

  wire [31:0] alu_out;
  wire less, zero;

  ALU alu (
      .a   (alu_src_a ? pc : rbus1),
      .b   (alu_src_b == 2'b00 ? rbus2 : alu_src_b == 2'b01 ? imm : alu_src_b == 2'b10 ? 4 : 0),
      .ctr (alu_ctr),
      .out (alu_out),
      .less(less),
      .zero(zero)
  );

  // Data Memory

  wire [31:0] mem_out;
  DataMem data_mem (
      .addr  (alu_out),
      .rd_clk(clk),
      .rd_en (mem_rd_en),
      .out   (mem_out),
      .wr_clk(clk),
      .wr_en (mem_wr_en),
      .op    (mem_op),
      .in    (rbus2)
  );

endmodule
