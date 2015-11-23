
# PlanAhead Launch Script for Post-Synthesis floorplanning, created by Project Navigator

create_project -name NES_CONTROLLER_TEST -dir "C:/Madsen/NES_CONTROLLER_TEST/planAhead_run_1" -part xc6slx45fgg484-3
set_property design_mode GateLvl [get_property srcset [current_run -impl]]
set_property edif_top_file "C:/Madsen/NES_CONTROLLER_TEST/nes_controller.ngc" [ get_property srcset [ current_run ] ]
add_files -norecurse { {C:/Madsen/NES_CONTROLLER_TEST} }
set_property target_constrs_file "nes_controller.ucf" [current_fileset -constrset]
add_files [list {nes_controller.ucf}] -fileset [get_property constrset [current_run]]
open_netlist_design
