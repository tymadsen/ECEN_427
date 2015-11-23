restart

isim force add Bus2IP_Clk 0 -time 0 -value 1 -time 5ns -repeat 10ns
isim force add Bus2IP_Resetn 0 -time 0 -value 1 -time 10us

put C1_DATA 0
put C2_DATA 0

run 25 us

put C1_DATA 1
put C2_DATA 0

run 10 us

put C1_DATA 0
put C2_DATA 1

run 10 us

put C1_DATA 1
put C2_DATA 1

run 10 us

put C1_DATA 0
put C2_DATA 0

run 10 us

put C1_DATA 1
put C2_DATA 0

run 10 us

put C1_DATA 0
put C2_DATA 1

run 10 us

put C1_DATA 1
put C2_DATA 1

run 50 us


#source user_logic_tcl.tcl