if {![namespace exists ::IMEX]} { namespace eval ::IMEX {} }
set ::IMEX::dataVar [file dirname [file normalize [info script]]]
set ::IMEX::libVar ${::IMEX::dataVar}/libs

create_library_set -name generic_library_set\
   -timing\
    [list ${::IMEX::libVar}/mmmc/NangateOpenCellLibrary.lib]
create_op_cond -name generic_opcond -library_file ${::IMEX::libVar}/mmmc/NangateOpenCellLibrary.lib -P 1 -V 1.1 -T 25
create_rc_corner -name generic_rc_corner\
   -preRoute_res 1\
   -postRoute_res {1 1 1}\
   -preRoute_cap 1\
   -postRoute_cap {1 1 1}\
   -postRoute_xcap {1 1 1}\
   -preRoute_clkres 0\
   -preRoute_clkcap 0\
   -postRoute_clkcap {1 1 1}\
   -postRoute_clkres {1 1 1}\
   -T 25
create_delay_corner -name generic_delay_corner\
   -rc_corner generic_rc_corner\
   -early_library_set generic_library_set\
   -late_library_set generic_library_set\
   -early_opcond generic_opcond\
   -late_opcond generic_opcond
create_constraint_mode -name generic_constraints\
   -sdc_files\
    [list ${::IMEX::libVar}/mmmc/design.sdc]
create_analysis_view -name generic_view -constraint_mode generic_constraints -delay_corner generic_delay_corner
set_analysis_view -setup [list generic_view] -hold [list generic_view]
