if (!exists("data_file")) print "data_file not included"
if (!exists("data_name")) print "data_name not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name

plot data_file  using 1:2 with linespoints title data_name

pause -1