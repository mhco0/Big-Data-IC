if (!exists("data_file")) print "data_file not included"
if (!exists("data_name")) print "data_name not included"
if (!exists("xname")) print "xname not included"
if (!exists("yname")) print "yname not included"
if (!exists("title_name")) print "title_name not included"

set xlabel xname
set ylabel yname

set title title_name

#n = 10000
#min = 0
#max = 1023
#width = (max - min) / n
#hist(x, width)= (width / 2.0) + width * floor(x / width)

#set size sq

#plot data_file using (hist($1, width)):(1.0) smooth freq with boxes title data_name

#clear
#reset
#set key off
#set border 3

# Add a vertical dotted line at x=0 to show centre (mean) of distribution.
#set yzeroaxis

# Each bar is half the (visual) width of its x-range.
set boxwidth 0.05 absolute
#set style fill solid 1.0 noborder

bin_width = 0.1;

bin_number(x) = floor(x / bin_width)

rounded(x) = bin_width * ( bin_number(x) + 0.5 )

plot data_file using (rounded($1)):(1) smooth frequency with boxes fillcolor rgb "purple" title data_name

pause -1