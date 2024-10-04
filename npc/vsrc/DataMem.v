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

wire [31:0] aligned_addr = addr & ~32'b11; // 地址对齐到 4 字节
wire [1:0] align_offset = addr[1:0];       // 地址对齐到 4 字节的偏移量

// read

reg [31:0] read;
wire [7:0] r_byte1;  // 如果是读一个字节，这就是要读的那部分，由 mux_mem_read_byte1 给出
wire [15:0] r_byte2; // 如果是读两个字节，这就是要读的那部分，由 mux_mem_read_byte2 给出
MuxKey #(5, 3, 32) mux_mem_read(
    .key(MemOp),
    .out(out),
    .lut({
        3'b000, {{24{r_byte1[7]}}, r_byte1},  // 1字节读，带符号扩展
        3'b001, {{16{r_byte2[15]}}, r_byte2}, // 2字节读，带符号扩展
        3'b010, read,                         // 4字节读
        3'b100, {24'b0, r_byte1},             // 1字节读，无符号扩展
        3'b101, {16'b0, r_byte2}              // 2字节读，无符号扩展
    })
);
MuxKey #(4, 2, 8) mux_mem_read_byte1(
    .key(align_offset),
    .out(r_byte1),
    .lut({
        2'b00, read[31:24],
        2'b01, read[23:16],
        2'b10, read[15:8],
        2'b11, read[7:0]
    })
);
MuxKey #(3, 2, 16) mux_mem_read_byte2(
    .key(align_offset),
    .out(r_byte2),
    .lut({
        2'b00, read[31:16],
        2'b01, read[23:8],
        2'b10, read[15:0]
    })
);

// write

wire [31:0] write;
wire [7:0]  write_mask;
wire [31:0] write_byte1; // 如果是写一个字节，这就是要写的内容的扩展，由 mux_mem_write_byte1 给出
wire [31:0] write_byte2; // 如果是写两个字节，这就是要写的内容的扩展，由 mux_mem_write_byte2 给出
wire [7:0] write_mask1;  // 如果是写一个字节，这就是要写的内容的 mask，由 mux_mem_write_byte1 给出
wire [7:0] write_mask2;  // 如果是写两个字节，这就是要写的内容的 mask，由 mux_mem_write_byte2 给出
MuxKey #(3, 3, 40) mux_mem_write(
    .key(MemOp),
    .out({write, write_mask}),
    .lut({
        3'b000, {write_byte1, write_mask1}, // 1字节写
        3'b001, {write_byte2, write_mask2}, // 2字节写
        3'b010, {in, 8'b1111}             // 4字节写
    })
);
MuxKey #(4, 2, 40) mux_mem_write_byte1(
    .key(align_offset),
    .out({write_byte1, write_mask1}),
    .lut({
        2'b00, {{in[7:0], 24'b0}, 8'b1000},
        2'b01, {{8'b0, in[7:0], 16'b0}, 8'b0100},
        2'b10, {{16'b0, in[7:0], 8'b0}, 8'b0010},
        2'b11, {{24'b0, in[7:0]}, 8'b0001}
    })
);
MuxKey #(3, 2, 40) mux_mem_write_byte2(
    .key(align_offset),
    .out({write_byte2, write_mask2}),
    .lut({
        2'b00, {{in[15:0], 16'b0}, 8'b1100},
        2'b01, {{8'b0, in[15:0], 8'b0}, 8'b0110},
        2'b10, {{16'b0, in[15:0]}, 8'b0011}
    })
);


always @(posedge RdClk) begin
    read = pmem_read(aligned_addr);
    $display("read: ", read);
end

always @(posedge WrClk) begin
    if (MemWr) begin
        pmem_write(aligned_addr, write, write_mask);
    end
end

endmodule
