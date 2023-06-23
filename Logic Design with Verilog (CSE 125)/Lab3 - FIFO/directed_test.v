`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/01/2020 09:51:24 PM
// Design Name: 
// Module Name: fifo_tb
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


module directed_test;
    parameter DEPTH = 4;
    parameter WIDTH = 32;
    
    localparam TEST_CASES = DEPTH;
    integer i, passed = 1'b1;
    
    // Inputs
    reg clk;
    reg res_n;
    reg shift_in;
    reg shift_out;
    reg [WIDTH-1:0] data_in;
    reg [WIDTH-1:0] test_data_in [0:TEST_CASES-1];
    
    // Outputs
    wire full;
    wire empty;
    wire [WIDTH-1:0] data_out;
    
    // Instantiate the Unit Unter Test (UUT)
    fifo #(.WIDTH(WIDTH), .DEPTH(DEPTH)) uut (
        .clk(clk),
        .res_n(res_n),
        .shift_in(shift_in),
        .shift_out(shift_out),
        .data_in(data_in),
        .full(full),
        .empty(empty),
        .data_out(data_out)
    );
    
    always 
        #5 clk = ~clk;
    
    initial begin
        // Initialize inputs
        clk = 0;
        res_n = 0;
        shift_in = 0;
        shift_out = 0;
        data_in = 0;
        
        @(posedge clk); @(posedge clk); #1 res_n = 1'b1;
        
        // Shift in values
        shift_in = 1'b1;
        for (i =0; i < TEST_CASES; i = i+1) begin
            data_in = i;
            test_data_in[i] = i;
            @(posedge clk); #1;
        end
        shift_in = 1'b0;
        
        // Shift out values
        shift_out = 1'b1;
        for (i =0; i < TEST_CASES; i = i+1) begin
            if(data_out != test_data_in[i]) begin
                passed = 0;
            end
            @(posedge clk); #1;
        end
        shift_out = 1'b0;
        
        if(!passed) $display("The shifted out FIFO data did not match the shifted in data. Failed Test 1");
        else $display("The shifted out FIFO data matched the shifted in data. Passed Test 1");
            
        // Run another set of tests with shifting in and out simultaneously
        passed = 1'b1;
        
        // Shift in 1 value
        @(posedge clk); #1;
        shift_in = 1'b1;
        data_in = test_data_in[0];
        @(posedge clk); #1;
        
        // Shift values in and out
        shift_out = 1'b1;
        for (i =0; i < TEST_CASES; i = i+1) begin
            data_in = test_data_in[i];
            if(data_out != test_data_in[i-1]) begin
                passed = 0;
            end
            @(posedge clk); #1;
        end
        shift_in = 1'b0;
        if(data_out != test_data_in[TEST_CASES-1]) begin
            passed = 0;
        end
        @(posedge clk); #1; shift_out = 1'b0;
        
        if(!passed) $display("The shifted out FIFO data did not match the shifted in data. Failed Test 2");
        else $display("The shifted out FIFO data matched the shifted in data. Passed Test 2");
    end
endmodule