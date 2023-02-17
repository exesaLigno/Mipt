module tf_VCB4RE;
    // Inputs
    reg ce;
    reg clk;
    reg clr;
    // Outputs
    wire [2:0] Q;
    wire TC;
    wire CEO;
    // Instantiate the Unit Under Test (UUT)
    VCB4RE uut (.ce(ce), .Q0(Q0), .Q1(Q1), .Q2(Q2), .Q3(Q3), .clk(clk), .TC(TC), .r(r), .CEO(CEO) );
    // Генратор периодичеккого сигнала синхронизации clk
    parameter Tclk=20; // Период сигнала синхронизации 20 нс
    always begin clk=1; #(Tclk/2); clk=0; #(Tclk/2); end
    // Генератор периодического сигнала ce
    parameter Tce=160; //Период сигнала ce 80 нс
    always begin ce=0; #(7*Tce/8); ce=1; #(1*Tce/8); end //Только целая часть деления
    initial 
    begin // Initialize Inputs
        clr = 0; //Исходное состояние входов
        #380; clr = 1; // Через 38 нс 1
        #10; clr = 0; // Через 10 нс 0
        #611; clr = 1; // Через 611 нс 1
        #270; clr = 0; // Через 270 нс 0
    end
endmodule
