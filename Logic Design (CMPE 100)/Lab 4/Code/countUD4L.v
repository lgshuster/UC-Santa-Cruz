`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/30/2019 02:30:44 AM
// Design Name: 
// Module Name: countUD4L
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


module countUD4L(
    input clk,
    input reset,
    input Up,
    input Dw,
    input LD,
    input [3:0] Din,
    output [3:0] Q,
    output UTC,
    output DTC    
    );
    
    wire [3:0] in;
    wire enable = Up | Dw | LD;
    
    assign in[3] = ((((Q[3]&~Q[2] | Q[3]&~Q[1] | Q[3]&~Q[0] | ~Q[3]&Q[2]&Q[1]&Q[0]) & Up)
                    | ((Q[3]&Q[2] | Q[3]&Q[0] | Q[3]&Q[1] | ~Q[3]&~Q[2]&~Q[1]&~Q[0]) & Dw)) & ~LD)
                    | (Din[3] & LD);
                    
    assign in[2] = ((((Q[2]&~Q[1] | Q[2]&~Q[0] | ~Q[2]&Q[1]&Q[0]) & Up) 
                    | ((Q[2]&Q[0] | Q[2]&Q[1] | ~Q[2]&~Q[1]&~Q[0]) & Dw)) & ~LD)
                    | (Din[2] & LD);
    assign in[1] = ((((Q[1]&~Q[0] | ~Q[1]&Q[0]) & Up)
                    | ((Q[1]&Q[0] | ~Q[1]&~Q[0]) & Dw)) & ~LD)
                    | (Din[1] & LD);
    assign in[0] = ((((~Q[0]) & Up)
                    | (~Q[0] & Dw)) & ~LD)
                    | (Din[0] & LD);
                    

    FDRE #(.INIT(1'b0) ) ff1 (.C(clk), .R(reset), .CE(enable), .D(in[3]), .Q(Q[3]));
    FDRE #(.INIT(1'b0) ) ff2 (.C(clk), .R(reset), .CE(enable), .D(in[2]), .Q(Q[2]));
    FDRE #(.INIT(1'b0) ) ff3 (.C(clk), .R(reset), .CE(enable), .D(in[1]), .Q(Q[1]));
    FDRE #(.INIT(1'b0) ) ff4 (.C(clk), .R(reset), .CE(enable), .D(in[0]), .Q(Q[0]));
    
    
    assign UTC = Q[3]&Q[2]&Q[1]&Q[0];
    assign DTC = ~Q[3]&~Q[2]&~Q[1]&~Q[0];
endmodule
