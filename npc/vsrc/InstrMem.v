import "DPI-C" function void pmem_read(input int pc, output int instr);
import "DPI-C" function void ebreak();

module InstrMem (
    input [31:0] pc,
    output [31:0] instr
);

    always @(*) begin
        pmem_read(pc, instr);
        $display("PC: ", pc, " instr: %b", instr);

        if (instr == 32'b00000000000100000000000001110011) begin
            ebreak();
        end
    end

endmodule