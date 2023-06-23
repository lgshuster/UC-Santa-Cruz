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
    input qsec,
    input LS,
    input RS,
    input SHR,
    input SHL,
    output [7:0] Q
    );
    
    wire Reset;
    wire Advance = qsec & (SHR | SHL);
    
    assign Reset = (qsec & &Q) | (~LS | ~RS);
        
    FDRE  #(.INIT(1'b0) ) ff8 (.C(clk), .R(Reset), .CE(Advance), .D((Q[6] & SHL) | SHR), .Q(Q[7]));
    FDRE  #(.INIT(1'b0) ) ff7 (.C(clk), .R(Reset), .CE(Advance), .D((Q[5] & SHL) | (Q[7] & SHR)), .Q(Q[6]));
    FDRE  #(.INIT(1'b0) ) ff6 (.C(clk), .R(Reset), .CE(Advance), .D((Q[4] & SHL) | (Q[6] & SHR)), .Q(Q[5]));
    FDRE  #(.INIT(1'b0) ) ff5 (.C(clk), .R(Reset), .CE(Advance), .D((Q[3] & SHL) | (Q[5] & SHR)), .Q(Q[4]));
    FDRE  #(.INIT(1'b0) ) ff4 (.C(clk), .R(Reset), .CE(Advance), .D((Q[2] & SHL) | (Q[4] & SHR)), .Q(Q[3]));
    FDRE  #(.INIT(1'b0) ) ff3 (.C(clk), .R(Reset), .CE(Advance), .D((Q[1] & SHL) | (Q[3] & SHR)), .Q(Q[2]));
    FDRE  #(.INIT(1'b0) ) ff2 (.C(clk), .R(Reset), .CE(Advance), .D((Q[0] & SHL) | (Q[2] & SHR)), .Q(Q[1]));
    FDRE  #(.INIT(1'b0) ) ff1 (.C(clk), .R(Reset), .CE(Advance), .D(SHL | (Q[1] & SHR)), .Q(Q[0])); 
    
endmodule
