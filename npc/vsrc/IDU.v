// Instruction Decode Unit
module IDU(
        input [31:0] instr,
        input [2:0] ExtOP,
        output [6:0] op,
        output [2:0] func3,
        output [6:0] func7,
        output [4:0] rd,
        output [4:0] rs1,
        output [4:0] rs2,
        output [31:0] imm
    );

    assign op    = instr[6:0];
    assign func3 = instr[14:12];
    assign func7 = instr[31:25];
    assign rd    = instr[11:7];
    assign rs1   = instr[19:15];
    assign rs2   = instr[24:20];

    wire [31:0] immI, immU, immS, immB, immJ;
    assign immI = {{20{instr[31]}}, instr[31:20]};
    assign immU = {instr[31:12], 12'b0};
    assign immS = {{20{instr[31]}}, instr[31:25], instr[11:7]};
    assign immB = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};
    assign immJ = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};

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

    always @(*) begin
        $display("instr: %b", instr);
        $display("immI: ", immI);
        $display("imm: ", imm);
    end

endmodule
