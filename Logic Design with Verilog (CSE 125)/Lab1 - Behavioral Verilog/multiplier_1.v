`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/12/2020 10:18:47 PM
// Design Name: 
// Module Name: multiplier_1
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

module multiplier_1(
    input wire clk, 
    input wire res_n, 
    input wire start, 
    output reg done,
    input wire [15:0] arg1, 
    input wire [15:0] arg2, 
    output reg [31:0] product
    );
    
    integer go = 0; // Stores value when start signal goes high so multiplier can start and keep counting
    integer inc = 0; // Keeps track of how many additions have been done
            
    always @(posedge clk) begin
        if (!res_n) begin // If reset, initialize everything to 0 for multiplier
            product <= 32'b0;
            done <= 0;
            go <= 0;
        end
        else if (start) go <= 1; // If start goes high, store its value
        
        if (go) begin // If time to start
            if (inc < arg2) begin // If number of additions does not equal second number, keep adding
                product <= product + arg1;
                inc <= inc + 1;
            end
            else done <= 1; // Else if 0 remaining additions, assert done signal
        end
    end


endmodule  
