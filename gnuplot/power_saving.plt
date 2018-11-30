set style data histogram
set style histogram clustered
set boxwidth 1 relative
#set xlabel "{/:Bold=12 COMPONENTS}"offset 0,-4
set yrange [-5:100]
set xrange [-0.5:10]
set ytics 10
set mytics 1
set xtics nomirror
set ytics nomirror
set ylabel "{/:Bold=16 Power Saving (%)}" offset -4,0
set style fill pattern 6 border 7
set style line 2 linewidth 1
set key autotitle columnhead
set key font "Helvetica Bold,12"
set key vertical
set grid ytics lw 1 lc 15
set border lw 2
set xtics font "Helvetica Bold,12" rotate by -90 offset 1,0
set ytics font "Helvetica Bold,12"
set bmargin 20
set lmargin 22
set tmargin 2
set key off
set xtic scale 0
set terminal png size 800,600
set output "power_saving.png"

plot "./PowerSaving.txt"  using 2:xticlabels(1)  lc rgb "black"  fs pattern 3 border 7
