`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/03/2019 02:29:54 AM
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
    input clkin,
    input btnR,
    input btnU,
    input btnD,
    input btnC,
    input btnL,
    input [15:0] sw,
    output [6:0] seg,
    output dp,
    output [3:0] an,
    output [15:0] led
//    output UTC,
//    output DTC
    );
    
    
    wire clk, digsel;
    wire btnUEdge, btnDEdge, c_count, Up, Dw, UTC, DTC;
    wire [15:0] Qcounter;
    wire [3:0] QringCounter, Qselector;
    
    lab4_clks slowit (.clkin(clkin), .greset(btnR), .clk(clk), .digsel(digsel));
    
    
    
    assign c_count = btnC & ~(Qcounter[15]&~Qcounter[14]&~Qcounter[13]&Qcounter[12]&Qcounter[11]&~Qcounter[10]&~Qcounter[9]&(&Qcounter[8:2]));
    assign Up = btnUEdge | c_count; 
    assign Dw = btnDEdge;
    assign dp = ~((~an[3] & UTC) | (~an[0] & DTC));
    assign an = ~QringCounter;
    assign led = sw;




    edge_detector UpEdgeDetector (.clk(clk), .btn(btnU), .clockEdge(btnUEdge));
    edge_detector DownEdgeDetector (.clk(clk), .btn(btnD), .clockEdge(btnDEdge));    
    BCDbinUD16L counter (.clk(clk), .Up(Up), .Dw(Dw), .LD(btnL), .Din(sw), .Q(Qcounter), .UTC(UTC), .DTC(DTC));
    ring_counter RingCounter (.clk(clk), .Advance(digsel), .Q(QringCounter));
    selector Selector (.sel(QringCounter), .N(Qcounter), .H(Qselector));
    hex7seg display (.n(Qselector), .e(1'b1), .seg(seg));
    
    
    
    
    
    
endmodule
