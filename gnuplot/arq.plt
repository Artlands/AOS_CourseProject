
set style data histogram
set style histogram clustered
set boxwidth 1 relative
set xlabel "{/:Bold=15 Category}"offset 0,0
set yrange [-5:100]
set xrange [-0.5:10]
set ytics 10
set mytics 1
set xtics nomirror
set ytics nomirror
set ylabel "{/:Bold=15 Power Saving (%)}" offset 0,0
set style fill pattern 6 border 7
set style line 2 linewidth 2
set key autotitle columnhead
set key font "Helvetica Bold,12"
set key horizontal top center
#set key vertical top right
set grid ytics lw 1 lc 15
set border lw 2
set xtics font "Helvetica Bold,12" rotate by 270 offset 1,0
set ytics font "Helvetica Bold,12"
set bmargin 20
set lmargin 22
set tmargin 2
set key on
set xtic scale 0
set terminal png size 1200,800
set output "power_saving.png"

plot "./all.txt" using 2:xticlabels(1) lc 2 fs pattern 1 border 50,\
'' using 3:xticlabels(1) lc  7 fs pattern 7 border -1,\
'' using 4:xticlabels(1)  lc 53  fs pattern 6 border 7,\

#plot "./all.txt" using 2:xticlabels(1) lc  7 fs pattern 7 border -1,\
#'' using 3:xticlabels(1) lc 53  fs pattern 1 border 7,\
# '' using 4:xticlabels(1)  lc rgb "navy"  fs solid 1 noborder
