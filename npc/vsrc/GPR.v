module GPR (
    input rst,

    // write port
    input wr_clk,
    input wr_en,
    input [4:0] wr_reg,
    input [31:0] wr_bus,

    // read port 1
    input  [ 4:0] rd_reg_a,
    output [31:0] rd_bus_a,

    // read port 2
    input  [ 4:0] rd_reg_b,
    output [31:0] rd_bus_b
);

  reg [31:0] gpr[31:0]  /* verilator public */;

  assign rd_bus_a = rd_reg_a == 0 ? 0 : gpr[rd_reg_a];
  assign rd_bus_b = rd_reg_b == 0 ? 0 : gpr[rd_reg_b];

  always @(posedge wr_clk) begin
    if (rst) begin
      for (int i = 0; i < 32; ++i) begin
        gpr[i] <= 0;
      end
    end else if (wr_en) begin
      gpr[wr_reg] <= wr_bus;
    end
  end

endmodule
