set boxwidth 0.5 absolute
set style fill solid 1.00 border lt -1
set key outside right top vertical Left reverse noenhanced autotitle columnhead nobox
set key invert samplen 4 spacing 1 width 0 height 0
set style increment default
set style histogram rowstacked title textcolor lt -1
set style data histogram
set xtics border in scale 0,0 nomirror autojustify
set xtics norangelimit
set xtics ()

set title "{/:Bold=15 Power Consumption using SCATTER benchmark}"
set key font "Helvetica Bold,12"
set ylabel "{/:Bold=12 POWER(WATTS)}" offset 2,0
set ytics font "Helvetica Bold,12"
set xtics font "Helvetica Bold,12"

set terminal png size 800,600
set output "stack_power.png"

set xrange[ * : * ] noreverse writeback
set x2range[ * : * ] noreverse writeback
set yrange [0:22000] noreverse writeback
set ytics 2000
set y2range[ * : * ] noreverse writeback
set zrange[ * : * ] noreverse writeback
set cbrange[ * : * ] noreverse writeback
set rrange[ * : * ] noreverse writeback


# the x-ticks are coming from column 1
plot './PowerStack.txt' using 2:xtic(1)\
    ,'' using 3 \
    ,'' using 4 \
    ,'' using 5 \
    ,'' using 6 \
    ,'' using 7 \
    ,'' using 8 \
    ,'' using 9 \
    ,'' using 10
