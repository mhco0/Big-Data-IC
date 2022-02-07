if (!exists("data_files")) print "data_files not included"
if (!exists("data_files_names")) print "data_files_names not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name
set grid y
set yrange [-0.1:1]
set ytics autofreq 0.25
set key noautotitle
set style boxplot nooutliers

quantiles = "0.25 0.5 0.75 0.99"
myXtic(n) = sprintf("%s", word(data_files, n))
posX = 1
cols = 4
padding_factor = 1.5

set for [i = 1 : words(quantiles)] xtics (word(quantiles, i) ((words(data_files) % 2 == 0) ? ((words(data_files) / 2  + 0.5 + padding_factor * words(data_files) * (i - 1))) : (words(data_files) / 2 +  padding_factor * words(data_files) * (i - 1) + 1)))
set key right top
set key outside

plot for [i = 1 : words(data_files)] for [quant = 1 : words(quantiles)] posX = i + ((quant - 1) * padding_factor * words(data_files)) word(data_files, i) using (posX):(100 * column(quant)) with boxplot lc i, \
    for [i = 1 : words(data_files)] NaN with boxes lc i title word(data_files_names, i)


pause -1

