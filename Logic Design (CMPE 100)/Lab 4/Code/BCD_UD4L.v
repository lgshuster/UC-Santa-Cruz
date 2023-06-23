`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/01/2019 08:46:25 PM
// Design Name: 
// Module Name: BCD_UD4L
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


module BCD_UD4L(
    input clk,
    input Up,
    input Dw,
    input LD,
    input [3:0] Din,
    output [3:0] Q,
    output UTC,
    output DTC    
    );
    
    wire [3:0] in;

    wire isMax = Din[3] & (Din[2] | Din[1]);
    wire [3:0] max = ({4{isMax}} & 4'd9) | (~{4{isMax}} & Din);
    
    
    wire enable = Up | Dw | LD;

    assign in[3] = ((((Q[3]&~Q[0] | Q[2]&Q[1]&Q[0]) & Up)
                    | ((Q[3]&Q[0] | ~Q[3]&~Q[2]&~Q[1]&~Q[0]) & Dw)) & ~LD)
                    | (max[3] & LD);
                    
    assign in[2] = ((((Q[2]&~Q[1] | Q[2]&~Q[0] | ~Q[2]&Q[1]&Q[0]) & Up) 
                    | ((Q[2]&Q[0] | Q[2]&Q[1] | Q[3]&~Q[0]) & Dw)) & ~LD)
                    | (max[2] & LD);
    assign in[1] = ((((Q[1]&~Q[0] | ~Q[3]&~Q[1]&Q[0]) & Up)
                    | ((Q[1]&Q[0] | Q[3]&~Q[0] | Q[2]&~Q[1]&~Q[0]) & Dw)) & ~LD)
                    | (max[1] & LD);
    assign in[0] = ((((~Q[0]) & Up)
                    | (~Q[0] & Dw)) & ~LD)
                    | (max[0] & LD);

    
    FDRE #(.INIT(1'b0) ) ff1 (.C(clk), .R(1'b0), .CE(enable), .D(in[3]), .Q(Q[3]));
    FDRE #(.INIT(1'b0) ) ff2 (.C(clk), .R(1'b0), .CE(enable), .D(in[2]), .Q(Q[2]));
    FDRE #(.INIT(1'b0) ) ff3 (.C(clk), .R(1'b0), .CE(enable), .D(in[1]), .Q(Q[1]));
    FDRE #(.INIT(1'b0) ) ff4 (.C(clk), .R(1'b0), .CE(enable), .D(in[0]), .Q(Q[0]));
    
    
    assign UTC = Q[3]&~Q[2]&~Q[1]&Q[0];
    assign DTC = ~Q[3]&~Q[2]&~Q[1]&~Q[0];
    
endmodule
