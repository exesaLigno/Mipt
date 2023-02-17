module VCB2CE ( input ce, output reg Q0 = 0,
                input clk, output reg Q1 = 0,
                input clr, output wire TC,
                output wire CEO);
    assign TC = (Q1 & Q0) ; // Q0 & Q1=1
    assign CEO = ce & TC ; // Сигнал переноса
    always @ (posedge clk or posedge clr) 
    begin
        Q0 <= clr? 0 : ce? !Q0 : Q0 ; /* Если clr=1, то сброс в 0, иначе если ce=1, то "переключаться", иначе "стоять"*/
        Q1 <= clr? 0 : (ce & Q0)? !Q1 : Q1 ; /* Если clr=1, то сброс в 0, иначе если (ce & Q0)=1, то "переключаться", иначе "стоять"*/
    end
endmodule
