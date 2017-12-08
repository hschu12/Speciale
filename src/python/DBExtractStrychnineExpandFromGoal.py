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
		if c:
			for r_tup in c.inReactions:
				r = r_tup[1]
				if r:
					iterationRs.add(r)
					rs.add(r)
	iterationCs.clear()
	for r in iterationRs:
		if r:
			for c in r.educts:
				if c:
					cs.add(c[1])
					iterationCs.add(c[1])
	iterationRs.clear()

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
