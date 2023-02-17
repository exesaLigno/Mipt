all: display

display: DISPLAY.v D7seg.v Gen1ms.v Gen4an.v Gen_P.v MUX16_4.v
	iverilog DISPLAY.v D7seg.v Gen1ms.v Gen4an.v Gen_P.v MUX16_4.v -o DISPLAY.out

clean:
	rm *.out
