
set style data histogram
set style histogram clustered
set boxwidth 1 relative
set xlabel "{/:Bold=42 Benchmarks}"offset 0,-12
#set yrange [0:90]
set xrange [-0.5:12]
set logscale y 2
set format y "2 ^{%L}"
#set ytics 10
set mytics 1
set xtics nomirror
set ytics nomirror
set ylabel "{/:Bold=42 Bank Conflict Reductions}" offset -14,-4
set style fill pattern 6 border 7
set style line 2 linewidth 4
set key autotitle columnhead
set key font "Helvetica Bold,30"
set key vertical
set grid noxtics lw 1.5 lc 15
set border lw 2
set xtics font "Helvetica Bold,30" rotate by 270 offset 3,-3
set ytics font "Helvetica Bold,30"
set bmargin 20
set lmargin 26
set tmargin 2
set key off
set xtic scale 0

plot "./bank_conflict.txt" using 2:xticlabels(1) lc rgb "red"  fs solid 0.5 noborder

#plot "Data/pnnl/bank_conflict.txt" using 2:xticlabels(1) lc rgb "#FF1493"  fs solid 0.4 noborder
#'' using 2:xticlabels(1)  lc -1  fs pattern 3 border 7,\
