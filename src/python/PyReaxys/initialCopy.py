from .env import *
from .op import *

def reactions():
    msg = "Copying initial reaction files from Wilmer:"
    msgFolder = "Copying folder %d ..."
    def op(self, fSrc, fTar):
        for e in os.scandir(fSrc):
            if not e.name.endswith(".rdf"): continue
            t = os.path.basename(e.name)
            t = "%s/%sxml" % (fTar, t[:-3])
            shutil.copyfile("%s/%s" % (fSrc, e.name), t)
    return Op(msg, msgFolder, wilmerRawReactions, origReaction, op)

def molecules():
    msg = "Copying initial molecule files from Wilmer:"
    msgFolder = "Copying folder %d ..."
    def op(self, fSrc, fTar):
        for e in os.scandir(fSrc):
            if not e.name.endswith(".xml"): continue
            t = os.path.basename(e.name)
            t = "%s/%s" % (fTar, t)
            shutil.copyfile("%s/%s" % (fSrc, e.name), t)
    return Op(msg, msgFolder, wilmerRawMolecules, origMolecule, op)
