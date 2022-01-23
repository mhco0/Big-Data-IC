if (!exists("gk_file_ct")) print "gk_file_ct not included"
if (!exists("dcs_file_ct")) print "dcs_file_ct not included"
if (!exists("q_digest_file_ct")) print "q_digest_file_ct not included"
if (!exists("kll_file_ct")) print "kll_file_ct not included"
if (!exists("gk_file_ut")) print "gk_file_ut not included"
if (!exists("dcs_file_ut")) print "dcs_file_ut not included"
if (!exists("q_digest_file_ut")) print "q_digest_file_ut not included"
if (!exists("kll_file_ut")) print "kll_file_ut not included"
if (!exists("gk_file_qt")) print "gk_file_qt not included"
if (!exists("dcs_file_qt")) print "dcs_file_qt not included"
if (!exists("q_digest_file_qt")) print "q_digest_file_qt not included"
if (!exists("kll_file_qt")) print "kll_file_qt not included"
if (!exists("gk_file_m")) print "gk_file_m not included"
if (!exists("dcs_file_m")) print "dcs_file_m not included"
if (!exists("q_digest_file_m")) print "q_digest_file_m not included"
if (!exists("kll_file_m")) print "kll_file_m not included"
if (!exists("title_name_ct")) print "title_name_ct not included"
if (!exists("title_name_ut")) print "title_name_ut not included"
if (!exists("title_name_qt")) print "title_name_qt not included"
if (!exists("title_name_m")) print "title_name_m not included"
if (!exists("xlabel1")) print "title_name_ct not included"
if (!exists("xlabel2")) print "title_name_ut not included"
if (!exists("xlabel3")) print "title_name_qt not included"
if (!exists("xlabel4")) print "title_name_m not included"
if (!exists("ylabel1")) print "title_name_ct not included"
if (!exists("ylabel2")) print "title_name_ut not included"
if (!exists("ylabel3")) print "title_name_qt not included"
if (!exists("ylabel4")) print "title_name_m not included"

set multiplot layout 1,2
set size square 0.5,1
#set key left top
set key outside

set xlabel xlabel1
set ylabel ylabel1

set title title_name_ct

plot gk_file_ct using (log10($1)):2 with linespoints title "gk", \
     dcs_file_ct using (log10($1)):2 with linespoints title "dcs", \
     q_digest_file_ct using (log10($1)):2 with linespoints title "q_digest", \
     kll_file_ct using (log10($1)):2 with linespoints title "kll"

set size square 0.5,1
#set key left top
set key outside

set xlabel xlabel2
set ylabel ylabel2

set title title_name_ut

plot gk_file_ut using (log10($1)):2 with linespoints title "gk", \
     dcs_file_ut using (log10($1)):2 with linespoints title "dcs", \
     q_digest_file_ut using (log10($1)):2 with linespoints title "q_digest", \
     kll_file_ut using (log10($1)):2 with linespoints title "kll"

pause -1

set xlabel xlabel3
set ylabel ylabel3

set title title_name_qt

pause -1

plot gk_file_qt using (log10($1)):2 with linespoints title "gk", \
     dcs_file_qt using (log10($1)):2 with linespoints title "dcs", \
     q_digest_file_qt using (log10($1)):2 with linespoints title "q_digest", \
     kll_file_qt using (log10($1)):2 with linespoints title "kll"


set xlabel xlabel4
set ylabel ylabel4

set title title_name_m

plot gk_file_m with linespoints title "gk", \
     dcs_file_m with linespoints title "dcs", \
     q_digest_file_m with linespoints title "q_digest", \
     kll_file_m with linespoints title "kll"

pause -1