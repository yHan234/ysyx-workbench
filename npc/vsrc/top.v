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
      .wr_en   (gpr_wr_en),
      .wr_reg  (rd),
      .wr_bus  (gpr_wr_src == 2'b00 ? alu_out : gpr_wr_src == 2'b01 ? mem_out : csr_bus),
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

  wire [ 6:0] op;
  wire [ 2:0] funct3;
  wire [ 6:0] funct7;
  wire [ 4:0] rd;
  wire [ 4:0] rs1;
  wire [ 4:0] rs2;
  wire [31:0] imm;

  IDU idu (
      .inst     (inst),
      .inst_type(inst_type),
      .op       (op),
      .funct3   (funct3),
      .funct7   (funct7),
      .rd       (rd),
      .rs1      (rs1),
      .rs2      (rs2),
      .imm      (imm)
  );


  // Control Signal Generate

  wire [2:0] inst_type;
  wire       gpr_wr_en;
  wire [1:0] gpr_wr_src;
  wire [2:0] branch;
  wire       mem_rd_en;
  wire       mem_wr_en;
  wire [2:0] mem_op;
  wire       alu_src_a;
  wire [1:0] alu_src_b;
  wire [3:0] alu_ctr;
  wire       csr_wr_en;
  wire       csr_wr_set;

  CSG csg (
      .op        (op),
      .funct3    (funct3),
      .funct7    (funct7),
      .inst_type (inst_type),
      .gpr_wr_en (gpr_wr_en),
      .gpr_wr_src(gpr_wr_src),
      .branch    (branch),
      .mem_rd_en (mem_rd_en),
      .mem_wr_en (mem_wr_en),
      .mem_op    (mem_op),
      .alu_src_a (alu_src_a),
      .alu_src_b (alu_src_b),
      .alu_ctr   (alu_ctr),
      .csr_wr_en (csr_wr_en),
      .csr_wr_set(csr_wr_set)
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

  // CSR

  wire [31:0] csr_bus;

  CSR csr (
      .rst   (rst),
      .wr_clk(clk),
      .wr_en (csr_wr_en),
      .wr_set(csr_wr_set),
      .wr_reg(imm[11:0]),
      .wr_bus(rbus1),
      .rd_reg(imm[11:0]),
      .rd_bus(csr_bus)
  );

endmodule
