transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/altera_lite/16.0/projects/uart_v2 {C:/altera_lite/16.0/projects/uart_v2/DE1_SOC_golden_top.v}
vlog -vlog01compat -work work +incdir+C:/altera_lite/16.0/projects/uart_v2 {C:/altera_lite/16.0/projects/uart_v2/uart_v2.v}
vlog -vlog01compat -work work +incdir+C:/altera_lite/16.0/projects/uart_v2 {C:/altera_lite/16.0/projects/uart_v2/baud_rate_gen.v}
vlog -vlog01compat -work work +incdir+C:/altera_lite/16.0/projects/uart_v2 {C:/altera_lite/16.0/projects/uart_v2/receiver.v}
vlog -vlog01compat -work work +incdir+C:/altera_lite/16.0/projects/uart_v2 {C:/altera_lite/16.0/projects/uart_v2/transmitter.v}

