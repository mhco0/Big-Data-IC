if (!exists("quadtree_bounds_file")) print "quadtree_bounds_file not included"
if (!exists("points_file")) print "points_file not included"

set style arrow 1 nohead
plot quadtree_bounds_file u 1:2:($3-$1):(0) with vectors nohead lc 1 title 'Bounds', \
    '' u 3:2:(0):($4-$2) with vectors nohead lc 1 notitle, \
    '' u 3:4:($1-$3):(0) with vectors nohead lc 1 notitle, \
    '' u 1:4:(0):($2-$4) with vectors nohead lc 1 notitle, \
    points_file using 1:2 pt 7 ps 0.1 title "Data"

pause -1