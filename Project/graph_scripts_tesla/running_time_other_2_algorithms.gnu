set terminal postscript eps color enh
set terminal jpeg
set output 'Running time for other two algorithms.jpg'
set xlabel 'Number of vertices'
set ylabel 'Running time(us)'
set xrange[4:20]
set yrange[0:400]
set xtics 1
#set ytics 2
plot "a1time.dat" using 1:2:3 with yerrorlines title 'APPROX-1',"a2time.dat" using 1:2:3 with yerrorlines title 'APPROX-2'
