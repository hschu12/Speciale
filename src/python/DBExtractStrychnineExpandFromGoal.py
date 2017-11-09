from PyReaxys import *

K = 5

cIds = [
	52979,
	]

db = Database("/scr/k61san/jlandersen")

cs = set()
rs = set()
for i in cIds:
	c = db.compoundFromId(i)
	if not c:
		raise Exception("Compound %d does not exist." % i)
	cs.add(c)
for i in range(0, K): #expand K steps out from target compound
	for c in cs:
		for r in c.inReactions:
			rs.add(r)
	for r in rs:
		for c in r.educts:
			cs.add(c[1])
		for c in r.products:
			cs.add(c[1])

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
		print(r.id ,end='')
		print("->")
		print(c[1].id)
	for c in r.products:
		print(r.id ,end='')
		print("->")
		print(c[1].id)
print("}")