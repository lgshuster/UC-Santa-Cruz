`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/12/2020 04:30:20 PM
// Design Name: 
// Module Name: lfsr_behavioural
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

module lfsr_behavioural(
    input wire clk, 
    input wire res_n, 
    input wire [7:0] data_in,
    output reg [7:0] data_out 
    );
            
    always @(posedge clk) begin
        if (!res_n) begin // If reset, load in data_in into lfsr
            data_out <= data_in;
        end
        else begin // If no reset, do normal lfsr with synchronous non-blocking assignments
          data_out[7] <= data_out[6];
          data_out[6] <= data_out[5]; 
          data_out[5] <= data_out[4]; 
          data_out[4] <= data_out[3] ^ data_out[7]; 
          data_out[3] <= data_out[2] ^ data_out[7]; 
          data_out[2] <= data_out[1] ^ data_out[7]; 
          data_out[1] <= data_out[0]; 
          data_out[0] <= data_out[7];  
        end
    end
endmodule