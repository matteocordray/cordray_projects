transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/altera_lite/16.0/projects/uart_v1 {C:/altera_lite/16.0/projects/uart_v1/DE1_SOC_golden_top.v}
vlog -vlog01compat -work work +incdir+C:/altera_lite/16.0/projects/uart_v1 {C:/altera_lite/16.0/projects/uart_v1/uart_rx.v}

