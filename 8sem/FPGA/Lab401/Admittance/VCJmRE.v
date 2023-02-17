// Счетчик Джонсона
`define m 4
module VCJmRE  (input ce, output wire TC,
                input clk, output wire CEO,
                input R, output reg[`m-1:0] Q = 0);
    assign TC = (Q==(1<<`m)-1) ; // q0,q1,...q'm-1 ==1
    assign CEO = ce & TC ; // Сигнал переноса
    always @ (posedge clk) 
    begin
        Q <= R? 0 : ce? Q<<1 | !Q[`m-1] : Q ;
    end
endmodule
