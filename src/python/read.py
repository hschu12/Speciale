from PyReaxys import *

db = Database("/home/jla/stuff/reaxys")

c = db.compoundFromId(43170)
cd = {c: 42, c: 60}
cs = set([c, c])
print("cd:", cd)
print("cs:", cs)
print("Compound:", c.id)
print("Names:")
for n in c.names:
	print("   ", n)
print("Out:")
for r in c.outReactions:
	print("   ", r)
print("In:")
for r in c.inReactions:
	print("   ", r)

r = db.reactionFromId(328856)
rd = {r: 42, r: 60}
rs = set([r, r])
print("rd:", rd)
print("rs:", rs)
print("Reaction:", r)
print("Types:")
for t in r.types:
	print("   ", t)
print("Educts:")
for c in r.educts:
	print("   ", c)
print("Products:")
for c in r.products:
	print("   ", c)
