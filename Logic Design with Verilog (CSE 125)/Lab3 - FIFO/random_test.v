`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/02/2020 09:51:09 PM
// Design Name: 
// Module Name: random
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


module random_test;
    integer seed = 9;
    parameter WIDTH = 32;
    parameter DEPTH = 1000;
    parameter TEST_NUMBER = 1000;
    
    // Inputs
    reg clk;
    reg res_n;
    reg shift_in;
    reg shift_out;
    reg [WIDTH-1:0] data_in;
    
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
    
    integer i, o, in_pointer, out_pointer, rand;
    reg [WIDTH-1:0] data_in_arr [0:TEST_NUMBER-1]; // 2D array for data_in
    reg [WIDTH-1:0] data_out_arr [0:TEST_NUMBER-1]; // 2D array for data_out
    
    initial begin
        i = $fopen("random_input_data.txt", "w"); // Open file for writing data_in
        
        // Generate random data for data_in and write to file
        for (in_pointer = 0; in_pointer < TEST_NUMBER; in_pointer = in_pointer + 1) begin
            rand = $random(seed); // Generate random data based on seed
            $fwrite(i, "%b\n", rand); // Write random data_in to file
            //$display("Test %d: %d", in_pointer, rand);
        end
        $fclose(i); // Close file
        
        $readmemb("random_input_data.txt", data_in_arr); // Read input data
    
        // Initialize Inputs
        clk = 0;
        res_n = 0;
        shift_in = 0;
        shift_out = 0;
        data_in = 0;
        
        // Wait 100 ns for global reset to finish
        #105;
        
        res_n = 1;
        in_pointer = 0;
        out_pointer = 0;
        
        o = $fopen("random_output_data.txt", "w"); // Open file for writing data_out
        while (in_pointer < TEST_NUMBER) begin // Loop until in_pointer reaches end of data_in array
            @(posedge clk); #1;
                    
            data_in = data_in_arr[in_pointer][WIDTH-1:0]; // Get new data_in from data_in array
            
            // Generate random shift_in and shift_out
            shift_in = $random(seed) % 2;
            shift_out = $random(seed) % 2;
                        
            $display("shift_in: %0d shift out: %0d", shift_in, shift_out);
            
            if (shift_in && ~full) begin // If shift_in and not full, increment data_in array pointer
                in_pointer = in_pointer + 1;
            end
            if (shift_out && ~empty) begin // If shift_out and not empty, write data_out to file and increment data_out array pointer
                data_out_arr[out_pointer][WIDTH-1:0] = data_out;
                
                $fwrite(o, "%b\n", data_out_arr[out_pointer][WIDTH-1:0]); // Write data_out to file
                //$display("%b\n", data_out_arr[out_pointer][WIDTH-1:0]);
                                
                out_pointer = out_pointer + 1;
            end
            
            if ((full && shift_in) || (empty && shift_out)) begin // If illegal shift_in and shift_out, force in_pointer to end of data_in array
               in_pointer = TEST_NUMBER;
            end
        end
        
        while (out_pointer < TEST_NUMBER) begin // Add rest of data_in array to data_out array in case of illegal shift_in or shift_out
            $fwrite(o, "%b\n", data_in_arr[out_pointer][WIDTH-1:0]); // Write data_out to file
            //$display("%b\n", data_in_arr[out_pointer][WIDTH-1:0]);
            
            out_pointer = out_pointer + 1;
        end
        
        $fclose(o); // Close file
        $finish;
    end
endmodule