`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/18/2019 02:38:15 PM
// Design Name: 
// Module Name: top_mod
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


module top_mod(
    input [15:0] sw,
    input btnU,
    input btnR,
    input clkin,
    output [6:0] seg,
    output dp,
    output [3:0] an
    
    //output [7:0] first_num, second_num, tmp_sum,
    //output [6:0] tmp_seg1, tmp_seg2
    );
    
    wire [7:0] sum;
    wire [6:0] seg1, seg2;
    wire dig_sel, dp2;
    
    lab3_digsel select1 (.clkin(clkin), .greset(btnR), .digsel(dig_sel));
    
    AddSub8 adder_subtractor (.A(sw[15:8]), .B(sw[7:0]), .sub(btnU), .S(sum), .ovfl(dp2));
    hex7seg right_display (.n(sum[3:0]), .e(~dig_sel), .seg(seg1));
    hex7seg left_display (.n(sum[7:4]), .e(dig_sel), .seg(seg2));
    m2_1x8 seg_mux (.in0(seg1), .in1(seg2), .s(dig_sel), .o(seg));
    
    assign an[0] = dig_sel;
    assign an[1] = ~dig_sel;
    assign an[2]= 1'b1;
    assign an[3]= 1'b1;
    assign dp = ~dp2;
    
    //assign tmp_sum = sum;
    //assign first_num = sw[15:8];
    //assign second_num = sw[7:0];
    //assign tmp_seg1 = seg1;
    //assign tmp_seg2 = seg2;
    
    
    
endmodule
