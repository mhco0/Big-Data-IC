if (!exists("real_rank")) print "real_rank not included"
if (!exists("app_rank")) print "app_rank not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name

plot real_rank with linespoints title "real_rank", \
     app_rank with linespoints title "app_rank"

pause -1