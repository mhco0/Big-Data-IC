if (!exists("gk_file")) print "gk_file not included"
if (!exists("dcs_file")) print "dcs_file not included"
if (!exists("q_digest_file")) print "q_digest_file not included"
if (!exists("kll_file")) print "kll_file not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name

plot [5 : 8] gk_file using (log10($1)):2:3 with yerrorbars title "gk", \
     dcs_file using (log10($1)):2:3 with yerrorbars title "dcs", \
     q_digest_file using (log10($1)):2:3 with yerrorbars title "q_digest", \
     kll_file using (log10($1)):2:3 with yerrorbars title "kll"

pause -1