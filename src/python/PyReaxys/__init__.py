import sqlite3 as sql

fCompoundNames = "1_compoundNamesFromXml/compoundNames.sqlite"
fCompounds = "1_compoundsFromXml/compounds.sqlite"
fReactions = "1_reactionsFromXml/reactions.sqlite"
fEductsProducts = "2_eductProduct/eductsProducts.sqlite"


class InvalidOperation(Exception):
	def __init__(self, msg):
		self.msg = msg

	def __str__(self):
		return self.msg


class Compound(object):
	def __init__(self, db=None, id_=None):
		self._db = db
		self._id = id_
		self._names = None
		self._outReactions = None
		self._inReactions = None

	def __str__(self):
		if self._id is None:
			return "Compound(null)"
		else:
			return "Compound(%d)" % self._id

	def __bool__(self):
		return self._id is not None

	@property
	def id(self):
		return self._id

	@property
	def names(self):
		if self._id is None:
			raise InvalidOperation("Can not get names of a null Compound.")
		if self._names is None:
			res = self._db._conn.execute("SELECT compound_name FROM CNames.CompoundNames WHERE compound_id = ?", (self._id,))
			self._names = [row[0] for row in res]
		return self._names

	@property
	def outReactions(self):
		if self._id is None:
			raise InvalidOperation("Can not get out-reactions of a null Compound.")
		if self._outReactions is None:
			res = self._db._conn.execute("SELECT reaction_id, educt_multiplicity FROM EP.Educts WHERE educt_id = ?;", (self._id,))
			self._outReactions = [(row[1], self._db.reactionFromId(row[0])) for row in res]
		return self._outReactions

	@property
	def inReactions(self):
		if self._id is None:
			raise InvalidOperation("Can not get in-reactions of a null Compound.")
		if self._inReactions is None:
			res = self._db._conn.execute("SELECT reaction_id, product_multiplicity FROM EP.Products WHERE product_id = ?;", (self._id,))
			self._inReactions = [(row[1], self._db.reactionFromId(row[0])) for row in res]
		return self._inReactions


class Reaction(object):
	def __init__(self, db=None, id_=None):
		self._db = db
		self._id = id_
		self._types = None
		self._educts = None
		self._products = None

	def __str__(self):
		if self._id is None:
			return "Reaction(null)"
		else:
			return "Reaction(%d)" % self._id

	def __bool__(self):
		return self._id is not None

	@property
	def id(self):
		return self._id

	@property
	def types(self):
		if self._id is None:
			raise InvalidOperation("Can not get types of a null Reaction.")
		if self._types is None:
			res = self._db._conn.execute("SELECT reaction_record_id FROM R.ReactionRecords WHERE reaction_id = ?;", (self._id,))
			self._types = [self._db.reactionTypes[row[0]] for row in res]
		return self._types

	@property
	def educts(self):
		if self._id is None:
			raise InvalidOperation("Can not get educts of a null Reaction.")
		if self._educts is None:
			res = self._db._conn.execute("SELECT educt_id, educt_multiplicity FROM EP.Educts WHERE reaction_id = ?;", (self._id,))
			self._educts = [(row[1], self._db.compoundFromId(row[0])) for row in res]
		return self._educts

	@property
	def products(self):
		if self._id is None:
			raise InvalidOperation("Can not get products of a null Reaction.")
		if self._products is None:
			res = self._db._conn.execute("SELECT product_id, product_multiplicity FROM EP.Products WHERE reaction_id = ?;", (self._id,))
			self._products = [(row[1], self._db.compoundFromId(row[0])) for row in res]
		return self._products


class ReactionType(object):
	def __init__(self, name):
		self._name = name

	def __str__(self):
		return self._name


class Database(object):
	def __init__(self, folder):
		self._folder = folder
		self._conn = sql.connect(":memory:")
		self._conn.execute("ATTACH DATABASE '%s/%s' as CNames;" % (folder, fCompoundNames))
		self._conn.execute("ATTACH DATABASE '%s/%s' as C;" % (folder, fCompounds))
		self._conn.execute("ATTACH DATABASE '%s/%s' as R;" % (folder, fReactions))
		self._conn.execute("ATTACH DATABASE '%s/%s' as EP;" % (folder, fEductsProducts))

		self._compounds = {}
		self._reactions = {}
		self._reactionTypes = None

	def compoundFromId(self, id_):
		if id_ in self._compounds:
			return self._compounds[id_]
		res = self._conn.execute("SELECT compound_id FROM C.Compounds WHERE compound_id = ?;", (id_,))
		res = res.fetchone()
		if not res:
			return Compound()
		c = Compound(self, id_)
		self._compounds[id_] = c
		return c

	def reactionFromId(self, id_):
		if id_ in self._reactions:
			return self._reactions[id_]
		res = self._conn.execute("SELECT reaction_id FROM R.Reactions WHERE reaction_id = ?;", (id_,))
		res = res.fetchone()
		if not res:
			return Reaction()
		r = Reaction(self, id_)
		self._reactions[id_] = r
		return r

	@property
	def reactionTypes(self):
		if self._reactionTypes is None:
			res = self._conn.execute("SELECT reaction_record_name FROM R.ReactionRecordTypes ORDER BY reaction_record_id;")
			self._reactionTypes = [ReactionType(row[0]) for row in res]
		return self._reactionTypes
