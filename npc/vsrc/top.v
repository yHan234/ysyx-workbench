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
            .wen  ( 1'b1   )
        );
    assign NextPC = (PCAsrc == 0 ? 4 : imm) + (PCBsrc == 0 ? pc : reg_src1);

    // GPR

    wire [31:0] reg_src1, reg_src2;

    GPR gpr(
            .rst   ( rst                        ),
            .WrClk ( clk                        ),
            .RegWr ( RegWr                      ),
            .Rw    ( rd                         ),
            .busW  ( MemToReg ? 32'b0 : ALUout  ),
            .Ra    ( rs1                        ),
            .busA  ( reg_src1                   ),
            .Rb    ( rs2                        ),
            .busB  ( reg_src2                   )
        );

    // Instruction Memory

    wire [31:0] inst /* verilator public */;

    InstrMem instr_mem(
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
            .MemWr      ( MemWr    ),
            .MemOp      ( MemOp    )
        );


    // ALU

    wire [31:0] ALUout;
    wire Less, Zero;

    ALU alu(
            .A      ( ALUAsrc ? pc : reg_src1 ),
            .B      ( ALUBsrc == 2'b00 ? reg_src2 :
                      ALUBsrc == 2'b01 ? imm :
                      ALUBsrc == 2'b10 ? 4 :
                      0
                    ),
            .ctr    ( ALUctr  ),
            .out    ( ALUout  ),
            .Less   ( Less    ),
            .Zero   ( Zero    )
        );

endmodule
