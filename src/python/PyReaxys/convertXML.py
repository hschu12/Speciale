import base64
import openbabel as ob
import pybel
import sqlite3 as sql
import xml.etree.ElementTree as xml
import zlib

from .env import *
from .db import *
from .op import *

def _unzipBase64(data):
    return zlib.decompress(base64.b64decode(data)).decode('utf-8')

# http://stackoverflow.com/questions/3443831/python-and-elementtree-return-inner-xml-excluding-parent-element
def content(t):
    return t.text + ''.join(str(xml.tostring(e)) for e in t)

class RXNFileEmpty(Exception):
    pass

class RXNFileMultipleRXN(Exception):
    pass

class RXNFileEmptyMolFile(Exception):
    pass

def _rxnToSmiles(rxnFileTree):
    res = rxnFileTree.text
    # some of them are encoded multiple times
    count = 0
    while len(res) > 0 and res.find("$RXN") == -1:
        res = _unzipBase64(res)
        count += 1
        if count == 10:
            print(">>>", res, "<<<")
            print(len(res))
            print("Something must be wrong.")
            assert False
    rxnFile = res
    if len(rxnFile) == 0:
        raise RXNFileEmpty()
    findRxn = rxnFile.find('$RXN')
    if findRxn == -1:
        print(">>>", rxnFile, "<<<")
        print(len(rxnFile))
        assert False
    else:
        oneMore = rxnFile.find('$RXN', findRxn + 1)
        if oneMore != -1:
            raise RXNFileMultipleRXN()
    firstMol = rxnFile.find('$MOL', findRxn)
    countLinePos = rxnFile.rfind('\n', findRxn, firstMol - 1)
    countLine = rxnFile[countLinePos:firstMol]
    counts = countLine.split()
    if len(counts) != 2:
        print(">>>", rxnFile, "<<<")
        print(len(rxnFile))
        print("Error in count line, len == %d != 2:\n\t%s" % (len(counts), countLine))
        print(counts)
    assert len(counts) == 2
    numEducts = int(counts[0])
    numProducts = int(counts[1])
    rxnFile = rxnFile[rxnFile.find('$MOL', findRxn):]
    rSmiles = []
    molFiles = list(rxnFile[findRxn:].split('M  END\n'))
    for molFile in molFiles[:-1]:
        molFile = molFile[molFile.find('$MOL') + 5:] + 'M  END'
        conv = ob.OBConversion()
        conv.SetInFormat('mol')
        conv.SetOutFormat('smi')
        mol = ob.OBMol()
        conv.ReadString(mol, molFile)
        mSmiles = conv.WriteString(mol).strip()
        regPos = molFile.find("M  REG")
        if regPos <= 0:
            print("rxnFile:", rxnFile)
            print("molFile:", molFile)
            if len(rSmiles) != numEducts + numProducts:
                print("len(rSmiles):", len(rSmiles))
                print("numEducts:   ", numEducts)
                print("numProducts: ", numProducts)
                raise RXNFileEmptyMolFile()
            elif sum(1 for c in molFile if c == "\n") != 6:
                assert False
            else:
                # strange empty'ish mol files:
                #   R>Mv4.0000000000002D 1   1.00000     0.00000     0
                #
                #   0  0  0  0  0  0  0  0  0  0999 V2000
                # M  STY  1   1 SUP
                # M  SMT   1
                # M  END
                continue
        assert regPos > 0
        regPosEnd = molFile.find("\n", regPos)
        assert regPosEnd > 0
        strReg = molFile[regPos + 7:regPosEnd]
        iReg = int(strReg)
        rSmiles.append((iReg, mSmiles))
        #print(">>>\n%s\n<<<" % molFile)
    if len(rSmiles) != numEducts + numProducts:
        print(">>>", rxnFile, "<<<")
        print(len(rxnFile))
        print("len(rSmiles):", len(rSmiles))
        print("numEducts:", numEducts)
        print("numProducts:", numProducts)
        print("countLine:", countLine)
        print("findRxn:", findRxn)
        print("firstMol:", firstMol)
        print("countLinePos:", countLinePos)
    assert len(rSmiles) == numEducts + numProducts
    educts = rSmiles[:numEducts]
    products = rSmiles[numProducts:]
    return educts, products

def compounds():
    msg = "Extracting compounds from XML:"
    msgFolder = "Converting folder %d ..."
    
    def begin(self):
        self.conn = sql.connect("%s/compounds.sqlite" % self.tarFolder)
        c = self.conn.cursor()
        c.executescript(compoundsSchema)
        self.conn.commit()
    def end(self):
        self.conn.commit()
        self.conn.close()
    def op(self, fSrc, fTar):
        c = self.conn.cursor()
        c.execute('pragma journal_mode = MEMORY;')
        for e in os.scandir(fSrc):
            assert e.name.endswith(".xml")
            fileId = int(os.path.basename(e.name)[:-4])
            tree = xml.parse("%s/%s" % (fSrc, e.name))
            root = tree.getroot()
            for mTree in root.iter('substance'):
                ideTree = mTree.find('IDE')
                mId = ideTree.find('IDE.XRN').text
                mId = int(mId)
                c.execute("INSERT INTO Compounds VALUES (?, ?, ?);", 
                          (mId, self.folderId, fileId))
    return Op(msg, msgFolder, origCompounds, compoundsFromXml, op, begin=begin, end=end, makeFolders=False)

def compoundNames():
    msg = "Extracting compound names from XML:"
    msgFolder = "Converting folder %d ..."
    
    def begin(self):
        self.conn = sql.connect("%s/compoundNames.sqlite" % self.tarFolder)
        c = self.conn.cursor()
        c.executescript(compoundNamesSchema)
        self.conn.commit()
    def end(self):
        self.conn.commit()
        self.conn.close()
    def op(self, fSrc, fTar):
        c = self.conn.cursor()
        c.execute('pragma journal_mode = MEMORY;')
        for e in os.scandir(fSrc):
            assert e.name.endswith(".xml")
            fileId = int(os.path.basename(e.name)[:-4])
            tree = xml.parse("%s/%s" % (fSrc, e.name))
            root = tree.getroot()
            for mTree in root.iter('substance'):
                ideTree = mTree.find('IDE')
                mId = ideTree.find('IDE.XRN').text
                mId = int(mId)
                for name in ideTree.iter('IDE.CN'):
                    n = content(name).strip()
                    c.execute("INSERT INTO CompoundNames VALUES (?, ?, ?, ?);",
                              (mId, n, self.folderId, fileId))
    return Op(msg, msgFolder, origCompounds, compoundNamesFromXml, op, begin=begin, end=end, makeFolders=False)

def reactions():
    msg = "Extracting reactions from XML:"
    msgFolder = "Converting folder %d ..."

    def begin(self):
        self.conn = sql.connect("%s/reactions.sqlite" % self.tarFolder)
        c = self.conn.cursor()
        c.executescript(reactionsSchema)
        self.conn.commit()
    def end(self):
        self.conn.commit()
        self.conn.close()
    def op(self, fSrc, fTar):
        c = self.conn.cursor()
        c.execute('pragma journal_mode = MEMORY;')
        for e in os.scandir(fSrc):
            assert e.name.endswith(".xml")
            fileId = int(os.path.basename(e.name)[:-4])
            tree = xml.parse("%s/%s" % (fSrc, e.name))
            root = tree.getroot()
            for rTree in root.iter('reaction'):
                rxTree = rTree.find('RX')
                rId = rxTree.find('RX.ID').text
                rId = int(rId)
                c.execute("INSERT INTO Reactions VALUES (?, ?, ?);", (rId, self.folderId, fileId))
                for rTypeTree in rxTree.iter('RX.RTYP'):
                    rType = rTypeTree.text.strip()
                    c.execute("INSERT OR IGNORE INTO ReactionRecordTypes(reaction_record_name) VALUES (?)", (rType,))
                    c.execute('SELECT reaction_record_id FROM ReactionRecordTypes WHERE reaction_record_name=?', (rType,))
                    rrId = c.fetchone()[0]
                    c.execute("INSERT INTO ReactionRecords VALUES (?, ?, ?, ?);",
                              (rId, rrId, self.folderId, fileId))
    return Op(msg, msgFolder, origReactions, reactionsFromXml, op, begin=begin, end=end, makeFolders=False)

def reactionEductsProductsSmiles():
    msg = "Extracting educts, products, and SMILES strings from XML:"
    msgFolder = "Converting folder %d ..."

    def op(self, fSrc, fTar):
        conn = sql.connect("%s/eductsProducts.sqlite" % fTar)
        c = conn.cursor()
        c.execute('pragma journal_mode = MEMORY;')
        c.executescript(eductProductSchema)
        c.executescript(smilesSchema)
        conn.commit()
        with open("%s/rxnFileEmpty.txt" % fTar, "w") as fRxnFileEmpty, open("%s/rxnFileMulipleRXN.txt" % fTar, "w") as fRxnFileMultipleRXN, open("%s/rxnFileEmptyMolFile.txt" % fTar, "w") as fRxnFileEmptyMolFile: 
            for e in os.scandir(fSrc):
                assert e.name.endswith(".xml")
                fileId = int(os.path.basename(e.name)[:-4])
                tree = xml.parse("%s/%s" % (fSrc, e.name))
                root = tree.getroot()
                for rTree in root.iter('reaction'):
                    rxTree = rTree.find('RX')
                    rId = rxTree.find('RX.ID').text
                    rId = int(rId)
                    rxnFileTree = rxTree.find('RX.RXNFILE')
                    if rxnFileTree is not None:
                        try:
                            educts, products = _rxnToSmiles(rxnFileTree)
                        except RXNFileEmpty:
                            fRxnFileEmpty.write("%s %s  %s\n" % (fSrc, fileId, rId))
                        except RXNFileMultipleRXN:
                            fRxnFileMultipleRXN.write("%s %s  %s\n" % (fSrc, fileId, rId))
                        except RXNFileEmptyMolFile:
                            fRxnFileEmptyMolFile.write("%s %s  %s\n" % (fSrc, fileId, rId))
                        except AssertionError:
                            print("fSrc   =", fSrc)
                            print("fileId =", fileId)
                            print("rId    =", rId)
                            raise
                    eductData = {}
                    productData = {}
                    smilesData = []
                    for mId, smiles in educts:
                        if mId not in eductData:
                            eductData[mId] = (rId, mId, 1, self.folderId, fileId)
                        else:
                            t = eductData[mId]
                            eductData[mId] = (t[0], t[1], t[2] + 1, t[3], t[4])
                        smilesData.append((mId, smiles, rId, self.folderId, fileId)) 
                    for mId, smiles in products:
                        if mId not in productData:
                            productData[mId] = (rId, mId, 1, self.folderId, fileId)
                        else:
                            t = productData[mId]
                            productData[mId] = (t[0], t[1], t[2] + 1, t[3], t[4])
                        smilesData.append((mId, smiles, rId, self.folderId, fileId)) 
                    c.executemany("INSERT INTO Educts VALUES (?, ?, ?, ?, ?);", eductData.values())
                    c.executemany("INSERT INTO Products VALUES (?, ?, ?, ?, ?);", productData.values())
                    c.executemany("INSERT INTO Smiles VALUES (?, ?, ?, ?, ?);", smilesData)
        conn.commit()
        c.close()
        conn.close()
    return Op(msg, msgFolder, origReactions, eductProductSmilesFromXml, op)


def mergeEductProduct():
    msg = "Merging educt and product tables:"
    msgFolder = "Converting folder %d ..."

    def begin(self):
        self.conn = sql.connect("%s/eductsProducts.sqlite" % self.tarFolder)
        c = self.conn.cursor()
        c.execute('pragma journal_mode = MEMORY;')
        c.executescript(eductProductSchema)
        self.conn.commit()
    def end(self):
        self.conn.commit()
        self.conn.close()
    def op(self, fSrc, fTar):
        c = self.conn.cursor()
        c.execute('pragma journal_mode = MEMORY;')
        f = fSrc + "/eductsProducts.sqlite"
        c.execute("attach '%s' as src;" % f)
        c.execute("INSERT INTO Educts SELECT * FROM src.Educts;")
        c.execute("INSERT INTO Products SELECT * FROM src.Products;")
        self.conn.commit()
        c.execute("detach src;")
    return Op(msg, msgFolder, eductProductSmilesFromXml , eductProduct, op, begin=begin, end=end, makeFolders=False)
