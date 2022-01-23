if (!exists("data_file")) print "data_file not included"
if (!exists("avg_data_name")) print "avg_data_name not included"
if (!exists("data_name")) print "data_name not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name

print data_file

plot [5:8] data_file using (log10($1)):5:3 with yerrorbars title data_name, \
    [5:8] data_file using (log10($1)):5 with lines title avg_data_name

pause -1