module ALU(
        input [31:0] A,
        input [31:0] B,
        input [3:0] ctr,
        output [31:0] out
    );

    assign out = A + B;

endmodule
