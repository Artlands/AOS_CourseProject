#
# Stacked histograms
#
set style data histogram
set style histogram clustered

# Where to put the legend
# and what it should contain
set key invert reverse Left outside
set key autotitle columnheader
set key font "Helvetica Bold,12"

set yrange [0:100]
set ylabel "{/:Bold=15 TOTAL (%)}"
set ytics font "Helvetica Bold,12"
set xtics font "Helvetica Bold,15"

# Define plot style 'stacked histogram'
# with additional settings
set style data histogram
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.75

# We are plotting columns 2, 3 and 4 as y-values,
# the x-ticks are coming from column 1
plot './to_plot_stack_scatter.txt' using 2:xtic(1) \
    ,'' using 3 \
    ,'' using 4 \
    ,'' using 5 \
    ,'' using 6 \
    ,'' using 7 \
    ,'' using 8 \
    ,'' using 9 \
    ,'' using 10
