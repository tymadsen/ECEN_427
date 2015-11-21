restart

isim force add clk 0 -time 0 -value 1 -time 5ns -repeat 10ns
isim force add rst 1 -time 0 -value 0 -time 10us

put C1_DATA 0
put C2_DATA 0

run 50 us

#source nes_controller_tb.tcl