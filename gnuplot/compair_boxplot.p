if (!exists("data_files")) print "data_files not included"
if (!exists("data_files_names")) print "data_files_names not included"
if (!exists("box_names")) print "box_names not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name
set grid y
set yrange [-0.001:]
#set ytics autofreq 0.0025
set key noautotitle
#set style boxplot nooutliers

posX = 1
padding_factor = 1.5

set for [i = 1 : words(box_names)] xtics (word(box_names, i) (i + padding_factor))
set key right top
set key outside

plot for [i = 1 : words(data_files)] posX = i + padding_factor word(data_files, i) using (posX):1 with boxplot lc i, \
    for [i = 1 : words(data_files)] NaN with boxes lc i title word(data_files_names, i)


pause -1

