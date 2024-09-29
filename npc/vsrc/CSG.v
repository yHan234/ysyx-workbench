// Control Signal Generator
module CSG(
        input [6:0] op,
        input [2:0] func3,
        input [6:0] func7,
        output reg [2:0] ExtOP,     // 选择立即数产生器的输出类型，具体见代码。
        output reg RegWr,           // 控制是否对寄存器rd进行写回，为1时写回寄存器。
        output reg [2:0] Branch,    // 说明分支和跳转的种类，用于生成最终的分支控制信号，具体见代码。
        output reg MemtoReg,        // 选择寄存器rd写回数据来源，为0时选择ALU输出，为1时选择数据存储器输出。
        output reg MemWr,           // 控制是否对数据存储器进行写入，为1时写回存储器。
        output reg [2:0] MemOP,     // 控制数据存储器读写格式，为010时为4字节读写，为001时为2字节读写带符号扩展，为000时为1字节读写带符号扩展，为101时为2字节读写无符号扩展，为100时为1字节读写无符号扩展。
        output reg ALUAsrc,         // 选择ALU输入端A的来源。为0时选择rs1，为1时选择PC。
        output reg [1:0] ALUBsrc,   // 选择ALU输入端B的来源。为00时选择rs2，为01时选择imm(当是立即数移位指令时，只有低5位有效)，为10时选择常数4（用于跳转时计算返回地址PC+4）。
        output reg [3:0] ALUctr     // 选择ALU执行的操作，具体见ALU代码。
    );

    always @(*) begin
        ExtOP = 0;
        RegWr = 0;
        Branch = 0;
        MemtoReg = 0;
        MemWr = 0;
        MemOP = 0;
        ALUAsrc = 0;
        ALUBsrc = 0;
        ALUctr = 0;
        case (op)
            7'b0010011: begin
                case (func3)
                    3'b000: begin // addi
                        ExtOP = 3'b000;
                        RegWr = 1'b1;
                        Branch = 3'b000;
                        MemtoReg = 1'b0;
                        MemWr = 1'b0;
                        MemOP = 3'b000;
                        ALUAsrc = 1'b0;
                        ALUBsrc = 2'b01;
                        ALUctr = 4'b0000;
                    end
                    default:;
                endcase
            end
            7'b0010111: begin // auipc
                ExtOP = 3'b001;
                RegWr = 1'b1;
                Branch = 3'b000;
                MemtoReg = 1'b0;
                MemWr = 1'b0;
                MemOP = 3'b000;
                ALUAsrc = 1'b1;
                ALUBsrc = 2'b01;
                ALUctr = 4'b0000;
            end
            7'b0110111: begin // lui
                ExtOP = 3'b001;
                RegWr = 1'b1;
                Branch = 3'b000;
                MemtoReg = 1'b0;
                MemWr = 1'b0;
                MemOP = 3'b000;
                ALUAsrc = 1'b1;
                ALUBsrc = 2'b01;
                ALUctr = 4'b0011;
            end
            7'b1101111: begin // jal
                ExtOP = 3'b100;
                RegWr = 1'b1;
                Branch = 3'b001;
                MemtoReg = 1'b0;
                MemWr = 1'b0;
                MemOP = 3'b000;
                ALUAsrc = 1'b1;
                ALUBsrc = 2'b10;
                ALUctr = 4'b0000;
            end
            7'b1100111: begin // jalr
                case (func3)
                    3'b000: begin
                        ExtOP = 3'b000;
                        RegWr = 1'b1;
                        Branch = 3'b010;
                        MemtoReg = 1'b0;
                        MemWr = 1'b0;
                        MemOP = 3'b000;
                        ALUAsrc = 1'b1;
                        ALUBsrc = 2'b10;
                        ALUctr = 4'b0000;
                    end
                    default:;
                endcase
            end
            default:;
        endcase
        $display("op: ", op);
        $display("ExtOP: ", ExtOP);
    end

endmodule
