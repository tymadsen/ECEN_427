restart

isim force add Bus2IP_Clk 0 -time 0 -value 1 -time 5ns -repeat 10ns
isim force add Bus2IP_Resetn 0 -time 0 -value 1 -time 10us

run 10 us
isim force add Bus2Ip_data 0x0a000002
isim force add Bus2Ip_be 0xf
isim force add Bus2IP_WrCE 0xf000
isim force add mst_reg(0) 0x02
run 10 us

isim force add mst_reg(8) 0xff
run 10 us
isim force add mst_reg(9) 0xff
run 10 us
isim force add mst_reg(12) 0x04
run 10 us
isim force add mst_reg(15) 0x0a
run 10 us
isim force add Bus2IP_Mst_CmdAck 1 -time 0 -value 0 -time 10 us -repeat 20us
isim force add Bus2IP_Mst_Cmplt 0 -time 0 -value 1 -time 10 us -repeat 20us

run 40 us


run 10 ms


#  mst_cntl_rd_req   <= mst_reg(0)(0);
#  mst_cntl_wr_req   <= mst_reg(0)(1);
#  mst_cntl_bus_lock <= mst_reg(0)(2);
#  mst_cntl_burst    <= mst_reg(0)(3);
#  mst_ip2bus_addr   <= mst_reg(7) & mst_reg(6) & mst_reg(5) & mst_reg(4);
#  mst_ip2bus_be     <= mst_reg(9) & mst_reg(8);
#  mst_xfer_length   <= mst_reg(13)(3 downto 0) & mst_reg(12);
#

#1. write 0x40 to the control register
#  --   2. write the target address to the address register
#  --   3. write valid byte lane value to the be register
#  --      - note: this value must be aligned with ip2bus address
#  --   4. write 0x0004 to the length register
#  --   5. write 0x0a to the go register, this will start the master write operation

