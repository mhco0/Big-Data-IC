if (!exists("data_file")) print "data_file not included"
if (!exists("data_names")) print "data_name not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name

set yrange [0:0.05]
set ytics autofreq 0.009    
set boxwidth 0.8
set style fill solid
cols = 2

set for [i = 1 : words(data_names)] xtics (word(data_names, i) (i))

plot for[i = 1 : cols] data_file using (i):i with boxes title word(data_names, i)

pause -1