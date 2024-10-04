// PCAsrc控制PC加法器输入A的信号，为0时选择常数4，为1时选择imm
// PCBsrc控制PC加法器输入B的信号，为0时选择本周期PC，为1时选择寄存器rs1

module BranchCond(
    input [2:0] Branch,
    input Less,
    input Zero,
    output PCAsrc,
    output PCBsrc
);
    wire [1:0] zero_ctr = ^{Branch, Zero} ? 2'b00 : 2'b10;
    wire [1:0] less_ctr = ^{Branch, Less} ? 2'b00 : 2'b10;
    always @(*) $display(Branch, ", ", Zero, ", ", ^{Branch, Zero}, ", ", PCAsrc, PCBsrc);
    always @(*) $display(Branch, ", ", Less, ", ", ^{Branch, Less}, ", ", PCAsrc, PCBsrc);
    MuxKey #(7, 3, 2) mux_br(
        .key(Branch),
        .out({PCAsrc, PCBsrc}),
        .lut({
            3'b000, 2'b00,
            3'b001, 2'b10,
            3'b010, 2'b11,
            3'b100, zero_ctr,
            3'b101, zero_ctr,
            3'b110, less_ctr,
            3'b111, less_ctr
        })
    );
endmodule