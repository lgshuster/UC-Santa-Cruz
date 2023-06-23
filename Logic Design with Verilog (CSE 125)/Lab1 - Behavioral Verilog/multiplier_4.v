`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/14/2020 02:06:26 AM
// Design Name: 
// Module Name: multiplier_4
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

module multiplier_4(
    input wire clk, 
    input wire res_n, 
    input wire start, 
    output reg done,
    input wire [15:0] arg1, 
    input wire [15:0] arg2, 
    output reg [31:0] product
    );
    
    integer go = 0; // Stores value when start signal goes high so multiplier can start and keep counting
    integer i; // Incrementer for for loops
    integer inc = 0; // Keeps track of how many additions have been done
            
    always @(posedge clk) begin
        if (!res_n) begin // If reset, initialize everything to 0 for multiplier
            product <= 32'b0;
            done <= 0;
            go <= 0;
        end
        else if (start) go <= 1; // If start goes high, store its value
        
        if (go) begin // If time to start
            if (arg2 - inc > 3) begin // If 4 or more remaining additions, add up to 4 times
                product <= product + arg1 + arg1 + arg1 + arg1;
                inc <= inc + 4;
            end 
            else if (arg2 - inc == 3) begin // Else if 3 remaining additions, add 3 times
                product <= product + arg1 + arg1 + arg1;
                inc <= inc + 3;
            end
            else if (arg2 - inc == 2) begin // Else if 2 remaining additions, add 2 times
                product <= product + arg1 + arg1;
                inc <= inc + 2;
            end
            else if (arg2 - inc == 1) begin // Else if 1 remaining addition, add 1 time
                product <= product + arg1;
                inc <= inc + 1;
            end
            else done <= 1; // Else if 0 remaining additions, assert done signal 
        end
    end


endmodule  