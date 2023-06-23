`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/11/2020 05:41:49 PM
// Design Name: 
// Module Name: structural_LSFR
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

module lfsr_structural(
    input clk,
    input res_n,
    input [7:0] data_in,
    output [7:0] data_out
    );
    wire rnd [2:0];
    assign rnd[0] = data_out[1] ^ data_out[7]; // random bits that change the bits that are shifted around
    assign rnd[1] = data_out[2] ^ data_out[7];
    assign rnd[2] = data_out[3] ^ data_out[7];
    
    //FDRE's taken from CSE100 
    // The first flip flop isnt instantiated because data_in will instantiate it instead   
    //res_n is active low so when ~res_n is when you want to use data_in 
    FDRE #(.INIT(1'b0) ) ff_instance_101 (.C(clk), .CE(1'b1), .D((res_n & data_out[7]) | (~res_n & data_in[0])), .Q(data_out[0]));
    FDRE #(.INIT(1'b0) ) ff_instance_102 (.C(clk), .CE(1'b1), .D((res_n & data_out[0]) | (~res_n & data_in[1])), .Q(data_out[1]));
    FDRE #(.INIT(1'b0) ) ff_instance_103 (.C(clk), .CE(1'b1), .D((res_n & rnd[0]) | (~res_n & data_in[2])), .Q(data_out[2]));
    FDRE #(.INIT(1'b0) ) ff_instance_104 (.C(clk), .CE(1'b1), .D((res_n & rnd[1]) | (~res_n & data_in[3])), .Q(data_out[3]));  
    FDRE #(.INIT(1'b0) ) ff_instance_105 (.C(clk), .CE(1'b1), .D((res_n & rnd[2]) | (~res_n & data_in[4])), .Q(data_out[4]));
    FDRE #(.INIT(1'b0) ) ff_instance_106 (.C(clk), .CE(1'b1), .D((res_n & data_out[4]) | (~res_n & data_in[5])), .Q(data_out[5]));
    FDRE #(.INIT(1'b0) ) ff_instance_107 (.C(clk), .CE(1'b1), .D((res_n & data_out[5]) | (~res_n & data_in[6])), .Q(data_out[6]));
    FDRE #(.INIT(1'b0) ) ff_instance_108 (.C(clk), .CE(1'b1), .D((res_n & data_out[6]) | (~res_n & data_in[7])), .Q(data_out[7]));     
endmodule