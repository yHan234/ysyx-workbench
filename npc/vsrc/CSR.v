module CSR (
    input rst,

    // write port
    input wr_clk,
    input wr_en,
    input wr_set,
    input [11:0] wr_reg,
    input [31:0] wr_bus,

    // read port
    input [11:0] rd_reg,
    output reg [31:0] rd_bus,

    // expose for BranchCond
    output reg[31:0] mtvec,
    output reg[31:0] mepc
);

  reg [31:0] mstatus;
  reg [31:0] mcause;

  always @(*) begin
    case (rd_reg)
      12'h300: rd_bus = mstatus;
      12'h305: rd_bus = mtvec;
      12'h341: rd_bus = mepc;
      12'h342: rd_bus = mcause;
      default: rd_bus = 0;
    endcase
  end

  always @(posedge wr_clk) begin
    if (rst) begin
      mstatus = 32'h1800; // MPP 设为 M 模式
      mtvec   = 0;
      mepc    = 0;
      mcause  = 0;
    end else if (wr_en) begin
      if (wr_set)
        case (wr_reg)
          12'h300: mstatus |= wr_bus;
          12'h305: mtvec |= wr_bus;
          12'h341: mepc |= wr_bus;
          12'h342: mcause |= wr_bus;
          default: ;
        endcase
      else
        case (wr_reg)
          12'h300: mstatus = wr_bus;
          12'h305: mtvec = wr_bus;
          12'h341: mepc = wr_bus;
          12'h342: mcause = wr_bus;
          default: ;
        endcase
    end
  end

endmodule
