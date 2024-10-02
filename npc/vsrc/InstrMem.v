import "DPI-C" function void get_inst(output int inst);
import "DPI-C" function void ebreak();

module InstrMem (
    input [31:0] pc,
    output [31:0] inst
);

    always @(*) begin
        get_inst(inst);
        $display("PC: ", pc, " inst: %b", inst);

        if (inst == 32'b00000000000100000000000001110011) begin
            ebreak();
        end
    end

endmodule