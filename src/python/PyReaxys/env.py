import logging

logging.basicConfig(level=logging.INFO, format='%(asctime)s %(message)s')
l = logging.getLogger("PyReaxys")

maxSubfolder = 45
wilmerBase = "/scr/k70san/wilmer"
wilmerRawReactions = wilmerBase + "/RX-RXD-CIT"
wilmerRawMolecules = wilmerBase + "/substances"
base = "/scr/k61san/jlandersen"
origCompounds = base + "/0_orig_compounds"
origReactions = base + "/0_orig_reactions"
compoundsFromXml = base + "/1_compoundsFromXml"
compoundNamesFromXml = base + "/1_compoundNamesFromXml"
reactionsFromXml = base + "/1_reactionsFromXml"
eductProductSmilesFromXml = base + "/1_eductProductSmilesFromXml"

eductProduct = base + "/2_eductProduct"
