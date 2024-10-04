/*
ALUctr[3]	ALUctr[2:0] 	ALU操作
0	        000	            选择加法器输出，做加法
1	        000	            选择加法器输出，做减法
×	        001	            选择移位器输出，左移
0	        010	            做减法，选择带符号小于置位结果输出, Less按带符号结果设置
1	        010	            做减法，选择无符号小于置位结果输出, Less按无符号结果设置
×	        011	            选择ALU输入B的结果直接输出
×	        100	            选择异或输出
0	        101	            选择移位器输出，逻辑右移
1	        101	            选择移位器输出，算术右移
×	        110	            选择逻辑或输出
×	        111	            选择逻辑与输出
*/

module ALU(
        input [31:0] A,
        input [31:0] B,
        input [3:0] ctr,
        output [31:0] out,
        output Less,
        output Zero
    );

    assign Less = ctr[3] == 0 ? A < B : $signed(A) < $signed(B);
    assign Zero = A == B;

    wire [31:0] adder_out;
    MuxKey #(2, 1, 32) mux_adder (
        .out(adder_out),
        .key(ctr[3]),
        .lut({
            1'b0, A + B,
            1'b1, A - B
        })
    );

    wire [31:0] shifter_out;
    BarrelShifter shifter(
        .la(ctr[3]),
        .lr(ctr[2]),
        .shamt(B[4:0]),
        .in(A),
        .out(shifter_out)
    );

    MuxKey #(8, 3, 32) mux_top (
        .out(out),
        .key(ctr[2:0]),
        .lut({
                3'b000, adder_out,
                3'b001, shifter_out,
                3'b010, {31'b0, Less},
                3'b011, B,
                3'b100, A ^ B,
                3'b101, shifter_out,
                3'b110, A | B,
                3'b111, A & B
        })
    );

endmodule
