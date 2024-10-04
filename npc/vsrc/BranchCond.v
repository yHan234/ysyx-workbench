module BranchCond(
    input [2:0] Branch,
    input Less,
    input Zero,
    output PCAsrc,
    output PCBsrc
);
    wire [1:0] zero_ctr = ^{Branch, Zero} == 1 ? 2'b00 : 2'b10;
    wire [1:0] less_ctr = ^{Branch, Zero} == 0 ? 2'b00 : 2'b10;
    MuxKey #(6, 3, 2) mux_br(
        .key(Branch),
        .out({PCAsrc, PCBsrc}),
        .lut({
            3'b000, 2'b00,
            3'b001, 2'b10,
            3'b100, zero_ctr,
            3'b101, zero_ctr,
            3'b110, less_ctr,
            3'b111, less_ctr
        })
    );
endmodule