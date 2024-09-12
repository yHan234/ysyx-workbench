import "DPI-C" function void pmem_read(input int pc, output int instr);

module InstrMem (
    input [31:0] pc,
    output [31:0] instr
);

    always @(*) begin
        pmem_read(pc, instr);
    end

endmodule