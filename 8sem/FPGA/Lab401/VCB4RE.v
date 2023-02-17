module VCB4RE ( input ce,   output reg Q0 = 0,
                input clk,  output reg Q1 = 0,
                input r,    output reg Q2 = 0,
                            output reg Q3 = 0,
                            output wire TC,
                            output wire CEO);
    assign TC = (Q3 & Q2 & Q1 & Q0) ;
    assign CEO = ce & TC ;
    always @ (posedge clk) 
    begin
        Q0 <= r? 0 : ce? !Q0 : Q0 ; /* Если r=1, то сброс в 0, иначе если ce=1, то "переключаться", иначе "стоять"*/
        Q1 <= r? 0 : (ce & Q0)? !Q1 : Q1 ; /* Если r=1, то сброс в 0, иначе если (ce & Q0)=1, то "переключаться", иначе "стоять"*/
        Q2 <= r? 0 : (ce & Q0 & Q1)? !Q2 : Q2; 
        Q3 <= r? 0 : (ce & Q0 & Q1 & Q2)? !Q3 : Q3;
    end
endmodule
