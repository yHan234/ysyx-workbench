import "DPI-C" function int pmem_read(input int addr);
import "DPI-C" function void pmem_write(input int addr, input int data, input byte mask);

/*
MemOp:
000  1字节读/写，读时带符号扩展
001  2字节读/写，读时带符号扩展
010  4字节读/写
100  1字节读，无符号扩展
101  2字节读，无符号扩展
*/

module DataMem (
    input [31:0] addr,

    // read
    input RdClk,
    output reg [31:0] out,

    // write
    input WrClk,
    input MemWr,
    input [2:0] MemOp,
    input [31:0] in
);

reg [31:0] read;

always @(posedge RdClk) begin
    read = pmem_read(addr);
    case (MemOp)
        3'b000: // 1字节读，带符号扩展
            out = {{24{read[7]}}, read[7:0]};
        3'b001: // 2字节读，带符号扩展
            out = {{16{read[7]}}, read[15:0]};
        3'b010: // 4字节读
            out = read;
        3'b100: // 1字节读，无符号扩展
            out = {24'b0, read[7:0]};
        3'b101: // 2字节读，无符号扩展
            out = {16'b0, read[15:0]};
        default:;
    endcase
end

always @(posedge WrClk) begin
    if (MemWr) begin
        case (MemOp)
            3'b000: // 1字节写
                pmem_write(addr, in, 8'b00000001);
            3'b001: // 2字节写
                pmem_write(addr, in, 8'b00000011);
            3'b010: // 4字节写
                pmem_write(addr, in, 8'b00001111);
            default:;
        endcase
    end
end


endmodule