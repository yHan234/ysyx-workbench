module GPR (
        input rst,

        // write port
        input WrClk,
        input RegWr,
        input [4:0] Rw,
        input [31:0] busW,

        // read port 1
        input [4:0] Ra,
        output [31:0] busA,

        // read port 2
        input [4:0] Rb,
        output [31:0] busB
    );

    reg [31:0] gpr [31:0] /* verilator public */;

    assign busA = Ra == 0 ? 0 : gpr[Ra];
    assign busB = Rb == 0 ? 0 : gpr[Rb];

    always @(posedge WrClk) begin
        if (rst) begin
            for (int i = 0; i < 32; ++i) begin
                gpr[i] <= 0;
            end
        end
        else if (RegWr) begin
            gpr[Rw] <= busW;
        end
    end

endmodule
