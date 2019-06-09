set terminal postscript eps color enh
set terminal jpeg
set output 'Running time for cnf.jpg'
set xlabel 'Number of vertices'
set ylabel 'Running time(us)'
set xrange[4:20]
set yrange[30:1.5e+09]
set logscale y
set xtics 1
#set ytics 20000
plot "cnftime.dat" using 1:2:3 with yerrorlines title 'CNF-SAT'
