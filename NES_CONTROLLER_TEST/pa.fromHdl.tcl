
# PlanAhead Launch Script for Pre-Synthesis Floorplanning, created by Project Navigator

create_project -name NES_CONTROLLER_TEST -dir "C:/Madsen/NES_CONTROLLER_TEST/planAhead_run_1" -part xc6slx45fgg484-3
set_param project.pinAheadLayout yes
set srcset [get_property srcset [current_run -impl]]
set_property target_constrs_file "nes_controller.ucf" [current_fileset -constrset]
set hdlfile [add_files [list {nes_controller.vhd}]]
set_property file_type VHDL $hdlfile
set_property library work $hdlfile
set_property top nes_controller $srcset
add_files [list {nes_controller.ucf}] -fileset [get_property constrset [current_run]]
open_rtl_design -part xc6slx45fgg484-3
