int16_t pongloopstart = 18;

const int16_t pongpath[][2] = {
  {0,-6},{0,-5},{0,-4},{0,-3},{0,-2},{0,-1},
  {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
  {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
  
  // begin of loop

	// draw line from (0,0) to (21,60)

	// deltax: 21, deltay = 60

	{0,0},{0,1},{0,2},{1,3},
	{1,4},{1,5},{2,6},{2,7},
	{2,8},{3,9},{3,10},{3,11},
	{4,12},{4,13},{4,14},{5,15},
	{5,16},{5,17},{6,18},{6,19},
	{7,20},{7,21},{7,22},{8,23},
	{8,24},{8,25},{9,26},{9,27},
	{9,28},{10,29},{10,30},{10,31},
	{11,32},{11,33},{11,34},{12,35},
	{12,36},{12,37},{13,38},{13,39},
	{14,40},{14,41},{14,42},{15,43},
	{15,44},{15,45},{16,46},{16,47},
	{16,48},{17,49},{17,50},{17,51},
	{18,52},{18,53},{18,54},{19,55},
	{19,56},{19,57},{20,58},{20,59},
	{21,60},

	// wait at (21,60) for 10 cycles

	{21,60},{21,60},{21,60},{21,60},
	{21,60},{21,60},{21,60},{21,60},
	{21,60},{21,60},

	// draw line from (21,60) to (32,0)

	// deltax: 11, deltay = -60

	{21,60},{21,59},{21,58},{21,57},
	{21,56},{21,55},{22,54},{22,53},
	{22,52},{22,51},{22,50},{23,49},
	{23,48},{23,47},{23,46},{23,45},
	{23,44},{24,43},{24,42},{24,41},
	{24,40},{24,39},{25,38},{25,37},
	{25,36},{25,35},{25,34},{25,33},
	{26,32},{26,31},{26,30},{26,29},
	{26,28},{27,27},{27,26},{27,25},
	{27,24},{27,23},{27,22},{28,21},
	{28,20},{28,19},{28,18},{28,17},
	{29,16},{29,15},{29,14},{29,13},
	{29,12},{29,11},{30,10},{30,9},
	{30,8},{30,7},{30,6},{31,5},
	{31,4},{31,3},{31,2},{31,1},
	{32,0},

	// wait at (32,0) for 5 cycles

	{32,0},{32,0},{32,0},{32,0},
	{32,0},

	// draw line from (32,0) to (43,60)

	// deltax: 11, deltay = 60

	{32,0},{32,1},{32,2},{32,3},
	{32,4},{32,5},{33,6},{33,7},
	{33,8},{33,9},{33,10},{34,11},
	{34,12},{34,13},{34,14},{34,15},
	{34,16},{35,17},{35,18},{35,19},
	{35,20},{35,21},{36,22},{36,23},
	{36,24},{36,25},{36,26},{36,27},
	{37,28},{37,29},{37,30},{37,31},
	{37,32},{38,33},{38,34},{38,35},
	{38,36},{38,37},{38,38},{39,39},
	{39,40},{39,41},{39,42},{39,43},
	{40,44},{40,45},{40,46},{40,47},
	{40,48},{40,49},{41,50},{41,51},
	{41,52},{41,53},{41,54},{42,55},
	{42,56},{42,57},{42,58},{42,59},
	{43,60},

	// wait at (43,60) for 10 cycles

	{43,60},{43,60},{43,60},{43,60},
	{43,60},{43,60},{43,60},{43,60},
	{43,60},{43,60},

	// draw line from (43,60) to (32,0)

	// deltax: -11, deltay = -60

	{43,60},{43,59},{43,58},{43,57},
	{43,56},{43,55},{42,54},{42,53},
	{42,52},{42,51},{42,50},{41,49},
	{41,48},{41,47},{41,46},{41,45},
	{41,44},{40,43},{40,42},{40,41},
	{40,40},{40,39},{39,38},{39,37},
	{39,36},{39,35},{39,34},{39,33},
	{38,32},{38,31},{38,30},{38,29},
	{38,28},{37,27},{37,26},{37,25},
	{37,24},{37,23},{37,22},{36,21},
	{36,20},{36,19},{36,18},{36,17},
	{35,16},{35,15},{35,14},{35,13},
	{35,12},{35,11},{34,10},{34,9},
	{34,8},{34,7},{34,6},{33,5},
	{33,4},{33,3},{33,2},{33,1},
	{32,0},

	// draw line from (32,0) to (21,60)

	// deltax: -11, deltay = 60

	{32,0},{32,1},{32,2},{32,3},
	{32,4},{32,5},{31,6},{31,7},
	{31,8},{31,9},{31,10},{30,11},
	{30,12},{30,13},{30,14},{30,15},
	{30,16},{29,17},{29,18},{29,19},
	{29,20},{29,21},{28,22},{28,23},
	{28,24},{28,25},{28,26},{28,27},
	{27,28},{27,29},{27,30},{27,31},
	{27,32},{26,33},{26,34},{26,35},
	{26,36},{26,37},{26,38},{25,39},
	{25,40},{25,41},{25,42},{25,43},
	{24,44},{24,45},{24,46},{24,47},
	{24,48},{24,49},{23,50},{23,51},
	{23,52},{23,53},{23,54},{22,55},
	{22,56},{22,57},{22,58},{22,59},
	{21,60},

	// wait at (21,60) for 15 cycles

	{21,60},{21,60},{21,60},{21,60},
	{21,60},{21,60},{21,60},{21,60},
	{21,60},{21,60},{21,60},{21,60},
	{21,60},{21,60},{21,60},

	// draw line from (21,60) to (0,0)

	// deltax: -21, deltay = -60

	{21,60},{21,59},{21,58},{20,57},
	{20,56},{20,55},{19,54},{19,53},
	{19,52},{18,51},{18,50},{18,49},
	{17,48},{17,47},{17,46},{16,45},
	{16,44},{16,43},{15,42},{15,41},
	{14,40},{14,39},{14,38},{13,37},
	{13,36},{13,35},{12,34},{12,33},
	{12,32},{11,31},{11,30},{11,29},
	{10,28},{10,27},{10,26},{9,25},
	{9,24},{9,23},{8,22},{8,21},
	{7,20},{7,19},{7,18},{6,17},
	{6,16},{6,15},{5,14},{5,13},
	{5,12},{4,11},{4,10},{4,9},
	{3,8},{3,7},{3,6},{2,5},
	{2,4},{2,3},{1,2},{1,1},
	{0,0},
};

// count: 406 
