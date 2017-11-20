from PyReaxys import *

K = 4

cIds = [
	52979,
	]

db = Database("/scr/k61san/jlandersen")

cs = set()
rs = set()
iterationRs = set()
iterationCs = set()
for i in cIds:
	c = db.compoundFromId(i)
	if not c:
		raise Exception("Compound %d does not exist." % i)
	cs.add(c)
	iterationCs.add(c)
for i in range(0, K): #expand K steps out from target compound
	for c in iterationCs:
		for r_tup in c.inReactions:
			r = r_tup[1]
			if not r:
				raise Exception("Reaction %d does not exist." % i)
			iterationRs.add(r)
			rs.add(r)
	iterationCs.clear()
	for r in iterationRs:
		for c in r.educts:
			cs.add(c[1])
			iterationCs.add(c[1])
	iterationRs.clear()

print("digraph G {")
for c in cs:
	print(c.id, end=' ')
	print("N", end=' ')
	print("notS", end=' ')
	print("Weight", end=' ')
	print("2") #Hardcoded Weight until real weight is added to DB
for r in rs:
	print(r.id, end=' ')
	print("R", end=' ')
	print("Yield", end=' ')
	print("0.8") #Hardcoded Yield until real yield is added to DB
print("REACTIONS")
for r in rs:
	for c in r.educts:
		print(str(r.id)+"->"+str(c[1].id) )	
	for c in r.products:
		print(str(c[1].id)+"->"+str(r.id) )
print("}")
