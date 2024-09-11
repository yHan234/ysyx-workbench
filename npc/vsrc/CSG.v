// Control Signal Generator
module CSG(
        input [6:0] op,
        input [2:0] func3,
        input [6:0] func7,
        output [2:0] ExtOP,     // 选择立即数产生器的输出类型，具体见代码。
        output RegWr,           // 控制是否对寄存器rd进行写回，为1时写回寄存器。
        output ALUAsrc,         // 选择ALU输入端A的来源。为0时选择rs1，为1时选择PC。
        output [1:0] ALUBsrc,   // 选择ALU输入端B的来源。为00时选择rs2，为01时选择imm(当是立即数移位指令时，只有低5位有效)，为10时选择常数4（用于跳转时计算返回地址PC+4）。
        output [3:0] ALUctr,    // 选择ALU执行的操作，具体见ALU代码。
        output [2:0] Branch,    // 说明分支和跳转的种类，用于生成最终的分支控制信号，具体见代码。
        output MemtoReg,        // 选择寄存器rd写回数据来源，为0时选择ALU输出，为1时选择数据存储器输出。
        output MemWr,           // 控制是否对数据存储器进行写入，为1时写回存储器。
        output [2:0] MemOP      // 控制数据存储器读写格式，为010时为4字节读写，为001时为2字节读写带符号扩展，为000时为1字节读写带符号扩展，为101时为2字节读写无符号扩展，为100时为1字节读写无符号扩展。
    );

    // ExtOp: I 000, U 001, S 010, B 011, J 100
    MuxKeyWithDefault #(1, 7, 3) mux_extop (
                          .out(ExtOP),
                          .key(op),
                          .default_out(3'b111),
                          .lut({
                                   7'b0010011, 3'b000
                               })
                      );

    assign RegWr = 1'b1;
    assign ALUAsrc = 1'b0;
    assign ALUBsrc = 2'b01;
    assign ALUctr = 4'b0000;
    assign Branch = 3'b000;
    assign MemtoReg = 1'b0;
    assign MemWr = 1'b0;
    assign MemOP = 3'b000;

    always @(*) begin
        $display("op: ", op);
        $display("ExtOP: ", ExtOP);
    end

endmodule
