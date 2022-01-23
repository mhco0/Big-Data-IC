if (!exists("gk_file")) print "error_rank not included"
if (!exists("dcs_file")) print "dcs_file not included"
if (!exists("q_digest_file")) print "q_digest_file not included"
if (!exists("kll_file")) print "kll_file not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name

plot gk_file using (0):1 with boxplot title "gk", \
     dcs_file  using (1):1 with boxplot title "dcs", \
     q_digest_file  using (2):1 with boxplot title "q_digest", \
     kll_file  using (3):1 with boxplot title "kll"

pause -1