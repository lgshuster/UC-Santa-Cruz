`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/08/2019 01:49:11 AM
// Design Name: 
// Module Name: LFSR
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


module LFSR(
    input clk,
    output [15:0] rnd
    );
        
    FDRE #(.INIT(1'b1) ) ff1 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[0] ^ rnd[5] ^ rnd[6] ^ rnd[7]), .Q(rnd[0]));
    FDRE #(.INIT(1'b0) ) ff2 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[0]), .Q(rnd[1]));
    FDRE #(.INIT(1'b0) ) ff3 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[1]), .Q(rnd[2]));
    FDRE #(.INIT(1'b0) ) ff4 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[2]), .Q(rnd[3]));
    FDRE #(.INIT(1'b0) ) ff5 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[3]), .Q(rnd[4]));
    FDRE #(.INIT(1'b0) ) ff6 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[4]), .Q(rnd[5]));
    FDRE #(.INIT(1'b0) ) ff7 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[5]), .Q(rnd[6]));
    FDRE #(.INIT(1'b0) ) ff8 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[6]), .Q(rnd[7]));
    FDRE #(.INIT(1'b0) ) ff9 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[7]), .Q(rnd[8]));
    FDRE #(.INIT(1'b0) ) ff10 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[8]), .Q(rnd[9]));
    FDRE #(.INIT(1'b0) ) ff11 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[9]), .Q(rnd[10]));
    FDRE #(.INIT(1'b0) ) ff12 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[10]), .Q(rnd[11]));
    FDRE #(.INIT(1'b0) ) ff13 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[11]), .Q(rnd[12]));
    FDRE #(.INIT(1'b0) ) ff14 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[12]), .Q(rnd[13]));
    FDRE #(.INIT(1'b0) ) ff15 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[13]), .Q(rnd[14]));
    FDRE #(.INIT(1'b0) ) ff16 (.C(clk), .R(1'b0), .CE(1'b1), .D(rnd[14]), .Q(rnd[15]));
    
endmodule
