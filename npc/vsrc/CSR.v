module CSR (
        input rst,

        // write port
        input WrClk,
        input Wr,
        input [11:0] Rw,
        input [31:0] busW,

        // read port
        input [11:0] Rr,
        output reg [31:0] busR
    );

    reg [31:0] mstatus;
    reg [31:0] mtvec;
    reg [31:0] mepc;
    reg [31:0] mcause;

    always @(*) begin
        case (Rr)
        12'h300: busR = mstatus;
        12'h305: busR = mtvec;
        12'h341: busR = mepc;
        12'h342: busR = mcause;
        default: busR = 0;
        endcase
    end

    always @(posedge WrClk) begin
        if (rst) begin
            mstatus = 32'h1800; // MPP 设为 M 模式
            mtvec   = 0;
            mepc    = 0;
            mcause  = 0;
        end
        else if (Wr) begin
            case (Rr)
            12'h300: mstatus = busW;
            12'h305: mtvec   = busW;
            12'h341: mepc    = busW;
            12'h342: mcause  = busW;
            default: ;
            endcase
        end
    end

endmodule
