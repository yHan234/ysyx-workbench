import "DPI-C" function void get_inst(output int inst);
import "DPI-C" function void ebreak();

module InstrMem (
    input rst,
    input [31:0] pc,
    output reg [31:0] inst
);

    always @(*) begin
        if (!rst)
            get_inst(inst);
        else
            inst = 0;

        if (inst == 32'b00000000000100000000000001110011) begin
            ebreak();
        end
    end

endmodule