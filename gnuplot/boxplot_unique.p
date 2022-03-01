if (!exists("data_file")) print "data_file not included"
if (!exists("data_name")) print "data_name not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name
#set ytics autofreq 0.5

plot data_file using (0):1 with boxplot title data_name

pause -1