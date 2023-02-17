module Gen4an ( input clk, output reg [1:0] q = 0, //Счетчик номера анода
                input ce, output wire [3:0] an );
    assign an = (q==0)? 4'b1110 : // включение цифры 0 (младшей)
    (q==1)? 4'b1101 : // включение цифры 1
    (q==2)? 4'b1011 : // включение цифры 2
    4'b0111 ; // включение цифры 3 (старшей)
    always @ (posedge clk) if (ce) 
    begin
        q <= q+1 ;
    end
endmodule
