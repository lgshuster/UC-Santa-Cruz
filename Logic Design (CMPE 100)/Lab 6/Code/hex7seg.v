`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/18/2019 01:07:11 PM
// Design Name: 
// Module Name: hex7seg
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


module hex7seg(
    input [3:0] n,
    input e,
    output [6:0] seg
    );
    
    wire [7:0] CGmux_in, CFmux_in, CEmux_in, CDmux_in, CCmux_in, CBmux_in, CAmux_in;
    wire [2:0] sel;
    
    assign CGmux_in = {1'b1, 1'b0, 1'b0, n[0], 1'b0, 1'b0, ~n[0], 1'b0};
    assign CFmux_in = {n[0], 1'b1, 1'b0, n[0], 1'b0, 1'b0, n[0], 1'b0};
    assign CEmux_in = {n[0], n[0], 1'b1, n[0], n[0], 1'b0, 1'b0, 1'b0};
    assign CDmux_in = {n[0], 1'b0, ~n[0], n[0], 1'b0, ~n[0], 1'b0, n[0]};
    assign CCmux_in = {1'b0, ~n[0], 1'b0, 1'b0, 1'b0, 1'b0, ~n[0], 1'b1};
    assign CBmux_in = {1'b0, 1'b0, n[0], ~n[0], 1'b0, n[0], ~n[0], 1'b1};
    assign CAmux_in = {n[0], 1'b0, ~n[0], 1'b0, 1'b0, n[0], n[0], 1'b0};

    assign sel = {n[3], n[2], n[1]};
    
    m8_1e CGmux (.in(CGmux_in), .sel(sel), .e(e), .o(seg[6]));
    m8_1e CFmux (.in(CFmux_in), .sel(sel), .e(e), .o(seg[5]));
    m8_1e CEmux (.in(CEmux_in), .sel(sel), .e(e), .o(seg[4]));
    m8_1e CDmux (.in(CDmux_in), .sel(sel), .e(e), .o(seg[3]));
    m8_1e CCmux (.in(CCmux_in), .sel(sel), .e(e), .o(seg[2]));
    m8_1e CBmux (.in(CBmux_in), .sel(sel), .e(e), .o(seg[1]));
    m8_1e CAmux (.in(CAmux_in), .sel(sel), .e(e), .o(seg[0]));     
    
    
endmodule
