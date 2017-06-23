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
- traslate the icmake routines into cmake

There is a problem with the interpolations when data extraction start 1000 ms before target onset and it has to do with the fact that sometimes there is not 1000 ms between the onset of the target and the display of the message with "onsetSoundStim" on the eyetracking computer. Because of that we assigned the timeBefore to be 20 ms less than the shorted interval between the onset of the sound stimulus and the onset of the target word/item

# this is R code
charlotte <- read.table("/home/paolot/ownCloud/Shared/eye-tracking_rhythm/charlotte/Exp1/position_fromGrids_Wilke.txt")
names(charlotte) <- c("item", "onset")
min(charlotte$onset)
marleen <- read.table("/home/paolot/ownCloud/Shared/eye-tracking_rhythm/marleen/ciCci/eyetracker/targetonsetsAll_mod.txt")
names(marleen) <- c("item", "onset")
min(marleen$onset)
min(c(charlotte$onset, marleen$onset))
