from PyReaxys import *

cIds = [
	43170, 51314, 52979, 57397, 71846, 72407, 75107, 97332, 125513, 216260,
	293793, 296858, 605308, 606441, 832415, 864517, 1088085, 1209486,
	1869400, 4750507, 6326519, 6331253, 6336914, 6345164, 6346670, 6624574,
	6829883, 6830503, 8635587, 8635602, 8657602, 8658486, 8933321, 9272532,
	9277624, 9279802, 9283985, 9285924, 9286046, 9770283, 9803030, 9827747,
]

rIds = [
	328856, 406667, 406670, 679101, 769989, 2508784, 3511250, 3687019,
	5679444, 5680485, 6123858, 6151157, 8637533, 8655921, 8661293, 9179092,
	9182499, 9204333, 9205044, 9221902, 9223710, 9442925, 9690099, 13553214,
	14048128, 14587542, 14933381, 16309504, 18402227, 18762882, 18981738,
	18999370, 19000437, 19002012,
]


db = Database("/home/jla/stuff/reaxys")

cs = set()
rs = []
for i in cIds:
	c = db.compoundFromId(i)
	if not c:
		raise Exception("Compound %d does not exist." % i)
	cs.add(c)
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
for c in cs:
	print(c)
for r in rs:
	print(r)
