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
        output [31:0] out
    );



    wire [31:0] add_sub;
    MuxKey #(2, 1, 32) mux_add_sub (
        .out(add_sub),
        .key(ctr[3]),
        .lut({
            1'b0, A + B,
            1'b1, A - B
        })
    );

    MuxKey #(8, 3, 32) mux_top (
        .out(out),
        .key(ctr[2:0]),
        .lut({
                3'b000, add_sub,
                3'b001, 32'd0,
                3'b010, 32'd0,
                3'b011, 32'd0,
                3'b100, 32'd0,
                3'b101, 32'd0,
                3'b110, 32'd0,
                3'b111, 32'd0
        })
    );

endmodule
