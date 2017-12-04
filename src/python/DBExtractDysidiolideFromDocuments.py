from PyReaxys import *

rIds = [4852418,
8543822,
8612166,
8612188,
8617726,
8629697,
8632647,
8644084,
8646713,
8648201,
8650626,
8652694,
8652713,
8652909,
8653438,
8662130,
10171849,
10207836,
14519122,
14519123,
14519124,
14519125,
14519126,
14519127,
14519128,
14519130,
14519131,
14519133,
14519134,
14519135,
14531630,
14531631,
14531632,
14531633,
14531634,
14531636,
14531637,
14531639,
14531640,
14531641,
14531700,
14531701,
14531702,
14531703,
14531704,
14531706,
14531707,
14531709,
14531710,
14531729,
14531730,
14531731,
14531732,
14531733,
14531734,
14531736,
14531737,
14531739,
14531740,
14531741,
14531785,
14531786,
14531787,
14531789,
14531790,
14531792,
14531793,
14531804,
14531805,
14531806,
14531808,
14531810,
14531811,
14531835,
14531836,
14531837,
14531838,
14531840,
14531841,
14531852,
14531853,
14531854,
14531859,
14531860,
14531861,
14531864,
14531865,
14531866,
14531867,
14531869,
14531870,
14531872,
14531873,
14531884,
14531885,
14531886,
14531887,
15756471,
15756472,
15756473,
15764627,
15764628,
15764629,
15764630,
15764631,
15764632,
15775838,
15779268,
15779269,
15779270,
15779271,
15779272,
15779273,
15780598,
15780599,
15780600,
15780601,
15780602,
15780603,
15780905,
15780906,
15780907,
15780908,
15780909,
15780910,
15783873,
15783874,
15783875,
15783876,
15783877,
15783878,
15784346,
15784347,
15784348,
15784349,
15784350,
15784351,
15785026,
15785027,
15785028,
15785029,
15785030,
15785031,
15785032,
15785033,
15785034,
15785035,
15785369,
15785370,
15785371,
15785372,
15785373,
15787501,
15788045,
15788046,
15789097,
15789098,
15789099,
15789100,
15789101,
15789102,
15789103,
15789104,
15790475,
15790476,
15790477,
15790478,
15790479,
15790480,
15790493,
15790494,
15790495,
15790496,
15790497,
15790498,
15790499,
15790500,
15790501,
15790667,
15790668,
15791157,
15791158,
15791159,
15791160,
15791161,
15791162,
15791163,
1022821,
1128860,
1327601,
4821416,
4825593,
4828485,
4836632,
4837782,
4838391,
4840172,
4841071,
4848493,
4848739,
4848932,
4851747,
4852418,
4855133,
5142034,
11555090,
15775838,
16389884,
17028614,
17028615,
17028616,
17040695,
17040696,
17040697,
17040698,
17040699,
17040700,
17040701,
17040702,
17040703,
17043051,
17043058,
17047865,
17047866,
17047867,
17047868,
17047869,
17047870,
17047871,
17047872,
17047873,
17047874,
17047875,
17047876,
17047877,
17047878,
17047879,
17049106,
17049107,
17049108,
17049109,
17049110,
17049111,
17049112,
17049113,
17049114,
17052637,
17052638,
17052639,
17052640,
17052641,
17052642,
17052643,
17052644,
17057219,
17057220,
17057221,
17057222,
17057223,
17057224,
17057225,
17061212,
17061213,
17061214,
17061215,
17061216,
17061560,
17061712,
17061713,
17061714,
17061715,
17061716,
17061717,
17064879,
17064880,
17064881,
17064882,
17065215,
17065216,
17065990,
17065991,
2288500,
3326716,
5061041,
5066207,
5067627,
5075166,
5075227,
5075479,
5075721,
5075840,
5076023,
5076155,
5076389,
5076390,
5076561,
5077689,
5077997,
5078048,
5082484,
8524625,
8524705,
8525475,
8540393,
8540746,
8541916,
15475453,
15475454,
16555519,
16555520,
16555521,
16555522,
16555523,
16555524,
16555525,
16555526,
16555527,
16555528,
16555529,
16555530,
16555531,
16555532,
16555533,
16555534,
16555535,
16555536,
16555537,
16555538,
16555539,
16555540,
16555541,
16555542,
16565790,
16565791,
16565792,
16565793,
16565794,
16565795,
16565796,
16565797,
16565798,
16565799,
16565800,
16565801,
16565802,
16565803,
16565804,
16565805,
16565806,
16565807,
16565808,
16565809,
16565810,
16565811,
16565812,
16584051,
16584052,
16584053,
16584054,
16584055,
16584056,
16584057,
16584058,
16584059,
16584060,
16584061,
16584062,
16584063,
16584064,
16584065,
16584066,
16584067,
16584068,
16584069,
16584070,
16586490,
16586491,
16586492,
16587365,
16587366,
16587367,
16587368,
16587369,
16587370,
16587371,
16587372,
16587373,
16587374,
16587375,
16587376,
16587377,
16587378,
16587443,
16587444,
16587517,
16587518,
16587519,
16587520,
16587521,
16587522,
16587523,
16587524,
16587525,
16587526,
16587527,
16587528,
16587529,
16587530,
16587531,
16587532,
16587533,
16587534,
16587535,
16587536,
16587537,
16587538,
16587539,
16587907,
16587908,
16587909,
16587910,
16587911,
16587912,
16587913,
16587914,
16587915,
16587916,
16587917,
16587918,
16587919,
16587920,
16587921,
16587922,
16587926,
16587927,
16587928,
16587929,
16587930,
16587931,
16587932,
16587933,
16587934,
16587935,
16587936,
16587937,
16587938,
16587939,
16587940,
16587941,
16587942,
16587943,
16587944,
16588253,
16588254,
16588255,
16588256,
16588257,
16588258,
16588259,
16588260,
16588261,
16588262,
16588263,
16588264,
16588265,
16588266,
16588267,
16588268,
16588269,
16588270,
16588271,
16588272,
16588273,
16588431,
16588432,
16588433,
16588434,
16588435,
16588436,
16588437,
16588438,
16588439,
16588440,
16588441,
16588442,
16588443,
16588444,
16588445,
16588446,
16588447,
16588448,
16588670,
16588671,
16588672,
16588673,
16588674,
16588675,
16588676,
16588677,
16588678,
16588679,
16588680,
16588681,
16588682,
16588683,
16588684,
16588874,
16589236,
16589237,
16589486,
16589487,
16589488,
16589489,
16589490,
16589491,
16589492,
16589493,
16589494,
16589495,
16589496,
16589497,
16589498,
16589499,
16589500,
16589501,
16589502,
16591463,
16591464,
16591465,
16591466,
16591467,
16591468,
16591469,
16591885,
16591886,
16591887,
16591888,
16591889,
16591890,
16591891,
16591892,
16591893,
16591894,
16591895,
16591989,
16591990,
16591991,
16591992,
16591993,
16591994,
16592102,
16592103,
16592104,
16592105,
16592106,
16592212,
16592213,
16592214,
16592215,
16592216,
16592217,
16592218,
16592219,
16592220,
16592221,
16592222,
16592223,
16592746,
16592747,
16592748,
16592749,
16592750,
16592751,
16592752,
16592753,
16592754,
16592755,
16593454,
16593455,
16593456,
16593457,
16593458,
16593459,
16593460,
16593461,
16593462,
16594255,
16594256,
16594257,
16594258,
16594259,
16594260,
16594261,
16594262,
2220268,
4821442,
4825544,
4826053,
4826715,
4827140,
4827919,
4830144,
4835319,
4841054,
4841257,
4841445,
4841618,
4841619,
4844971,
4845388,
4850101,
4851746,
4852899,
4852900,
17035360,
17035361,
17035362,
17035363,
17035364,
17035365,
17035366,
17035367,
17035368,
17035369,
17035370,
17035371,
17035372,
17035373,
17035374,
17035375,
17035376,
17035675,
17043546,
17043547,
17043548,
17043549,
17043550,
17043551,
17043552,
17043553,
17043554,
17043555,
17043743,
17043744,
17043745,
17043746,
17043747,
17043748,
17043749,
17043750,
17043751,
17043752,
17043753,
17043754,
17043755,
17043756,
17043757,
17043758,
17049073,
17049074,
17049075,
17049076,
17049077,
17049078,
17049079,
17049080,
17049081,
17049082,
17049083,
17049084,
17049085,
17049086,
17049087,
17049249,
17049250,
17049251,
17049252,
17049253,
17049254,
17049255,
17049256,
17049257,
17049258,
17049259,
17049260,
17049261,
17049262,
17049263,
17049739,
17049740,
17049741,
17049742,
17049743,
17049744,
17049745,
17049746,
17049747,
17049748,
17049749,
17049750,
17049751,
17049752,
17049875,
17049876,
17049877,
17049878,
17049879,
17049880,
17049881,
17049882,
17049883,
17049884,
17049885,
17049886,
17049887,
17049888,
17049948,
17049949,
17049950,
17049951,
17049952,
17049953,
17049954,
17049955,
17049956,
17049957,
17050077,
17050078,
17050079,
17050080,
17050081,
17050082,
17050083,
17050084,
17050085,
17050086,
17055270,
17055271,
17055272,
17055273,
17055274,
17055275,
17055276,
17055960,
17055961,
17055962,
17055963,
17055964,
17060739,
17060740,
17060741,
17060742,
17060743,
17060744,
17063161,
17063162,
17063163,
17063164,
17063165,
17063166,
17063167,
17063168,
17064875,
17064876,
17064877,
17064878,
17065213,
17065214,
17066558,
17066559,
17066560,
17066561,
17066562,
17066563,
17066564,
17066565,
17066566,
]


db = Database("/scr/k61san/jlandersen")

cs = set()
rs = []
for i in rIds:
	r = db.reactionFromId(i)
	if not r:
		raise Exception("Reaction %d does not exist." % i)
	rs.append(r)
for r in rs:
	for c in r.educts:
		cs.add(c[1])
	for c in r.products:
		cs.add(c[1])
print("digraph G {")
for c in cs:
	if c: 
		print(c.id, end=' ')
		print("N", end=' ')
		print("notS", end=' ')
		print("Weight", end=' ')
		print(c.molWeight) #Hardcoded Weight until real weight is added to DB
for r in rs:
	if(r.types[0] != 3):
		if(r.yields):
			print(r.id, end=' ')
			print("R", end=' ')
			print("Yield", end=' ')
			print(r.yields[0][1]) #Hardcoded Yield until real yield is added to DB #rojin: yields has (object, yield)
print("REACTIONS")
for r in rs:
	for c in r.educts:
		print(str(r.id)+"->"+str(c[1].id) )	
	for c in r.products:
		print(str(c[1].id)+"->"+str(r.id) )
print("}")
