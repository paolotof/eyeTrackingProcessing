# eyeTrackingProcessing

collection of c++ routines to analyze eyetracking data collected with SR-research eyetracking. 

The script:
- computes fixation into region of interests;
- linearly interpolates blink longer shorter than 300 ms using the median of the points tracked 50 ms before and after the blink
- extracts only trials in which correct responses were given
- exclude trials with too many/too long blinks within the time interval of interest
- counts fixations/saccades
- combines all participants results in one big file
- bins all observations into longer time intervals

# TODO list:
- implement better use of c++ containers
- implementation of parallel computing routines
