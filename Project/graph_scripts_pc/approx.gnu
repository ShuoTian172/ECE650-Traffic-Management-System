set terminal postscript eps color 
set terminal jpeg
set output 'approximation ratio.jpg'
set xlabel 'Number of vertices'
set ylabel 'Approximation ratio'
set xrange[4:16]
set yrange[0.75:2.5]
set xtics 1
set ytics 0.05
plot "approx1.dat" using 1:2:3 with yerrorlines title 'APPROX-1',"approx2.dat" using 1:2:3 with yerrorlines title 'APPROX-2'