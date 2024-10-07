// Instruction Decode Unit
module IDU(
        input [31:0] inst,
        input [2:0] ExtOP,
        output [6:0] op,
        output [2:0] func3,
        output [6:0] func7,
        output [4:0] rd,
        output [4:0] rs1,
        output [4:0] rs2,
        output [31:0] imm
    );

    assign op    = inst[6:0];
    assign func3 = inst[14:12];
    assign func7 = inst[31:25];
    assign rd    = inst[11:7];
    assign rs1   = inst[19:15];
    assign rs2   = inst[24:20];

    wire [31:0] immI, immU, immS, immB, immJ;
    assign immI = {{20{inst[31]}}, inst[31:20]};
    assign immU = {inst[31:12], 12'b0};
    assign immS = {{20{inst[31]}}, inst[31:25], inst[11:7]};
    assign immB = {{20{inst[31]}}, inst[7], inst[30:25], inst[11:8], 1'b0};
    assign immJ = {{12{inst[31]}}, inst[19:12], inst[20], inst[30:21], 1'b0};

    MuxKey #(5, 3, 32) mux_imm (
               .out(imm),
               .key(ExtOP),
               .lut({
                        3'b000, immI,
                        3'b001, immU,
                        3'b010, immS,
                        3'b011, immB,
                        3'b100, immJ
                    })
           );

endmodule
