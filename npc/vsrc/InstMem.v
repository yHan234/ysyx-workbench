import "DPI-C" function int get_inst();
import "DPI-C" function void ebreak();

module InstMem (
    input rst,
    input [31:0] pc,
    output reg [31:0] inst
);

    always @(*) begin
        if (!rst)
            inst = get_inst();
        else
            inst = 0;

        if (inst == 32'b00000000000100000000000001110011) begin
            ebreak();
        end
    end

endmodule