module top(
        input clk,
        input rst
    );

    // PC

    wire [31:0] NextPC, pc /* verilator public */;
    wire PCAsrc, PCBsrc;
    BranchCond bc (
        .Branch ( Branch  ),
        .Less   ( Less    ),
        .Zero   ( Zero    ),
        .PCAsrc ( PCAsrc  ),
        .PCBsrc ( PCBsrc  )
    );
    Reg #(32, 32'h80000000) pc_r(
            .clk  ( clk    ),
            .rst  ( rst    ),
            .din  ( NextPC ),
            .dout ( pc     ),
            .wen  ( 0   )
        );
    assign NextPC = (PCAsrc == 0 ? 4 : imm) + (PCBsrc == 0 ? pc : rbus1);

    // GPR

    wire [31:0] rbus1, rbus2;

    GPR gpr(
            .rst   ( rst               ),
            .WrClk ( clk               ),
            .RegWr ( RegWr             ),
            .Rw    ( rd                ),
            .busW  ( MemToReg ? MemOut
                              : ALUout ),
            .Ra    ( rs1               ),
            .busA  ( rbus1             ),
            .Rb    ( rs2               ),
            .busB  ( rbus2             )
        );

    // Instruction Memory

    wire [31:0] inst /* verilator public */;

    InstMem inst_mem(
                .rst  ( rst   ),
                .pc   ( pc    ),
                .inst ( inst  )
             );

    // Instruction Decode

    wire [2:0]      ExtOP;
    wire [6:0]  	op;
    wire [2:0]  	func3;
    wire [6:0]  	func7;
    wire [2:0]  	itype;
    wire [4:0]  	rd;
    wire [4:0]  	rs1;
    wire [4:0]  	rs2;
    wire [31:0] 	imm;

    IDU idu(
            .inst 	( inst   ),
            .ExtOP  ( ExtOP  ),
            .op    	( op     ),
            .func3 	( func3  ),
            .func7 	( func7  ),
            .rd    	( rd     ),
            .rs1   	( rs1    ),
            .rs2   	( rs2    ),
            .imm   	( imm    )
        );


    // Control Signal Generate

    wire        RegWr;
    wire        ALUAsrc;
    wire [1:0]  ALUBsrc;
    wire [3:0]  ALUctr;
    wire [2:0]  Branch;
    wire        MemToReg;
    wire        MemRd;
    wire        MemWr;
    wire [2:0]  MemOp;

    CSG csg(
            .op         ( op       ),
            .func3      ( func3    ),
            .func7      ( func7    ),
            .ExtOP      ( ExtOP    ),
            .RegWr      ( RegWr    ),
            .ALUAsrc    ( ALUAsrc  ),
            .ALUBsrc    ( ALUBsrc  ),
            .ALUctr     ( ALUctr   ),
            .Branch     ( Branch   ),
            .MemToReg   ( MemToReg ),
            .MemRd      ( MemRd    ),
            .MemWr      ( MemWr    ),
            .MemOp      ( MemOp    )
        );


    // ALU

    wire [31:0] ALUout;
    wire Less, Zero;

    ALU alu(
            .A      ( ALUAsrc ? pc : rbus1 ),
            .B      ( ALUBsrc == 2'b00 ? rbus2 :
                      ALUBsrc == 2'b01 ? imm :
                      ALUBsrc == 2'b10 ? 4 :
                      0
                    ),
            .ctr    ( ALUctr  ),
            .out    ( ALUout  ),
            .Less   ( Less    ),
            .Zero   ( Zero    )
        );

    // Data Memory

    wire [31:0] MemOut;
    DataMem data_mem(
        .addr  ( ALUout  ),
        .RdClk ( clk     ),
        .out   ( MemOut  ),
        .WrClk ( clk     ),
        .MemRd ( MemRd   ),
        .MemWr ( MemWr   ),
        .MemOp ( MemOp   ),
        .in    ( rbus2   )
    );

endmodule
