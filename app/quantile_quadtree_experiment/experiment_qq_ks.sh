#!/bin/bash
#./bin/quantile_quadtree_ks 10000 0 1023 1 10 50.0 0.003 1024 4 1 0.0 0.0 1280.0 720.0
#./bin/quantile_quadtree_ks 10000 0 1023 1 10 50.0 0.003 1024 3 0 -180.0 -90.0 180.0 90.0 "../../yellow_tripdata_2011-01 (1).csv"
#https://anthonylouisdagostino.com/bounding-boxes-for-all-us-states/
#./bin/quantile_quadtree_ks 0.003 1024 3 0 -79.762152 40.496103 -71.856214 45.01585 0 1023 1 -f "../../yellow_tripdata_2011-01 (1).csv" pickup_longitude pickup_latitude passenger_count integer
#https://www.openstreetmap.org/export#map=11/40.7106/-73.7698
./bin/quantile_quadtree_ks 0.003 1024 3 0 -74.2628 40.4762 -73.6983 40.9177 0 1023 1 -f "../../yellow_tripdata_2011-01 (1).csv" pickup_longitude pickup_latitude passenger_count integer
#./bin/quantile_quadtree_ks 0.003 1024 3 0 -79.762152 40.496103 -71.856214 45.01585 0 1023 1 -f "../../yellow_tripdata_2011-01 (1).csv" pickup_longitude pickup_latitude total_amount double