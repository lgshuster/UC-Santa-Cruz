`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/08/2019 03:24:22 AM
// Design Name: 
// Module Name: Numbers
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module LED_Shifter(
    input clk,
    input SHR,
    input SHL,
    output [15:0] Q
    );
    
    wire Advance = SHR | SHL;
    
    FDRE  #(.INIT(1'b0) ) ff16 (.C(clk), .R(1'b0), .CE(Advance),  .D((Q[14] & SHL)), .Q(Q[15]));
    FDRE  #(.INIT(1'b0) ) ff15 (.C(clk), .R(1'b0), .CE(Advance),  .D((Q[13] & SHL) | (Q[15] & SHR)), .Q(Q[14]));
    FDRE  #(.INIT(1'b0) ) ff14 (.C(clk), .R(1'b0), .CE(Advance),  .D((Q[12] & SHL) | (Q[14] & SHR)), .Q(Q[13]));
    FDRE  #(.INIT(1'b0) ) ff13 (.C(clk), .R(1'b0), .CE(Advance),  .D((Q[11] & SHL) | (Q[13] & SHR)), .Q(Q[12]));
    FDRE  #(.INIT(1'b0) ) ff12 (.C(clk), .R(1'b0), .CE(Advance),  .D((Q[10] & SHL) | (Q[12] & SHR)), .Q(Q[11]));
    FDRE  #(.INIT(1'b0) ) ff11 (.C(clk), .R(1'b0), .CE(Advance),  .D((Q[9] & SHL) | (Q[11] & SHR)), .Q(Q[10]));
    FDRE  #(.INIT(1'b0) ) ff10 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[8] & SHL) | (Q[10] & SHR)), .Q(Q[9]));
    FDRE  #(.INIT(1'b0) ) ff9 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[7] & SHL) | (Q[9] & SHR)), .Q(Q[8]));
    FDRE  #(.INIT(1'b0) ) ff8 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[6] & SHL) | (Q[8] & SHR)), .Q(Q[7]));
    FDRE  #(.INIT(1'b0) ) ff7 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[5] & SHL) | (Q[7] & SHR)), .Q(Q[6]));
    FDRE  #(.INIT(1'b0) ) ff6 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[4] & SHL) | (Q[6] & SHR)), .Q(Q[5]));
    FDRE  #(.INIT(1'b0) ) ff5 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[3] & SHL) | (Q[5] & SHR)), .Q(Q[4]));
    FDRE  #(.INIT(1'b0) ) ff4 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[2] & SHL) | (Q[4] & SHR)), .Q(Q[3]));
    FDRE  #(.INIT(1'b0) ) ff3 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[1] & SHL) | (Q[3] & SHR)), .Q(Q[2]));
    FDRE  #(.INIT(1'b0) ) ff2 (.C(clk), .R(1'b0), .CE(Advance), .D((Q[0] & SHL) | (Q[2] & SHR)), .Q(Q[1]));
    FDRE  #(.INIT(1'b0) ) ff1 (.C(clk), .R(1'b0), .CE(Advance), .D(SHL | (Q[1] & SHR)), .Q(Q[0]));

endmodule
