// Вычитающий счетчик
`define m 4
module VCBDmSE (input ce, output reg [`m-1:0] Q = (1<<`m)-1,
                input clk, output wire TC,
                input s, output wire CEO);
    assign TC = (Q==0) ; // Q0,Q1,...Q'm-1 ==0
    assign CEO = ce & TC ; // Сигнал переноса
    always @ (posedge clk) 
    begin
        Q <= s? ((1<<`m)-1) : ce? Q-1 : Q ; /* Если s=1, то запись 2m-1, иначе если ce=1, то "вычитать", иначе "стоять"*/
    end
endmodule
