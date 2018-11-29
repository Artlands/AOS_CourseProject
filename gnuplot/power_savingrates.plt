
set style data histogram
set style histogram clustered
set boxwidth 1 relative
#set xlabel "{/:Bold=12 COMPONENTS}"offset 0,-4
set yrange [0:100]
set xrange [-0.5:9]
set ytics 10
set mytics 1
set xtics nomirror
set ytics nomirror
set ylabel "{/:Bold=16 Power Saving Rates (%)}" offset -4,0
set style fill pattern 6 border 7
set style line 2 linewidth 1
set key autotitle columnhead
set key font "Helvetica Bold,12"
set key vertical
set grid ytics lw 1 lc 15
set border lw 2
set xtics font "Helvetica Bold,12" rotate by 270 offset 2,-1
set ytics font "Helvetica Bold,12"
set bmargin 20
set lmargin 22
set tmargin 2
set key off
set xtic scale 0

plot "./to_plot.txt"  using 2:xticlabels(1)  lc rgb "black"  fs pattern 3 border 7
#using 2:xticlabels(1)  lc 7  fs pattern 5 border -1,\
#'' using 3:xticlabels(1)  lc rgb "black"  fs pattern 3 border 7
