module Gen1ms ( input clk, // Сигнал синхронизации
                output wire ce1ms); //1 милисекунда
    parameter Fclk =50000000 ; // Частота генератора синхронизации 50 МГц
    parameter F1kHz =1000 ; // Частота 1 кГц
    reg[15:0]cb_ms = 0 ; // Счетчик миллисекунды
    assign ce1ms = (cb_ms==1) ; //1 милисекунда
    //Делитель частоты
    always @(posedge clk) 
    begin
        cb_ms <= ce1ms? ((Fclk/F1kHz)) : cb_ms-1 ; // Счет миллисекунд
    end
endmodule
